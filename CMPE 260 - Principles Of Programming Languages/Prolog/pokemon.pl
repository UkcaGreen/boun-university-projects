% emilcan arıcan
% 2016400231
% compiling:  yes
% complete:  yes

%facts.
:- [pokemon_data].

%takes first N elements of the list.
take_n(N,[X|R],FinalList) :- (N > 1 -> take_n(N-1,R,PreList);(PreList = [])),FinalList = [X|PreList].

%finds max of two numbers.
find_max(X,Y,Z) :-
	X >= Y, Z is X,!;
	Y >= X, Z is Y.

%adds an element to the list.
add_to_list(X,List,[X|List]).

%finds the intersection list of the two lists.
intersect(List1,List2,IntersectionList) :-
	findall(Item,(member(Item,List1),member(Item,List2)),IntersectionList).

%finds the highest possible evolution of a pokemon at a particular level.
% find_pokemon_evolution(+PokemonLevel, +Pokemon, -EvolvedPokemon)
find_pokemon_evolution(PokemonLevel,Pokemon,EvolvedPokemon) :-
	pokemon_evolution(Pokemon,Next,Z),PokemonLevel >= Z,
	find_pokemon_evolution(PokemonLevel,Next,EvolvedPokemon),!;
	EvolvedPokemon = Pokemon.


%calculates stats of any pokemon at a particular level.
%pokemon_level_stats(+PokemonLevel, ?Pokemon, -PokemonHp, -PokemonAttack, -PokemonDefense)
pokemon_level_stats(PokemonLevel, Pokemon, PokemonHp, PokemonAttack, PokemonDefense) :-
	pokemon_stats(Pokemon, _, HealthPoint, Attack, Defense), 
	PokemonHp is HealthPoint + 2*PokemonLevel,
	PokemonAttack is Attack + PokemonLevel,
	PokemonDefense is Defense + PokemonLevel.


%helper for single_type_multiplier predicate.
%traverses type list and multiplier list concurrently to reach wanted spots.
stm_helper(AttackerType,DefenderType,[H1|R1],[H2|R2],Result) :-
	DefenderType = H2,Result is H1;
	stm_helper(AttackerType,DefenderType,R1,R2,Result).

%among AttackerType, DefenderType or Multiplier derives uninitialized ones respect to the given ones.
%single_type_multiplier(?AttackerType, ?DefenderType, ?Multiplier)
single_type_multiplier(AttackerType, DefenderType, Multiplier) :-
	type_chart_attack(AttackerType,MultiplierList),pokemon_types(TypeList),
	stm_helper(AttackerType,DefenderType,MultiplierList,TypeList,Multiplier).


%calculates Multiplier or AttackerType in the case that types given as list.
%type_multiplier(?AttackerType, +DefenderTypeList, ?Multiplier)
type_multiplier(AttackerType,DefenderTypeList,Multiplier) :-
	DefenderTypeList = [FirstType,SecondType],
	single_type_multiplier(AttackerType,FirstType,Multiplier1),
	single_type_multiplier(AttackerType,SecondType,Multiplier2),
	Multiplier is Multiplier1*Multiplier2;
	DefenderTypeList = [Type],
	single_type_multiplier(AttackerType,Type,Multiplier).


%helper for pokemon_type_multiplier predicate.
%according to AttackerTypeList and DefenderTypeList calculates maximum multiplier.	
ptm_helper(AttackerTypeList,DefenderTypeList,Multiplier) :-
	AttackerTypeList = [AttackerFirstType,AttackerSecondType],
	type_multiplier(AttackerFirstType,DefenderTypeList,Multiplier1),
	type_multiplier(AttackerSecondType,DefenderTypeList,Multiplier2),
	find_max(Multiplier1,Multiplier2,Multiplier),!;
	AttackerTypeList = [AttackerFirstType],
	type_multiplier(AttackerFirstType,DefenderTypeList,Multiplier).

%calculates the multiplier respect to AttackerPokemon and DefenderPokemon.
%extracts type list of AttackerPokemon and DefenderPokemon and calls ptm_helper.
%pokemon_type_multiplier(?AttackerPokemon, ?DefenderPokemon, ?Multiplier)
pokemon_type_multiplier(AttackerPokemon,DefenderPokemon,Multiplier) :- 
	pokemon_stats(AttackerPokemon, AttackerTypeList, _, _, _),
	pokemon_stats(DefenderPokemon, DefenderTypeList, _, _, _),
	ptm_helper(AttackerTypeList, DefenderTypeList, Multiplier).


%first derives the multiplier then calculates Damage that the AttackerPokemon dealt.
%pokemon_attack(+AttackerPokemon, +AttackerPokemonLevel, +DefenderPokemon, +DefenderPokemonLevel, -Damage)
pokemon_attack(AttackerPokemon,AttackerPokemonLevel,DefenderPokemon,DefenderPokemonLevel,Damage) :-
	pokemon_level_stats(AttackerPokemonLevel,AttackerPokemon,_,AttackerPokemonAttack,_),
	pokemon_level_stats(DefenderPokemonLevel,DefenderPokemon,_,_,DefenderPokemonDefense),
	pokemon_type_multiplier(AttackerPokemon,DefenderPokemon,TypeMultiplier),
	Damage is (0.5*AttackerPokemonLevel*(AttackerPokemonAttack/DefenderPokemonDefense)*TypeMultiplier)+1.


%helper for pokemon_fight predicate
%recursively decreases health points of the pokemon subsequently derives final health points.
%also counts rounds of the fight.
pf_helper(Pokemon1Damage,Pokemon2Damage,CurrHp1,CurrHp2,Rounds,Counter,LastHp1,LastHp2) :- 
	NewHp1 is CurrHp1 - Pokemon2Damage,
	NewHp2 is CurrHp2 - Pokemon1Damage,
	CurrHp1 >= 0, CurrHp2 >= 0,
	pf_helper(Pokemon1Damage,Pokemon2Damage,NewHp1,NewHp2,Rounds,Counter+1,LastHp1,LastHp2),!;
	Rounds is Counter,LastHp1 is CurrHp1,LastHp2 is CurrHp2.

%calculates final health points of the battled pokemon and the number of rounds.
%prepares proper inputs for the pf_helper, then calls the pf_helper.
%pokemon_fight(+Pokemon1, +Pokemon1Level, +Pokemon2, +Pokemon2Level, -Pokemon1Hp, -Pokemon2Hp, -Rounds)
pokemon_fight(Pokemon1,Pokemon1Level,Pokemon2,Pokemon2Level,Pokemon1Hp,Pokemon2Hp,Rounds) :-
	pokemon_attack(Pokemon1,Pokemon1Level,Pokemon2,Pokemon2Level,Pokemon1Damage),
	pokemon_attack(Pokemon2,Pokemon2Level,Pokemon1,Pokemon1Level,Pokemon2Damage),
	pokemon_level_stats(Pokemon1Level,Pokemon1,CurrHp1,_,_),
	pokemon_level_stats(Pokemon2Level,Pokemon2,CurrHp2,_,_),
	pf_helper(Pokemon1Damage,Pokemon2Damage,CurrHp1,CurrHp2,Rounds,0,Pokemon1Hp,Pokemon2Hp).


%takes list of pokemons and list of levels and creates a list of pokemons at the highest possible evolutions of the first list.
pt_evolution([],[],[]).
pt_evolution([Pokemon|PRest],[Level|LRest],FinalList) :-
	find_pokemon_evolution(Level,Pokemon,Evolved),
	pt_evolution(PRest,LRest,Rest),add_to_list(Evolved,Rest,FinalList).

%keeps track of the winners while traversing pokemon lists of the trainers concurrently and battling those pokemon.
%creates a list with those winners.
multi_fight(_,[],[],_,[],[],[]).
multi_fight(Trainer1,[Pokemon1|PokemonRest1],[Level1|LevelRest1],Trainer2,[Pokemeon2|PokemonRest2],[Level2|LevelRest2],WinnerList) :-
	pokemon_fight(Pokemon1,Level1,Pokemeon2,Level2,Health1,Health2,_),
	(Health1>Health2 -> Winner = Trainer1;(Health2>Health1 -> Winner = Trainer2;Winner=Trainer1)),
	multi_fight(Trainer1,PokemonRest1,LevelRest1,Trainer2,PokemonRest2,LevelRest2,WinnerListRest),
	add_to_list(Winner,WinnerListRest,WinnerList).

%takes level list and the pokemon list of the trainers.
%evolves the pokemon if they can.
%simulates the battles and keeps track of the winners.
%pokemon_tournament(+PokemonTrainer1, +PokemonTrainer2, -WinnerTrainerList)
pokemon_tournament(PokemonTrainer1,PokemonTrainer2,WinnerTrainerList) :-
	pokemon_trainer(PokemonTrainer1,PokemonList1,LevelList1),
	pokemon_trainer(PokemonTrainer2,PokemonList2,LevelList2),
	pt_evolution(PokemonList1,LevelList1,FinalList1),
	pt_evolution(PokemonList2,LevelList2,FinalList2),
	multi_fight(PokemonTrainer1,FinalList1,LevelList1,PokemonTrainer2,FinalList2,LevelList2,WinnerTrainerList).

%finds the best pokemon againist the EnemyPokemon at a particular level and the RemainingHp from the fight.
%creates a key-value pair list from pokemon and remaining healths after the fight, then sorts the list.
%finds all pokemons with the max remaining hp (which is remaining hp of the first item of the list).
%best_pokemon(+EnemyPokemon, +LevelCap, -RemainingHp, -BestPokemon)
best_pokemon(EnemyPokemon,LevelCap,RemainingHp,BestPokemon) :-
	findall(Item,(pokemon_fight(EnemyPokemon,LevelCap,Pokemon,LevelCap,_,Health2,_),Key is (Health2*(-1)),Item = Key-Pokemon),PokemonPairList),
	keysort(PokemonPairList,SortedList), SortedList = [BestKey-_|_],
	findall(Item2,(member(BestKey-Item2,SortedList)),List), member(BestPokemon,List), RemainingHp is (BestKey*(-1)).


%takes one item from each list and finds best pokemon respect to this data then adds it to the list.
%and keeps doing it until lists got empty.
bpt_helper([],[],[]).
bpt_helper([Pokemon|PokemonRest],[Level|LevelsRest],BestPokemonList) :-
	bpt_helper(PokemonRest,LevelsRest,BestPokemonListRest),
	best_pokemon(Pokemon,Level,_,BestPokemon),
	add_to_list(BestPokemon,BestPokemonListRest,BestPokemonList).

%finds best pokemon team againist given pokemon team.
%derives pokemon list of the opponent and levels of those pokemon then finds best pokemons one by one.
%best_pokemon_team(+OpponentTrainer, -PokemonTeam)
best_pokemon_team(OpponentTrainer,PokemonTeam) :-
	pokemon_trainer(OpponentTrainer,PokemonList,LevelList),
	bpt_helper(PokemonList,LevelList,PokemonTeam).


%finds pokemons that are in the InitialPokemonList and whose at least one type appears at TypeList.
%then calls sort to get rid of duplicates.
%pokemon_types(+TypeList, +InitialPokemonList, -PokemonList)
pokemon_types(TypeList,InitialPokemonList,PokemonList) :-
	findall(Pokemon,(member(Pokemon,InitialPokemonList),pokemon_stats(Pokemon,Types,_,_,_),intersect(Types,TypeList,IntersectionList),\+ (IntersectionList = [])),PokemonList).


%removes keys from the key-value list to create value list.
key_strip([],[]).
key_strip([_-Value|Rest],ValueList) :- 
	key_strip(Rest,ValueListRest),
	ValueList = [Value|ValueListRest].

%finds pokemons that have liked type while does not have any disliked type.
%creates key-value pairs those pokemon according to given Criterion.
%sorts the list respect to key values.
%takes first Count items of the list.
%get rid of the keys and creates values list which is the PokemonTeam.
%generate_pokemon_team(+LikedTypes, +DislikedTypes, +Criterion, +Count, -PokemonTeam)
generate_pokemon_team(LikedTypes, DislikedTypes, Criterion, Count, PokemonTeam) :-
	((Criterion = h) -> findall(X,(pokemon_stats(P,Types,H,A,D),intersect(Types,LikedTypes,LT),\+ (LT = []),intersect(Types,DislikedTypes,DT),DT = [],K is H*(-1),X=K-[P,H,A,D]),PotentialPokemon);
	(Criterion = a -> findall(X,(pokemon_stats(P,Types,H,A,D),intersect(Types,LikedTypes,LT),\+ (LT = []),intersect(Types,DislikedTypes,R),R = [],K is A*(-1),X=K-[P,H,A,D]),PotentialPokemon);
	(Criterion = d -> findall(X,(pokemon_stats(P,Types,H,A,D),intersect(Types,LikedTypes,LT),\+ (LT = []),intersect(Types,DislikedTypes,R),R = [],K is D*(-1),X=K-[P,H,A,D]),PotentialPokemon)))),
	keysort(PotentialPokemon,SortedList),
	take_n(Count,SortedList,PrePokemonTeam),
	key_strip(PrePokemonTeam,PokemonTeam).
	