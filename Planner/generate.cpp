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

class state;

int N; // No of blocks

void help();
state generateRandomState();

class state{
public:
	set<pair<int,int> >  on;
	set<int> ontable;
	set<int> clearblock;
	set<int> hold;
	bool armempty;
public:
	state(): armempty(false) {}
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

int main(int argc, char *argv[])
{
	
	for (int i=1; i< argc; i++){
		string token = string(argv[i]);
		if (token == "--help" || token == "-h"){
			help();
			return 1;
		}
	}

	if (argc< 4){
		cout<<"Please specify output file and No of blocks and Planner"<<endl;
		help();
		return 1;
	}
	srand(time (NULL));
	
	N = atoi(argv[2]);

	if (N <= 0){
		cout<<"No of blocks should be greator than 0"<<endl;
		help();
		return 1;
	}

	ofstream output_file;
	output_file.open(argv[1], ios::out | ios::trunc);
	if (output_file.fail()) {
		cout<<"Invalid Output file"<<endl;
		help();
		return 1;
	}

	char type;

	if (string(argv[3]) == "f"){
		type = 'f';
	}
	else if (string(argv[3]) == "a"){
		type = 'a';
	}
	else if (string(argv[3]) == "g"){
		type = 'g';
	}
	else {
		cout<<"Invalid Type of Planner"<<endl;
		help();
		return 1;
	}

	state initial = generateRandomState();
	state goal = generateRandomState();

	output_file<<N<<endl;
	output_file<<type<<endl;
	output_file<<"initial"<<endl;
	initial.print(output_file);
	output_file<<"goal"<<endl;
	goal.print(output_file);
	
	output_file.close();

	cout<<"Generated Initial State"<<endl;
	initial.printtable();
	cout<<"Generated Goal State"<<endl;
	goal.printtable();

	return 0;
}

void help(){	
		cout<<"Usage ./generate output_file no_of_blocks planner(f|a|g)"<<endl;
		cout<<endl;
		cout<<"Optional parameters"<<endl;
		cout<<"-h, --help: Help"<<endl;
}

state generateRandomState(){
	state randstate;
	randstate.armempty = true;
	set<int> done;
	int block_x;
	for (int i = 1; i<= N; i++){
		block_x = rand() % N +1;
		while (done.find(block_x) != done.end()){
			block_x = rand() % N +1;
		}
		done.insert(block_x);
		int prob = rand() % N +1;
		bool onAnotherBlock = true;
		if (prob < (int)pow(N, 0.333333) + 1){
			onAnotherBlock = false;
		}
		if (onAnotherBlock && randstate.ontable.size() > 0){
			int pos = rand() % randstate.clearblock.size();
			set<int>::iterator it = randstate.clearblock.begin();
			for (int j=0; j<pos; j++){
				++it;
			}
			int block_y = *it;
			randstate.clearblock.erase(block_y);
			randstate.on.insert(make_pair<int,int>(block_x,block_y));
			randstate.clearblock.insert(block_x);

		} else {
			randstate.ontable.insert(block_x);
			randstate.clearblock.insert(block_x);
		}

	}
	return randstate;
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

bool state::checkState() const {
	if (armempty==true && hold.size() != 0){
		return false;
	}
	else if (armempty == false && hold.size() != 1){
		return false;
	}
	return true;
}
