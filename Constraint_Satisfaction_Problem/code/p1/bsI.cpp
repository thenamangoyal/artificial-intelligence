#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <set>
#include <list>
#include <cstdlib>
#include <ctime>

#define N 9
#define N_sqrt 3
using namespace std;

//Assignment 2d Matrix of int stores 1 to 9 if valid else store 0 if unassigned

bool solve(int (&assignment)[N][N], long long int& no_backtrack);
bool backtrack(int (&assignment)[N][N], list<int> (&domain)[N][N], long long int& no_backtrack);
bool findunassigned(int (&assignment)[N][N], list<int> (&domain)[N][N], int& row, int &col);
bool checkconsistent (int var_i, int var_j, int value, int (&assignment)[N][N]);
void print(int (&assignment)[N][N], char* outputFileName =  NULL);

int main(int argc, char* argv[]){

	if (argc< 2){
		cout<<"Please specify input file and output file name"<<endl;
		return 1;
	}
	if (argc< 3){
		cout<<"Please specify output file name"<<endl;
		return 1;
	}

	ifstream input_file;
	input_file.open(argv[1],ios::in);

	if (input_file.fail()) {
		cout<<"Invalid Input file"<<endl;
		return 1;
	}

	ofstream test_output_file;
	test_output_file.open(argv[2], ios::out | ios::trunc);

	if (test_output_file.fail()) {
		cout<<"Error opening Output file"<<endl;
		return 1;
	}
	else {
		test_output_file.close();
	}

	string line;
	int problem[N][N];
	int parse;

	double max_time;
	double min_time;
	double total_time=0;
	int count=0;

	long long int max_no_backtrack;
	long long int min_no_backtrack;
	long long int total_no_backtrack=0;

	ofstream output_file;
	bool sol;

	while(input_file>>line){
		if (line.size() == N*N){
			parse = 0;
			char in_char;
			for (int i=0; i<N; i++){
				for (int j=0; j<N; j++){
					in_char = line[parse++];
					if (in_char >= '0' && in_char <= '9'){
						problem[i][j] = in_char - '0';
					}
					else{
						problem[i][j] = 0;
					}		
				}
			}

			//Print problem to cout
			cout<<"Solving"<<endl;
			print(problem);

			long long int no_backtrack =0;

			clock_t start = clock();
			sol = solve(problem, no_backtrack);			
			clock_t end = clock();

			if (sol){
				//Print solution to file
				print(problem, argv[2]);				

				//Print solution to cout
				cout<<"Solution"<<endl;
				print(problem);
			}
			else{
				//Print unsat to output
				output_file.open(argv[2], ios::out | ios::app);
				output_file<<"UNSAT"<<endl;
				output_file.close();
				
				//Print unsat to cout
				cout<<"UNSAT"<<endl;
			}
			double time = double (end-start)/ CLOCKS_PER_SEC;

			if (count == 0 || time > max_time){
				max_time = time;
			}
			total_time += time;
			if (count == 0 || time < min_time){
				min_time =  time;
			}
			cout<<"Running time: "<<time<<endl;

			if (count == 0 || no_backtrack > max_no_backtrack){
				max_no_backtrack = no_backtrack;
			}
			total_no_backtrack += no_backtrack;
			if (count == 0 || no_backtrack < min_no_backtrack){
				min_no_backtrack =  no_backtrack;
			}
			cout<<"No of backtrack: "<<no_backtrack<<endl;
			cout<<endl;

			count++;
		}
		
	}
	cout<<"-------"<<endl;
	cout<<"Maximum Running time: "<<max_time<<endl;
	cout<<"Average Running time: "<<(total_time/count)<<endl;
	cout<<"Mininum Running time: "<<min_time<<endl;
	cout<<"Total   Running time: "<<total_time<<endl;	

	cout<<"-------"<<endl;
	cout<<"Maximum No of backtrack: "<<max_no_backtrack<<endl;
	cout<<"Average No of backtrack: "<<(total_no_backtrack/count)<<endl;
	cout<<"Mininum No of backtrack: "<<min_no_backtrack<<endl;
	cout<<"Total   No of backtrack: "<<total_no_backtrack<<endl;
	return 0;
}

void print(int (&assignment)[N][N], char* outputFileName){
	if (outputFileName){
		ofstream output;
		output.open(outputFileName, ios::out | ios::app);
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				if (assignment[i][j] != 0){
					output<<assignment[i][j];
				}
				else{
					output<<".";
				}
			}
		}
		output<<endl;
		output.close();
	}
	else{
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				if (assignment[i][j] != 0){
					cout<<assignment[i][j];
				}
				else{
					cout<<".";
				}
			}
		}
		cout<<endl;
	}
	
}

bool solve(int (&problem)[N][N], long long int& no_backtrack){

	list<int> domain[N][N];

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			for (int k=1; k<=N; k++){
				domain[i][j].push_back(k); 
			}
		}
	}
	return backtrack(problem, domain, no_backtrack);
}

bool backtrack(int (&assignment)[N][N], list<int> (&domain)[N][N], long long int& no_backtrack){
	//Assignment is complete
	int row, col;
	if (!findunassigned(assignment, domain, row, col)){
		return true;
	}

	// Select unassigned variable
	
	for (list<int>::iterator it = domain[row][col].begin(); it != domain[row][col].end(); ++it){
		int value = *it;
		if (checkconsistent(row, col, value, assignment)){
			
			// Add to assignment
			assignment[row][col] = value;			

			if (backtrack(assignment, domain, no_backtrack)){
				return true;
			}

			// Remove from assignment
			assignment[row][col] = 0;			
			no_backtrack++;
		}
	}
	return false;
}

bool findunassigned(int (&assignment)[N][N], list<int> (&domain)[N][N], int& row, int &col){
	bool found = false;
	int min_legal_values = N+1;
	int legal_values;
	for (int i = 0; i < N; i++){
		for (int j =0; j< N; j++){
			if (assignment[i][j] == 0){
				found = true;
				legal_values = 0;
				for (list<int>::iterator value_it = domain[i][j].begin(); value_it != domain[i][j].end();++value_it){
					if (checkconsistent(i, j, *value_it, assignment)){
						legal_values++;
					}
				}
				if (legal_values < min_legal_values){
					row = i;
					col = j;
					min_legal_values = legal_values;
				}
				if (legal_values == 0){
					break;
				}
			}
		}
	}
	return found;
}


bool checkconsistent (int var_i, int var_j, int value, int (&assignment)[N][N]){
	//Column contraint
	for (int i=0; i<N; i++){
		if (value == assignment[i][var_j]){
			return false;
		}
	}

	//Row contraint
	for (int j=0; j<N; j++){
		if (value == assignment[var_i][j]){
			return false;
		}
	}

	//Box contraint
	int start_i = var_i - var_i%N_sqrt;
	int start_j = var_j - var_j%N_sqrt;


	for (int m= 0; m < N_sqrt; m++){
		for (int n= 0; n < N_sqrt; n++){
			if (value == assignment[start_i + m][start_j + n]){
				return false;
			}
		}
	}
	return true;
}