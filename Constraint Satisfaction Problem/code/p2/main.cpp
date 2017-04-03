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

bool solve(int (&assignment)[N][N], int generic_cnf_no);
void print(int (&assignment)[N][N], char* outputFileName =  NULL);
int generateGenericSAT();

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

	int generic_cnf_no = generateGenericSAT();

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

			sol = solve(problem, generic_cnf_no);

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
			cout<<endl;
		}
		
	}
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

int generateGenericSAT(){

	ofstream output;
	output.open("process/generic_cnf_list.txt", ios::out | ios::trunc);
	int cnf_no=0;

	// Atleast one value from 1 to 9 in each entry
	for (int i=0; i< N; i++){
		for (int j=0; j< N; j++){
			for (int k=1; k<=N; k++){
				output<<i<<j<<k<<" ";
			}
			output<<"0"<<endl;
			cnf_no++;
		}
	}

	//Atmost one number form 1 to 9 in each row
	for (int i=0; i< N; i++){
		for (int k=1; k<=N; k++){
			for (int j=0; j< N; j++){
				for (int l=j+1; l<N; l++){
					output<<"-"<<i<<j<<k<<" "<<"-"<<i<<l<<k<<" "<<"0"<<endl;
					cnf_no++;
				}
			}
		}
	}

	//Atmost one number form 1 to 9 in each column
	for (int j=0; j< N; j++){
		for (int k=1; k<=N; k++){
			for (int i=0; i< N; i++){
				for (int l=i+1; l<N; l++){
					output<<"-"<<i<<j<<k<<" "<<"-"<<l<<j<<k<<" "<<"0"<<endl;
					cnf_no++;
				}
			}
		}
	}
	
	//Atmost one number form 1 to 9 in each box
	for (int a=0; a< N_sqrt; a++){
		for (int b=0; b< N_sqrt; b++){
			for(int k=1; k<=N; k++){

				for(int i=N_sqrt*a; i< N_sqrt*a+N_sqrt; i++){
					for(int j=N_sqrt*b; j<N_sqrt*b+N_sqrt; j++){

						for(int n=j+1; n<N_sqrt*a+N_sqrt; n++){
							output<<"-"<<i<<j<<k<<" "<<"-"<<i<<n<<k<<" "<<"0"<<endl;
							cnf_no++;
						}
						for(int m=i+1; m<N_sqrt*a+N_sqrt; m++){
							for(int n=N_sqrt*b; n<N_sqrt*b+N_sqrt; n++){
								output<<"-"<<i<<j<<k<<" "<<"-"<<m<<n<<k<<" "<<"0"<<endl;
								cnf_no++;
							}
						}
					}
				}
			}
		}
	}

	output.close();
	return cnf_no;
}


bool solve(int (&assignment)[N][N], int generic_cnf_no){

	ofstream output;
	output.open("process/cnf.txt", ios::out | ios::trunc);

	int cnf_no=generic_cnf_no;

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if (assignment[i][j] != 0){
				cnf_no++;
			}
		}
	}

	output<<"p cnf "<<100*(N-1)+ 10*(N-1) +N<<" "<<cnf_no<<endl;

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if (assignment[i][j] != 0){
				output<<i<<j<<assignment[i][j]<<" "<<"0"<<endl;
			}
		}
	}

	ifstream input_cnf;
	input_cnf.open("process/generic_cnf_list.txt", ios::in);

	char c;
	while (input_cnf.get(c)){
		output<<c;
	}

	input_cnf.close();

	output.close();


	system("./minisat process/cnf.txt process/sat_out.txt > process/sat_info.txt");

	ifstream input;
	input.open("process/sat_out.txt", ios::in);

	string top;
	input>>top;
	bool solved = false;
	if (top == "SAT"){
		solved = true;
		int literal;
		while(input>>literal){
			if (literal>0){
				// All true
				int value = literal%10;
				int i = (literal/100)%10;
				int j = (literal/10)%10;
				if (value>0 && value <= N && i < N && j < N){
					// Valid literal
					assignment[i][j] = value;
				}
			}
		}
	}
	else{
		solved = false;
	}
	input.close();

	return solved;
}