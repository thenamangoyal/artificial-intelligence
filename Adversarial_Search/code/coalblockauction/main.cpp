#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;

class Bid;
class State;
class Problem;

clock_t begin;


int T; //Time
int N; //No of coal blocks
int B; //No of bids
int C; //No of companies

vector< vector<int> >  bidsforBlock; // Array storing bids corresponding to Blocks
vector<Bid> bids; // Array of bids
vector<int> Com; // Starting index for bids of company
vector<int> ncid; // Array of no of bids for company

class Bid{
private:
	int Bid_Id; // Bid ID
	int bidvalue; // Value of Bid
	int nbid; // No of blocks in bid
	int cid; // Company Id
	vector<int> blocks; // Array of blocks
public:
	Bid(){}
	~Bid(){}
	void initialise(int Id){
		/* Initialise bids*/

		Bid_Id=Id;
		cin>>cid>>nbid>>bidvalue;
		cid--;
		blocks= vector<int>(nbid);

		for (int i=0; i<nbid; i++){
			cin>>blocks[i];
			blocks[i]--;
			bidsforBlock[blocks[i]].push_back(Id);
		}

	}
	const vector<int>& getBlocks() const{
		return blocks;
	}
    int getCid() const{
        return cid;
    }
    int getValue() const{
        return bidvalue;
    }
};

class State{
private:
	vector<int> Validity; // Storing validity of bids 0= valid and <0 = inavlid
	vector<int> Allotment; // Storing allotment of bid for company
	int value;
public:
	State(vector<int> v_Validity = vector<int>(B,0), vector<int> v_Allotment = vector<int>(C,-1)): value(0), Validity(v_Validity), Allotment(v_Allotment){
        /* Constructor for state calculates the objective function value */
        for (int i=0; i<C; i++){
            if (Allotment[i] != -1){
                value+=bids[Allotment[i]].getValue();
            }
        }

	}
	~State() {}
	void generateSuccessors(vector<State>& Successors){

		vector<int> changedValidity;
		vector<int> Blocks;

        for(int i=0;i<C;i++){

        	changedValidity=Validity;

        	if (Allotment[i] != -1){            	
                
	           	Blocks = bids[Allotment[i]].getBlocks();

	            // Choose a bid to remove and make all other intersecting bids (having same blocks) Valid
	            for(vector<int>::iterator itBlock=Blocks.begin();itBlock!=Blocks.end(); ++itBlock ){
	                for(vector<int>::iterator itBid=bidsforBlock[*itBlock].begin();itBid!=bidsforBlock[*itBlock].end(); ++itBid )
	                    changedValidity[*itBid]++;
	            }
	        }

            for(int j=Com[i];j<B && (i==C-1 || j<Com[i+1]);j++){

                if(j!=Allotment[i] && changedValidity[j]==0){

                	// Only choose this allotment for company if it is valid
                    vector<int> newAllotment=Allotment;
                    newAllotment[i]=j;
                    vector<int> newValidity=changedValidity;
                    vector<int> blk = bids[j].getBlocks();

                    // Choose a bid to insert and make all other intersecting bids (having same blocks) Invalid
                    for(vector<int>::iterator itBlock=blk.begin();itBlock!=blk.end(); ++itBlock){
                        for(vector<int>::iterator itBid=bidsforBlock[*itBlock].begin();itBid!=bidsforBlock[*itBlock].end();++itBid )
                            newValidity[*itBid]--;
                    }

                    Successors.push_back(State(newValidity, newAllotment));
                }
            }
        }

	}
	int getValue() const{
		return value;
	}
	void print() const{

		/* Print current state of allotment */

		cout<<value;
		for (int i=0; i<C; i++){
			if (Allotment[i] != -1){
				cout<<" "<<Allotment[i]+1;
			}
		}
		cout<<endl;

	}

};


class Problem{
private:
	State currentState; // Current State

	
	int HillClimb(){

		/* One Hill Climb solution i.e. take  current state and make it reach local maxima*/

		int current_value = currentState.getValue();
		vector<State> Successors;
		vector<State> betterSuccessors;
		vector<int> Cummlative;

		while((clock()-begin)/CLOCKS_PER_SEC < T*60){
			Successors.clear(); // Array of generated succesors
			Cummlative.clear(); // Array of Cummulative sum of successor states values better than current State

			currentState.generateSuccessors(Successors); // Generate succesors

			if (Successors.empty()){
				break; // No successor generated return current solution
			}
			
			
			Cummlative = vector<int>(Successors.size());
			int value;
			int sum=0; // Stores sum of difference between successors having better value than current state

			/* Iterate over all successor states to get those having objective function value better than current state */
			for (int i=0; i<Successors.size(); i++){
				value = Successors[i].getValue();

				if (value > current_value){
					sum += value - current_value;
					Cummlative[i] = sum;
				}
				else {
					Cummlative[i] = -1;
				}

			}

			if(sum==0){
				break; // Found no value in succesor better than current value i.e. local maxima
			}

			/* Randomaly choosing a successor state having having greator than current state i.e. it may not be the steepest ascent */
			int i;
			int random_number = rand()%sum; 
			for (i=0; i<Cummlative.size(); i++){
				if (Cummlative[i]>random_number){
					break;
				}
			}
			currentState = Successors[i];
			current_value = Successors[i].getValue();

		}
		return current_value;

	}

public:
	Problem(): currentState() {

		/* Initialize the problem */

		bids = vector<Bid>(B);
		ncid = vector<int>(C);
		Com = vector<int>(C);
		bidsforBlock = vector< vector<int> >(N);

		int temp_cid;
		int bid_counter=0;

		for (int i=0; i< C; i++){

			Com[i]=bid_counter;
			cin>>temp_cid>>ncid[i];

			// Scan all bids for a company
			for (int j=0; j<ncid[i]; j++){
				bids[bid_counter].initialise(bid_counter);
				bid_counter++;
			}
		}
	}
	~Problem() {}
	const State& solution(){

		/* Random restarts hill climbing solver*/

		State globalMaximum = currentState;
		int globalMaximumValue = currentState.getValue();
		int value;

		while((clock()-begin)/CLOCKS_PER_SEC < T*60){

         	restart(); // restart form a new state
			value = HillClimb(); // make that sate maixma

			if (value > globalMaximumValue){
				// check if that state is better than current globalMaxima
				globalMaximum = currentState; 
				globalMaximumValue = value;
			}
		}

		currentState = globalMaximum;
		return currentState;
	}

	void restart(){
		/* Generate a new state randomly from current state*/

	    vector<int> Validity(B,0);
		vector< set<int> > ValidBidsforComp(C); // Stores valid bid remaing for a company
		vector<int> Allotment(C); 

		for (int i=0; i<C; i++){
			for (int j=0; j< ncid[i]; j++){
				ValidBidsforComp[i].insert(Com[i]+j);
			}
		}

		int value;
		int comptoselect = rand()%C; // Select one particluar company		
		int i;
		for(int k=0;k<C;k++){
			i = (k+comptoselect)%C;

			//Randomly allot a valid bid for a company
			value = rand()%(ValidBidsforComp[i].size()+1) -1;

			if (value == -1){
				// Allotted a null bid for a company 
				Allotment[i] =-1;
			}
			else {

				// Find that bid
				set<int>::iterator it;
				for (it=ValidBidsforComp[i].begin(); it!=ValidBidsforComp[i].end(); ++it){
					if(value==0){
						break;
					}
					value--;
				}

				// Insert that bid in allotment
				Allotment[i] = *it;

				// Find all blocks corresponding to that bid
				vector<int> Blocks = bids[*it].getBlocks();

                for(vector<int>::iterator itBlock=Blocks.begin();itBlock!=Blocks.end(); ++itBlock){

                	// Find all bids corrsponding to taht block
                    for(vector<int>::iterator itBid=bidsforBlock[*itBlock].begin();itBid!=bidsforBlock[*itBlock].end();++itBid ){
                    	
                    	// Make those bids invalid
                    	Validity[*itBid]--;
						ValidBidsforComp[bids[*itBid].getCid()].erase(*itBid);

					}
				}
			}
		}
		currentState = State(Validity, Allotment);
	}
	const State& getCurrentState(){
		return currentState;
	}
};


int main(){
	srand(time(NULL));
	begin = clock();
	cin>>T>>N>>B>>C;
	Problem p;
	p.solution().print();
	//cout<<"Total Time Taken: "<<(clock()-begin)/CLOCKS_PER_SEC<<" sec"<<endl;

	return 0;
}
