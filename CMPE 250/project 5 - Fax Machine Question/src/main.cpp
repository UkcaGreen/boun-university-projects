#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <cstring>

using namespace std;

struct Word{
    string word;
    long long int hash;
};

struct Dictionary{
    long long int size;
    vector<Word> words;
};

string message;
long long int messageHash[1005];
long long int basePow[1005];
int dyn[1005];
Dictionary dic;
long long int prime = 1000000007;
long long int base = 29;

long long int myHash(string& s){
    long long int h = 0;
    for (int i = 0; i < s.length(); ++i) {
        h = (h*base+(s[i]-96))%prime;
        if(h<0){
            h += prime;
        }
    }
    return h;
}

long long int findHash(int begin, int end){
    if(begin == 0){
        return messageHash[end];
    }
    return (((messageHash[end] - messageHash[begin-1] * basePow[end-begin+1])%prime)+prime)%prime;
}

int F(int index){
    if(dyn[index] != -1){
        return dyn[index];
    }
    int result = 0;
    if(index > message.length()){
        return 0;
    }
    if(index == message.length()){
        return 1;
    }
    for (Word& s: dic.words) {
        if(index+(int)(s.word.length()) > message.length()){
            continue;
        }
        if(s.hash == findHash(index,index+(int)(s.word.length())-1)){
            result = (result + F(index+(int)(s.word.length())))%prime;
            if(result < 0){
                result += prime;
            }
            //check the strings
        }
    }
    dyn[index] = result;
    return result;
}

int main(int argc, char* argv[]) {

    ios_base::sync_with_stdio(false);

    if (argc != 3) {
        cout << "Run the code with the following command: ./project5 [input_file] [output_file]" << endl;
        return 1;
    }

    memset(dyn,-1,sizeof dyn);

    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);

    infile >> message >> dic.size;

    //fill basePow
    basePow[0] = 1;
    for (int k = 1; k <= 1000; ++k) {
        basePow[k] = (basePow[k-1]*base)%prime;
    }

    //fill dictionary and hashes
    for (int i = 0; i < dic.size; ++i) {
        Word temp;
        infile >> temp.word;
        temp.hash = myHash(temp.word);
        dic.words.push_back(temp);
    }

    //fill messageHash
    for (int j = 0; j < message.length(); ++j) {
        if(j == 0){
            messageHash[j] = (message[j]-96)%prime;
        } else{
            messageHash[j] = (messageHash[j-1]*base+(message[j]-96))%prime;
        }
        if(messageHash[j]<0){
            messageHash[j] += prime;
        }
    }

    int ans = F(0);

    // here, perform the output operation. in other words,
    // prlong long int your results long long into the file named <argv[2]>
    ofstream myfile;
    myfile.open (argv[2]);

    cout << ans << endl;

    myfile << ans;

    myfile.close();

    return 0;
}