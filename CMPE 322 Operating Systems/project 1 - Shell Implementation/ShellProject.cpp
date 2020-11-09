#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bits/stdc++.h>

using namespace std;

bool isThereArrow = false;	//isThereArrow flag
bool isTherePipe = false;	//isTherePipe flag
vector<string> rest;		//holds the second part of the command

//translation map
//translates commands to vanilla linux forms
map<string,string> tmap = {
		{"listdir","ls"},
		{"currentpath","pwd"},
		{"printfile","cat"},
		{"footprint","history"},
		{"exit","exit"}
};

//history deque
//holds recent commands
deque<string> history;

//adds a command to history
void addToHist(string line){
	//if new command same with the last command don't add to history
	if(history.size() != 0 && history.back() == line) return;

	//add to history
	history.push_back(line);

	//keep number of commands in the history les than or equal to 15
	while(history.size()>15){
		history.pop_front();
	}
}

//prints the history
void printHistory(){
	for(unsigned int i=0; i<history.size(); i++){
		//take current command
		string currCommand = history.front();

		//recycle the element
		history.pop_front();
		history.push_back(currCommand);

		//prints the current command with index
		cout << i + 1 << " " << currCommand << endl;
	}
	exit(0);
}

//strips the quotes
string stripQuote(string s){
	if(s[0] == '"' && s[s.size()-1] == '"'){
		//in order to getrid of quotes
		//removes first and last index of the string
		return s.substr(1,s.size()-2);
	}else{
		//if there are no quotes exit
		exit(1);
		return s;
	}
}

//converts string to char*
char* convertString(const string &s){
	//create a char array
	char *charPointer = new char[s.size()+1];
	//copies string into char array
	strcpy(charPointer, s.c_str());
	//returns char array
	return charPointer;
}

//tokenizes the command
vector<string> tokenize(string arg_line){

	string token;
	vector<string> tokens;

	//creates a string stream to tokenize command
	stringstream ss(arg_line);

	while(ss >> token){

		//if command contains ">" makes isThereArrow flag true
		if(token == ">"){
			isThereArrow=true;
			continue;
		}

		//if command contains "|" makes isTherePipe flag true
		if(token == "|"){
			isTherePipe=true;
			continue;
		}

		if(!(isTherePipe || isThereArrow))
			//saves the tokens of the main part of the command
			tokens.push_back(token);
		else
			//saves the tokens of the second (after the ">" or "|") part of the command
			rest.push_back(token);
	}

	//if tokens vector is not empty translates the command into vanilla linux
	if(!tokens.empty())
		tokens[0] = tmap[tokens[0]];

	//returns tokens
	return tokens;
}

//executes basic commands
void basicExecute(vector<string> &args){
	//creates char pointer array
	char* cArgs[args.size()+1];

	//fills the char pointer array with the args vector
	for(int i=0; i<int (args.size()); i++){
		cArgs[i] = convertString(args[i]);
	}

	//adds NULL to the end of the cArgs
	cArgs[args.size()] = NULL;

	//calls exec for the command
	execvp(cArgs[0],cArgs);

	//free's cArgs
	free(cArgs);
}

//executes the commands that contains arrow
void arrowExecute(vector<string> &args){
	int fileDesc;	//file descriptor

	//opens the file with given name (creates if not exist)
	fileDesc = open(convertString(rest[0]), O_CREAT | O_TRUNC | O_WRONLY );

	//redirects stdout to fileDesc
	dup2(fileDesc, 1);

	//executes the command
	basicExecute(args);

	//closes the file
	close(fileDesc);
}

//executes the commands that contains pipe
void pipeExecute(vector<string> &args){
	pid_t pid;				//process id
	int pipeFileDesc[2];	//file descriptors of pipe
	int status;				//status of wait

	//create pipe
	pipe(pipeFileDesc);

	//forks a child process
	pid = fork();

	if(pid == 0){
		//child

		//directs stdout out to pipe in
		dup2(pipeFileDesc[1],1);
		close(pipeFileDesc[0]);
		close(pipeFileDesc[1]);

		//executes the command
		basicExecute(args);

	}else{
		//parent

		//directs pipe out to stdin
		dup2(pipeFileDesc[0],0);
		close(pipeFileDesc[1]);
		close(pipeFileDesc[0]);

		//waits child process to complete
		waitpid(pid, &status, WUNTRACED);

		vector<string> tempVec;
		string grepToken = "";

		//unifies grep pattern int one token
		//(it might become more than one token while tokenizing with respect to spaces)
		for(unsigned int i=1; i<rest.size(); i++){
			grepToken += rest[i];
			if(i != (rest.size()-1))
				grepToken += " ";
		}

		//removes the quotes
		grepToken = stripQuote(grepToken);

		//create a temp vector to pass basicExecute with the proper grep pattern token
		tempVec.push_back(rest[0]);
		tempVec.push_back(grepToken);

		//executes the command
		basicExecute(tempVec);
	}
}

//executes commands respect to their types
int execute(vector<string> args){

	pid_t pid;			//process id
	int status;			//status of wait

	//forks a child process
	pid = fork();
	if(pid==0){
		//child process

		if(isThereArrow){
			//executes command that contains ">"
			arrowExecute(args);
		}else if(isTherePipe){
			//executes command that contains "|"
			pipeExecute(args);
		}else if(args[0] == "history"){
			//executes history (footprint) command
			printHistory();
		}else{
			//executes general commands
			basicExecute(args);
		}

	}else{
		//parent
		//waits child to finish
		waitpid(pid, &status, WUNTRACED);
	}

	//clears isThereArrow and isTherePipe flags
	isThereArrow = false;
	isTherePipe = false;
	//clears the "rest" vector
	rest.clear();

	return 1;
}

//loop that takes commands
int loop(){
	string line;			//a line of command as string
	vector<string> args;	//tokens array of the command
	int status = 1;			//process status

	do{

		//print username
		cout << getenv("USER") << " >>> ";

		//takes a line of command as a string
		getline(cin,line);

		//if command is empty string, continue
		if(line == "") continue;

		//tokenize command according to white space
		args = tokenize(line);

		//if args empty, continue
		if(args.empty()) continue;

		//add to history
		addToHist(line);

		//if command is undefined continues
		//(if command is not in the tmap, tokenize function makes arg[0] empty string)
		if(args[0] == "") continue;

		//exits program when exit command occurs
		if(args[0] == "exit") exit(0);

		//executes the command
		status = execute(args);

		//clears the variables before next iteration
		line.clear();
		args.clear();
	}while(status);

	return 0;
}

int main(int argc, char **argv) {

	//command loop
	loop();

	return 0;
}
