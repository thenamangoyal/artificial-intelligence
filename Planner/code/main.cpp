#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

enum action_name {PICK, UNSTACK, RELEASE, STACK};

class state;
class action;
class node;	// Node for BFS and A* Search
class element; // Element Goal Stack

// Comparator for state
class statepointer;
// Comparator for priority
class Comparepriority;

long long int bfsnodesexpanded;
long long int astarnodesexpanded;
long long int maxsizegoalstack;

int N; // No of blocks
bool admissible; // To use admissible or inadmissible

int heuristic(const state& s, const state& goal);
bool bfs(const state& initial, const state& goal, list<action>& plan);
bool astar(const state& initial, const state& goal, list<action>& plan);
bool goalstack(const state& initial, const state& goal, list<action>& plan);
void help();

class state{
public:
	set<pair<int,int> >  on;
	set<int> ontable;
	set<int> clearblock;
	set<int> hold;
	bool armempty;
public:
	state(): armempty(false) {}
	void generateSuccessor(vector<action>& action_list, vector<state>& successor);
	void print(ostream& output = std::cout) const;
	void printtable(ostream& output = std::cout) const;

	bool operator==(const state& p) const{
		return (on == p.on && ontable == p.ontable && clearblock == p.clearblock && hold == p.hold && armempty == p.armempty);
	}
	bool operator<(const state& p) const{
		if (on != p.on){
			return (on < p.on);
		}
		else if (ontable != p.ontable){
			return (ontable < p.ontable);
		}
		else if (clearblock != p.clearblock){
			return (clearblock < p.clearblock);
		}
		else if (hold != p.hold){
			return (hold < p.hold);
		}
		return armempty < p.armempty;
	}
	bool goalTest (const state& testgoal) const;

	bool checkState () const;
};

class action{
public:
	int name;
	vector<int> blocks;
public:
	action() {}
	bool operator==(const action& p) const{
		return (name == p.name && blocks== p.blocks);
	}
	bool operator<(const action& p) const{
		if (name != p.name){
			return (name < p.name);
		}
		return blocks < p.blocks;
	}
	void print(ostream& output = std::cout) const;
};


class node{
public:
	state* curr;
	action* leads;
	node* parent;
	int path_cost;

};

class statepointer{
public:
  bool operator() (const state* lhs, const state* rhs) const
  {return (*lhs)<(*rhs);}
};

class Comparepriority
{
public:
	bool operator() (const pair<node*, int>& p1, const pair<node*, int>& p2)
	{
		return (p1.second> p2.second);
	}
};



class element{
public:
	int what;
	state* conjunct;	// Precinditions literals are all positive
	action* a;
	pair<int,int> on;
	int ontable;
	int clearblock;
	int hold;
	bool armempty;
public:
	element(): what(0), conjunct(NULL), a(NULL){}
};

int main(int argc, char *argv[])
{
	bool type_set_user = false;
	char user_type;

	for (int i=1; i< argc; i++){
		string token = string(argv[i]);
		if (token == "--help" || token == "-h"){
			help();
			return 1;
		}
	}

	if (argc< 2){
		cout<<"Please specify input file name and output file name"<<endl;
		help();
		return 1;
	}
	else if (argc< 3){
		cout<<"Please specify output file name"<<endl;
		help();
		return 1;
	}

	srand(time (NULL));
	admissible = true;
	bool print_state = false;

	bool error_parse = false;
	string error_string;

	set<string> done_token;
	for (int i=3; i< argc && !error_parse; i++){
		string token = string(argv[i]);
		if (done_token.find(token) != done_token.end()){

			error_parse = true;
			error_string = "Repeat token " + token;
		}
		else if (token == "--inadmissible" || token == "-i"){
			admissible = false;
			done_token.insert(string("--inadmissible"));
			done_token.insert(string("-i"));
		}
		else if (token == "--print" || token == "-p"){
			print_state = true;
			done_token.insert(string("--print"));
			done_token.insert(string("-p"));
		}
		else if (token == "--bfs" || token == "-f"){
			if (type_set_user){
				error_parse = true;
				error_string = "Planner already set";
				switch(user_type){
					case 'f' : error_string += " as bfs";break;
					case 'a' : error_string += " as astar";break;
					case 'g' : error_string += " as goalstack";break;
				}
				done_token.insert(string("--bfs"));
				done_token.insert(string("-f"));
			}
			else{
				type_set_user = true;
				user_type = 'f';
			}	
		}
		else if (token == "--astar" || token == "-a"){
			if (type_set_user){
				error_parse = true;
				error_string = "Planner already set";
				switch(user_type){
					case 'f' : error_string += " as bfs";break;
					case 'a' : error_string += " as astar";break;
					case 'g' : error_string += " as goalstack";break;
				}
				done_token.insert(string("--astar"));
				done_token.insert(string("-a"));
			}
			else{
				type_set_user = true;
				user_type = 'a';
			}
		}
		else if (token == "--goalstack" || token == "-g"){
			if (type_set_user){
				error_parse = true;
				error_string = "Planner already set";
				switch(user_type){
					case 'f' : error_string += " as bfs";break;
					case 'a' : error_string += " as astar";break;
					case 'g' : error_string += " as goalstack";break;
					done_token.insert(string("--goalstack"));
					done_token.insert(string("-g"));
				}
			}
			else{
				type_set_user = true;
				user_type = 'g';
			}
		}
		else{
			error_parse = true;
			error_string = "Invalid parameter " + token;
		}
	}

	if (error_parse){
		cout<<error_string<<endl;
		help();
		return 1;
	}
	
	ifstream input_file;
	ofstream output_file;
	input_file.open(argv[1],ios::in);
	output_file.open(argv[2], ios::out | ios::trunc);
	if (input_file.fail()) {
		cout<<"Invalid Input file"<<endl;
		return 1;
	}

	if (output_file.fail()) {
		cout<<"Invalid Output file"<<endl;
		return 1;
	}

	state initial, goal;


	char type;
	string line, temp, token;


	if (getline(input_file, line)){
		stringstream line_stream(line);
		line_stream>>N;
	}
	if (getline(input_file, line)){
		stringstream line_stream(line);
		line_stream>>type;
	}

	if (getline(input_file, line)){
		stringstream line_stream(line);
		line_stream>>temp;
	}



	if (temp == "initial" && getline(input_file, line)){
		stringstream line_stream(line);
		while(getline(line_stream, token, ')')){
			int i;
			for (i=0; i<token.size(); i++){
				if (token[i] == '('){
					break;
				}
			}
			if (i!=token.size()){
				token = token.substr(i+1);
				stringstream token_stream(token);
				int a,b;
				string state_list;
				token_stream>>state_list;

				initial.armempty = false;
				if (state_list == "on"){
					token_stream>>a>>b;
					initial.on.insert(make_pair<int,int>(a,b));
				}
				else if (state_list == "ontable"){
					token_stream>>a;
					initial.ontable.insert(a);
				}
				else if (state_list == "clear"){
					token_stream>>a;
					initial.clearblock.insert(a);
				}
				else if (state_list == "hold"){
					token_stream>>a;
					initial.hold.insert(a);
					initial.armempty =false;
				}
				else if (state_list == "empty"){
					initial.armempty =true;
				}
			}
		}
	}

	if (getline(input_file, line)){
		stringstream line_stream(line);
		line_stream>>temp;
	}

	if (temp == "goal" && getline(input_file, line)){
		stringstream line_stream(line);
		while(getline(line_stream, token, ')')){
			int i;
			for (i=0; i<token.size(); i++){
				if (token[i] == '('){
					break;
				}
			}
			if (i!=token.size()){
				token = token.substr(i+1);
				stringstream token_stream(token);
				int a,b;
				string state_list;
				token_stream>>state_list;

				goal.armempty = false;
				if (state_list == "on"){
					token_stream>>a>>b;
					goal.on.insert(make_pair<int,int>(a,b));
				}
				else if (state_list == "ontable"){
					token_stream>>a;
					goal.ontable.insert(a);
				}
				else if (state_list == "clear"){
					token_stream>>a;
					goal.clearblock.insert(a);
				}
				else if (state_list == "hold"){
					token_stream>>a;
					goal.hold.insert(a);
					goal.armempty =false;
				}
				else if (state_list == "empty"){
					goal.armempty =true;
				}
			}
		}
	}
	input_file.close();

	list<action> plan;
	bfsnodesexpanded = 0;
	astarnodesexpanded = 0;
	maxsizegoalstack = 0;

	cout<<"-------------------"<<endl;

	if (print_state){
		cout<<"Initial State"<<endl;
		initial.printtable();

		cout<<"Goal State"<<endl;
		goal.printtable();
	}
	if ((type == 'f' && !type_set_user) || user_type == 'f'){
		cout<<"Forward planner with BFS"<<endl;
		bfs(initial, goal, plan);
		cout<<"Nodes expanded: "<<bfsnodesexpanded<<endl;
	}
	else if ((type == 'a' && !type_set_user) || user_type == 'a'){
		cout<<"Forward planner A* search";
		if (admissible){
			cout<<" admissible"<<endl;
		}
		else{
			cout<<" inadmissible"<<endl;
		}
		astar(initial,goal, plan);
		cout<<"Nodes expanded: "<<astarnodesexpanded<<endl;
	}
	else if ((type == 'g' && !type_set_user) || user_type == 'g'){
		cout<<"Goal Stack planner"<<endl;
		goalstack(initial,goal, plan);
		cout<<"Maximum Goal Stack size "<<maxsizegoalstack<<endl;
	}

	cout<<"Plan length: "<<plan.size()<<endl;
	output_file<<plan.size()<<endl;
	for(list<action>::iterator it = plan.begin(); it != plan.end() ; ++it){
		(*it).print(output_file);
		(*it).print();
	}
	cout<<"-------------------"<<endl;
	output_file.close();

	return 0;
}

void help(){	
		cout<<"Usage ./planner input_file output_file [Optional parameters]"<<endl;
		cout<<endl;
		cout<<"Optional parameters"<<endl;
		cout<<"-h, --help: Help"<<endl;
		cout<<"-i, --inadmissible :  Use inadmissible heuristic"<<endl;
		cout<<"-p, --print : Prints initial and goal state"<<endl;
		cout<<"-f, --bfs : Force to use bfs"<<endl;
		cout<<"-a, --astar : Force to use astar"<<endl;
		cout<<"-g, --goalstack : Force to use goalstack"<<endl;
}

bool bfs(const state& initial, const state& goal, list<action>& plan){

	if (initial.goalTest(goal)){
		return true;
	}

	list<node*> frontier;
	set<state*, statepointer> frontier_set;
	set<state*, statepointer> explored;

	node* current_node = new node();
	current_node->curr = new state();
	*(current_node->curr) = initial;
	current_node->leads = NULL;
	current_node->parent = NULL;
	current_node->path_cost = 0;
	// int level =-1;

	frontier.push_back(current_node);
	frontier_set.insert(current_node->curr);

	vector<state> successor;
	vector<action> action_list;

	while (!frontier.empty()){
		bfsnodesexpanded++;
		current_node = frontier.front(); frontier.pop_front();
		// if (current_node->path_cost != level){
		// 	level = current_node->path_cost;
		// 	cout<<"At level "<<level<<endl;
		// } 
		frontier_set.erase(current_node->curr);
		explored.insert(current_node->curr);
		
		action_list.clear();
		successor.clear();
		(current_node->curr)->generateSuccessor(action_list, successor);
		
		for (int i=0; i< successor.size(); i++){
			if (frontier_set.find(&successor[i]) == frontier_set.end() && explored.find(&successor[i]) == explored.end()){
				if (successor[i].goalTest(goal)){
					plan.push_front(action_list[i]);

					while (current_node->parent != NULL){
						plan.push_front(*(current_node->leads));
						current_node = current_node->parent;
					}
					return true;
				}
				node* new_node = new node();
				new_node->curr = new state();
				new_node->leads = new action();
				*(new_node->curr) = initial;
				*(new_node->curr) = successor[i];
				*(new_node->leads) = action_list[i];
				new_node->path_cost = current_node->path_cost + 1;
				new_node->parent = current_node;

				frontier.push_back(new_node);
				frontier_set.insert(new_node->curr);
			}

		}


	}
	return false;
}

int heuristic(const state& s, const state& goal){

	// Number of steps to move blocks in right position
	int val = 0;
	bool pool[N+1];
	for (int i=0; i< N+1; i++){
		pool[i] = false;
	}
	for (set<int>::iterator it = s.hold.begin(); it != s.hold.end(); ++it){
		pool[*it] = true;
	}
	for(set<int>::iterator it=goal.ontable.begin(); it != goal.ontable.end(); ++it){
		if (!pool[*it] && s.ontable.find(*it) == s.ontable.end()){
			
			int block_x = (*it);
			while (!pool[block_x]){
				pool[block_x] = true;
				val++;
				for (set<pair<int,int> >::iterator on_x = s.on.begin(); on_x != s.on.end(); ++on_x){
					if ((*on_x).second == block_x){
						
						block_x = (*on_x).first;
						break;
					}
				}
			}
		}
	}

	for(set<pair<int,int> >::iterator it=goal.on.begin(); it != goal.on.end(); ++it){
		if (!(pool[(*it).first] && pool[(*it).second]) && s.on.find(*it) == s.on.end()){
			int block_x = (*it).first;
			while(!pool[block_x]){
				pool[block_x] = true;
				val++;
				for (set<pair<int,int> >::iterator on_x = s.on.begin(); on_x != s.on.end(); ++on_x){
					if ((*on_x).second == block_x){
						
						block_x = (*on_x).first;
						break;
					}
				}
			}
			int block_y = (*it).second;
			if (s.clearblock.find(block_y) != s.clearblock.end()){
			}
			else {
				for (set<pair<int,int> >::iterator on_y = s.on.begin(); on_y != s.on.end(); ++on_y){
					if ((*on_y).second == block_y){
						
						block_y = (*on_y).first;
						break;
					}
				}
				while (!pool[block_y]){
					pool[block_y] = true;
					val++;
					
					for (set<pair<int,int> >::iterator on_y2 = s.on.begin(); on_y2 != s.on.end(); ++on_y2){
						if ((*on_y2).second == block_y){
							block_y = (*on_y2).first;
							break;
						}
					}

				}
			}
		}
	}
	for(set<int>::iterator it=goal.clearblock.begin(); it != goal.clearblock.end(); ++it){
		if (!pool[*it] && s.clearblock.find(*it) == s.clearblock.end()){
			int block_y = *it;
			for (set<pair<int,int> >::iterator on_y = s.on.begin(); on_y != s.on.end(); ++on_y){
				if ((*on_y).second == block_y){
					
					block_y = (*on_y).first;
					break;
				}
			}
			while (!pool[block_y]){
				pool[block_y] = true;
				val++;
				
				for (set<pair<int,int> >::iterator on_y2 = s.on.begin(); on_y2 != s.on.end(); ++on_y2){
					if ((*on_y2).second == block_y){
						block_y = (*on_y2).first;
						break;
					}
				}

			}
		}
	}
	for(set<int>::iterator it=goal.hold.begin(); it != goal.hold.end(); ++it){
		if (!pool[*it] && s.hold.find(*it) == s.hold.end()){
			int block_y = *it;
			while (!pool[block_y]){
				pool[block_y] = true;
				val++;
				
				for (set<pair<int,int> >::iterator on_y2 = s.on.begin(); on_y2 != s.on.end(); ++on_y2){
					if ((*on_y2).second == block_y){
						block_y = (*on_y2).first;
						break;
					}
				}

			}
		}
	}


	// Cost of stacking blocks in pool except the already hold block
	for (int i=1; i<=N;i++){
		if (pool[i] && goal.hold.find(i) == goal.hold.end()){
			if (admissible){
				val+=1;
			}
			else {
				if (goal.ontable.find(i) != goal.ontable.end()){
					val+=1;
				}
				else {
					val+=3;
				}
			}
		}
	}
	return val;
}

bool astar(const state& initial, const state& goal, list<action>& plan){

	priority_queue<pair<node*, int>, vector<pair<node*, int> >, Comparepriority> frontier;
	set<state*, statepointer> explored;

	node* current_node = new node();
	current_node->curr = new state();
	*(current_node->curr) = initial;
	current_node->leads = NULL; 
	current_node->parent = NULL;
	current_node->path_cost = 0;

	frontier.push(make_pair<node*, int>(current_node,current_node->path_cost+heuristic(*(current_node->curr),goal)));

	vector<state> successor;
	vector<action> action_list;

	while(!frontier.empty()){

		current_node = frontier.top().first; frontier.pop();
		if (explored.find(current_node->curr) == explored.end()){
			astarnodesexpanded++;
			if ((current_node->curr)->goalTest(goal)){

				while (current_node->parent != NULL){
					plan.push_front(*(current_node->leads));
					current_node = current_node->parent;
				}
				return true;
			}
			explored.insert(current_node->curr);
		
			action_list.clear();
			successor.clear();
			(current_node->curr)->generateSuccessor(action_list, successor);
			
			for (int i=0; i< successor.size(); i++){
				if (explored.find(&successor[i]) == explored.end()){
					
					node* new_node = new node();
					new_node->curr = new state();
					new_node->leads = new action();
					*(new_node->curr) = successor[i];
					*(new_node->leads) = action_list[i];
					new_node->path_cost = current_node->path_cost + 1;
					new_node->parent = current_node;

					frontier.push(make_pair<node*, int>(new_node,new_node->path_cost+heuristic(*(new_node->curr),goal)));
				}

			}

		}
		else{
			delete current_node->curr;
			delete current_node->leads;
			delete current_node;
		}
		

	}
	return false;
}

bool goalstack(const state& initial, const state& goal, list<action>& plan){

	stack<element> gstack;

	state current;
	current = initial;

	element e;
	e.what = 1;
	e.conjunct = new state();
	*(e.conjunct) = goal;
	gstack.push(e);

	while(!gstack.empty()){
		if (gstack.size() > maxsizegoalstack){
			maxsizegoalstack = gstack.size();
		}
		e = gstack.top();
		
		if (e.what == 1){
			//Conjunct Goal
			bool satifised = true;
			
			// Pushing only unsatified subgoals
			if ((e.conjunct)->armempty && current.armempty== false){
				satifised = false;
				element new_e;
				new_e.what = 7;
				new_e.armempty = true;
				gstack.push(new_e);
			}
			for(set<int>::iterator it=(e.conjunct)->hold.begin(); it != (e.conjunct)->hold.end(); ++it){
				if (current.hold.find(*it) == current.hold.end()){
					satifised = false;
					element new_e;
					new_e.what = 6;
					new_e.hold = (*it);
					gstack.push(new_e);
				}
			}
			for(set<int>::iterator it=(e.conjunct)->clearblock.begin(); it != (e.conjunct)->clearblock.end(); ++it){
				if (current.clearblock.find(*it) == current.clearblock.end()){
					satifised = false;
					element new_e;
					new_e.what = 5;
					new_e.clearblock = (*it);
					gstack.push(new_e);
				}
			}

			list<pair<int,int> > on_subgoals;
			for(set<pair<int,int> >::iterator it=(e.conjunct)->on.begin(); it != (e.conjunct)->on.end(); ++it){
				if (current.on.find(*it) == current.on.end()){
					satifised = false;
					list<pair<int,int> >::iterator it2 = on_subgoals.begin();
					for (; it2 != on_subgoals.end(); ++it2){
						if ((*it).second == (*it2).first){
							break;
						}
					}
					on_subgoals.insert(it2, (*it));
				}
			}
			for (list<pair<int,int> >::iterator it = on_subgoals.begin(); it != on_subgoals.end(); ++it){
				element new_e;
				new_e.what = 3;
				new_e.on = (*it);
				gstack.push(new_e);
			}

			for(set<int>::iterator it=(e.conjunct)->ontable.begin(); it != (e.conjunct)->ontable.end(); ++it){
				if (current.ontable.find(*it) == current.ontable.end()){
					satifised = false;
					element new_e;
					new_e.what = 4;
					new_e.ontable = (*it);
					gstack.push(new_e);
				}
			}

			if (satifised){
				delete (e.conjunct);
				gstack.pop();
			}


		}
		else if (e.what == 2){
			// Action
			plan.push_back(*(e.a));
			gstack.pop();
			/*action(pick block)
			preconditions – (ontable block) (clear block) (empty)
			effects – (hold block) ~(clear block) ~(empty) ~(ontable block)*/
			if ((e.a)->name==PICK){
					current.hold.insert((e.a)->blocks[0]);

					current.clearblock.erase((e.a)->blocks[0]);
					current.armempty = false;
					current.ontable.erase((e.a)->blocks[0]);
			}

			/*action(unstack blocka blockb)
			preconditions – (on blocka blockb) (clear blocka) (empty)
			effects – (hold blocka) clear(blockb) ~(on blocka blockb) ~(empty) ~(clear blocka)*/
			else if ((e.a)->name==UNSTACK){
					current.hold.insert((e.a)->blocks[0]);
					current.clearblock.insert((e.a)->blocks[1]);


					current.on.erase(make_pair<int,int>((e.a)->blocks[0],(e.a)->blocks[1]));
					current.armempty = false;
					current.clearblock.erase((e.a)->blocks[0]);
			}
	

			/*action(release block)
			preconditions – (hold block)
			effects – (ontable block) (clear block) (empty) ~(hold block)*/
			else if ((e.a)->name==RELEASE){
					current.ontable.insert((e.a)->blocks[0]);
					current.clearblock.insert((e.a)->blocks[0]);
					current.armempty = true;

					current.hold.erase((e.a)->blocks[0]);
			}

	
			/*action(stack blocka blockb)
			preconditions – clear(blockb) (hold blocka)
			effects – (on blocka blockb) (clear blocka) (empty) ~(hold blocka) ~(clear blockb)*/
			else if ((e.a)->name==STACK){
					current.on.insert(make_pair<int,int>((e.a)->blocks[0],(e.a)->blocks[1]));
					current.clearblock.insert((e.a)->blocks[0]);
					current.armempty = true;

					current.hold.erase((e.a)->blocks[0]);
					current.clearblock.erase((e.a)->blocks[1]);
			}

			delete (e.a);

		}
		else if (e.what == 3){
			//On A B
			gstack.pop();
			if (current.on.find(e.on) == current.on.end()){

				//use stack action
				/*action(stack blocka blockb)
				preconditions – clear(blockb) (hold blocka)
				effects – (on blocka blockb) (clear blocka) (empty) ~(hold blocka) ~(clear blockb)*/

				element new_e;
				new_e.what = 2;
				new_e.a = new action();

				vector<int> temp;
				temp.push_back(e.on.first);
				temp.push_back(e.on.second);
				(new_e.a)->name = STACK;
				(new_e.a)->blocks = temp;

				gstack.push(new_e);

				element pre_conj;
				pre_conj.what = 1;
				pre_conj.conjunct = new state();
				(pre_conj.conjunct)->clearblock.insert(e.on.second);
				(pre_conj.conjunct)->hold.insert(e.on.first);

				gstack.push(pre_conj);

			}

		}
		else if (e.what == 4){
			//On table A
			gstack.pop();

			if (current.ontable.find(e.ontable) == current.ontable.end()){
				//Use release action
				/*action(release block)
				preconditions – (hold block)
				effects – (ontable block) (clear block) (empty) ~(hold block)*/

				element new_e;
				new_e.what = 2;
				new_e.a = new action();

				vector<int> temp;
				temp.push_back(e.ontable);
				(new_e.a)->name = RELEASE;
				(new_e.a)->blocks = temp;

				gstack.push(new_e);

				element pre_conj;
				pre_conj.what = 1;
				pre_conj.conjunct = new state();
				(pre_conj.conjunct)->hold.insert(e.ontable);

				gstack.push(pre_conj);

			}

		}
		else if (e.what == 5){
			//Clear A

			gstack.pop();
			if (current.clearblock.find(e.clearblock) == current.clearblock.end()){
				//Possible actions - Unstack X A, Release A, Stack A X
				if (current.hold.find(e.clearblock) != current.hold.end()){
					// Action can be release/ stack choosing release as we are holding that block and we want to it be clear
					/*action(release block)
					preconditions – (hold block)
					effects – (ontable block) (clear block) (empty) ~(hold block)*/

					element new_e;
					new_e.what = 2;
					new_e.a = new action();

					vector<int> temp;
					temp.push_back(e.clearblock);
					(new_e.a)->name = RELEASE;
					(new_e.a)->blocks = temp;

					gstack.push(new_e);

					element pre_conj;
					pre_conj.what = 1;
					pre_conj.conjunct = new state();
					(pre_conj.conjunct)->hold.insert(e.clearblock);

					gstack.push(pre_conj);

				}
				else {
					int block_x = 0;
					for (set<pair<int,int> >::iterator it = current.on.begin(); it != current.on.end(); ++it){
						if ((*it).second == e.clearblock){
							block_x = (*it).first;
							break;
						}
					}
					if (block_x != 0){
						// Action is unstack X A
						/*action(unstack blocka blockb)
						preconditions – (on blocka blockb) (clear blocka) (empty)
						effects – (hold blocka) clear(blockb) ~(on blocka blockb) ~(empty) ~(clear blocka)*/
						element new_e;
						new_e.what = 2;
						new_e.a = new action();

						vector<int> temp;
						temp.push_back(block_x);
						temp.push_back(e.clearblock);
						(new_e.a)->name = UNSTACK;
						(new_e.a)->blocks = temp;
						gstack.push(new_e);

						element pre_conj;
						pre_conj.what = 1;
						pre_conj.conjunct = new state();
						(pre_conj.conjunct)->on.insert(make_pair<int,int>(block_x,e.clearblock));
						(pre_conj.conjunct)->clearblock.insert(block_x);
						(pre_conj.conjunct)->armempty = true;
						gstack.push(pre_conj);

					}
					else{
						//Error since block A is not hold nor any block is above it
					}
				}
			}

		}
		else if (e.what == 6){
			// Hold A
			gstack.pop();
			if (current.hold.find(e.hold) == current.hold.end()){
				// Two actions possible - Pick and Unstack
				if (current.ontable.find(e.hold) != current.ontable.end()){
					//Using Pick
					/*action(pick block)
					preconditions – (ontable block) (clear block) (empty)
					effects – (hold block) ~(clear block) ~(empty) ~(ontable block)*/

					element new_e;
					new_e.what = 2;
					new_e.a = new action();

					vector<int> temp;
					temp.push_back(e.hold);
					(new_e.a)->name = PICK;
					(new_e.a)->blocks = temp;

					gstack.push(new_e);

					element pre_conj;
					pre_conj.what = 1;
					pre_conj.conjunct = new state();
					(pre_conj.conjunct)->ontable.insert(e.hold);
					(pre_conj.conjunct)->clearblock.insert(e.hold);
					(pre_conj.conjunct)->armempty = true;
					gstack.push(pre_conj);
				}
				else {
					int block_x = 0;
					for (set<pair<int,int> >::iterator it = current.on.begin(); it != current.on.end(); ++it){
						if ((*it).first == e.hold){
							block_x = (*it).second;
							break;
						}
					}
					if (block_x != 0){
						// Action is unstack A X
						/*action(unstack blocka blockb)
						preconditions – (on blocka blockb) (clear blocka) (empty)
						effects – (hold blocka) clear(blockb) ~(on blocka blockb) ~(empty) ~(clear blocka)*/
						element new_e;
						new_e.what = 2;
						new_e.a = new action();

						vector<int> temp;
						temp.push_back(e.hold);
						temp.push_back(block_x);
						(new_e.a)->name = UNSTACK;
						(new_e.a)->blocks = temp;
						gstack.push(new_e);

						element pre_conj;
						pre_conj.what = 1;
						pre_conj.conjunct = new state();
						(pre_conj.conjunct)->on.insert(make_pair<int,int>(e.hold,block_x));
						(pre_conj.conjunct)->clearblock.insert(e.hold);
						(pre_conj.conjunct)->armempty = true;
						gstack.push(pre_conj);
					}
					else{
						// Error
					}

				}				

			}

		}
		else if (e.what == 7){
			// Empty
			gstack.pop();
			if (e.armempty && current.armempty == false){
				//Actions possible - release & stack
				//Choosing release action

				//First check current should be holding something 

				if (current.hold.size() == 1) {

					/*action(release block)
					preconditions – (hold block)
					effects – (ontable block) (clear block) (empty) ~(hold block)*/

					int block_to_release = *(current.hold.begin());

					element new_e;
					new_e.what = 2;
					new_e.a = new action();

					vector<int> temp;
					temp.push_back(block_to_release);
					(new_e.a)->name = RELEASE;
					(new_e.a)->blocks = temp;

					gstack.push(new_e);

					element pre_conj;
					pre_conj.what = 1;
					pre_conj.conjunct = new state();
					(pre_conj.conjunct)->hold.insert(block_to_release);

					gstack.push(pre_conj);

				}
				else{
					//Error Currently ~armempty but not holding anything
				}

			}
		}
		else{
			//Error;
			gstack.pop();
		}

	}


}

void state::print(ostream& output) const{
	
	for(set<int>::iterator it=ontable.begin(); it != ontable.end(); ++it){
		output<<"(ontable "<<(*it)<<") ";
	}
	for(set<int>::iterator it=clearblock.begin(); it != clearblock.end(); ++it){
		output<<"(clear "<<(*it)<<") ";
	}
	for(set<pair<int,int> >::iterator it=on.begin(); it != on.end(); ++it){
		output<<"(on "<<(*it).first<<" "<<(*it).second<<") ";
	}
	for(set<int>::iterator it=hold.begin(); it != hold.end(); ++it){
		output<<"(hold "<<(*it)<<") ";
	}
	if (armempty){
		output<<"(empty)";
	}
	output<<endl;
	
}

void state::printtable(ostream& output) const{
	
	int max_size = 0;
	map<int,int> depth;
	if (!hold.empty()){
		output<<"Holding : ";
	}
	for(set<int>::iterator it=hold.begin(); it != hold.end(); ++it){
		output<<setw(2)<<(*it)<<" ";
	}
	if (!hold.empty()){
		output<<endl;
	}
	for(set<int>::iterator it=clearblock.begin(); it != clearblock.end(); ++it){
		int relsize = 1;
		int block_x = (*it);
		
		while (ontable.find(block_x) == ontable.end()){
			
			for (set<pair<int,int> >::iterator on_x = on.begin(); on_x != on.end(); ++on_x){
				if ((*on_x).first == block_x){
					
					block_x = (*on_x).second;
					break;
				}
			}
			relsize++;
		}
		if (max_size < relsize){
			max_size = relsize;
		}
		depth[*it] = relsize;
	}
	for (int i=max_size; i>0; i--){
		for(set<int>::iterator it=clearblock.begin(); it != clearblock.end(); ++it){
			if (depth[*it] >= i){
				int block_x = *it;
				for (int j=0; j < depth[*it] - i; j++) {
					for (set<pair<int,int> >::iterator on_x = on.begin(); on_x != on.end(); ++on_x){
						if ((*on_x).first == block_x){
							
							block_x = (*on_x).second;
							break;
						}
					}
				}
				
				output<<"|"<<setw(2)<<block_x<<"| ";

			}
			else {
				output<<"     ";
			}
		}
		output<<endl;

	}
	for(set<int>::iterator it=clearblock.begin(); it != clearblock.end(); ++it){
		output<<"-----";
	}
	if (!clearblock.empty()){
		output<<endl;
	}
}


void state::generateSuccessor(vector<action>& action_list,vector<state>& successor){

	/*action(release block)
	preconditions – (hold block)
	effects – (ontable block) (clear block) (empty) ~(hold block)*/
	for (int i=1; i<=N; i++){
		if (hold.find(i)!=hold.end()){
			vector<int> temp;
			temp.push_back(i);
			action a;
			a.name = RELEASE;
			a.blocks = temp;
			action_list.push_back(a);

			state new_state = *this;
			new_state.ontable.insert(i);
			new_state.clearblock.insert(i);
			new_state.armempty = true;

			new_state.hold.erase(i);

			successor.push_back(new_state);
		}
	}

	
	/*action(stack blocka blockb)
	preconditions – clear(blockb) (hold blocka)
	effects – (on blocka blockb) (clear blocka) (empty) ~(hold blocka) ~(clear blockb)*/
	for (int i=1; i<=N; i++){
		for (int j=1; j<=N; j++){
			if (i != j){

				if (clearblock.find(j)!=clearblock.end() && hold.find(i)!=hold.end()){
					vector<int> temp;
					temp.push_back(i);
					temp.push_back(j);
					action a;
					a.name = STACK;
					a.blocks = temp;
					action_list.push_back(a);

					state new_state = *this;
					new_state.on.insert(make_pair<int,int>(i,j));
					new_state.clearblock.insert(i);
					new_state.armempty = true;

					new_state.clearblock.erase(j);
					new_state.hold.erase(i);

					successor.push_back(new_state);
				}
			}
		}
	}

	/*action(pick block)
	preconditions – (ontable block) (clear block) (empty)
	effects – (hold block) ~(clear block) ~(empty) ~(ontable block)*/
	for (int i=1; i<=N; i++){
		
		if (ontable.find(i)!=ontable.end() && clearblock.find(i)!=clearblock.end() && armempty){
			vector<int> temp;
			temp.push_back(i);
			action a;
			a.name = PICK;
			a.blocks = temp;
			action_list.push_back(a);

			state new_state = *this;
			new_state.hold.insert(i);

			new_state.clearblock.erase(i);
			new_state.ontable.erase(i);
			new_state.armempty = false;

			successor.push_back(new_state);
		}
	}

	/*action(unstack blocka blockb)
	preconditions – (on blocka blockb) (clear blocka) (empty)
	effects – (hold blocka) clear(blockb) ~(on blocka blockb) ~(empty) ~(clear blocka)*/
	for (int i=1; i<=N; i++){
		for (int j=1; j<=N; j++){
			if (i != j){

				if (on.find(make_pair<int,int>(i,j))!=on.end() && clearblock.find(i)!=clearblock.end() && armempty){
					vector<int> temp;
					temp.push_back(i);
					temp.push_back(j);
					action a;
					a.name = UNSTACK;
					a.blocks = temp;
					action_list.push_back(a);

					state new_state = *this;
					new_state.hold.insert(i);
					new_state.clearblock.insert(j);

					new_state.clearblock.erase(i);
					new_state.on.erase(make_pair<int,int>(i,j));
					new_state.armempty = false;

					successor.push_back(new_state);
				}
			}
		}
	}

}


bool state::goalTest(const state& testgoal) const {
	for(set<pair<int,int> >::iterator it=testgoal.on.begin(); it != testgoal.on.end(); ++it){
		if (on.find(*it) == on.end()){
			return false;
		}
	}
	for(set<int>::iterator it=testgoal.ontable.begin(); it != testgoal.ontable.end(); ++it){
		if (ontable.find(*it) == ontable.end()){
			return false;
		}
	}
	for(set<int>::iterator it=testgoal.clearblock.begin(); it != testgoal.clearblock.end(); ++it){
		if (clearblock.find(*it) == clearblock.end()){
			return false;
		}
	}
	for(set<int>::iterator it=testgoal.hold.begin(); it != testgoal.hold.end(); ++it){
		if (hold.find(*it) == hold.end()){
			return false;
		}
	}
	if (testgoal.armempty && armempty== false){
		return false;
	}
	return true;

}

bool state::checkState() const {
	if (armempty==true && hold.size() != 0){
		return false;
	}
	else if (armempty == false && hold.size() != 1){
		return false;
	}
	return true;
}


void action::print(ostream& output) const {
	
	if (name == PICK && blocks.size() == 1){
		output<<"action(pick "<<blocks[0]<<")"<<endl;
	}
	else if (name == UNSTACK && blocks.size() == 2){		
		output<<"action(unstack "<<blocks[0]<<" "<<blocks[1]<<")"<<endl;
	}
	else if (name == RELEASE && blocks.size() == 1){
		output<<"action(release "<<blocks[0]<<")"<<endl;
	}
	else if (name == STACK && blocks.size() == 2){
		output<<"action(stack "<<blocks[0]<<" "<<blocks[1]<<")"<<endl;
	}
}