//Chris Jackson & Jan Lao
//ME 493 
//Final Project
//May 11, 2017

//MultiObjective Optimization
//Time and Exploration

#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

////////////////////////////////////////////////////////
//Agent
class Agent{
public:
	double x_loc;
	double y_loc;
	double x_loc_initial;
	double y_loc_initial;

	void init();
	void Simulate();
	void Reset();
};

void Agent::init(){
	int x_max = 10;
	int y_max = 10;
	x_loc_initial = rand()%x_max;
	y_loc_initial = rand()%y_max;
	x_loc = (double)x_loc_initial;
	y_loc = (double)y_loc_initial;
}

void Agent::Reset(){
	x_loc = x_loc_initial;
	y_loc = y_loc_initial;
}

/////////////////////////////////////////////////////////////
//GridWorld

class GridWorld{
public:
    int xmapdim;
    int ymapdim;
    
    int countervalue;
    
    void mapinit();
    vector<int> xrowcounter; //x-values are pushed into this first. Then the rows are pushed in.
    vector<vector<int>> gridcounter; //Made up of rows made up of xdim's.
    
    void gridcountereval(int x_loc, int y_loc);
};

void GridWorld::mapinit(){
    xmapdim = 100;
    ymapdim = 100;
    
    for(int j=0; j<ymapdim; j++){
        for (int i=0; i<xmapdim; i++){
            xrowcounter.push_back(0);
        }
        gridcounter.push_back(xrowcounter);
    }
}

void GridWorld::gridcountereval(int x_loc, int y_loc){
    //Set
    countervalue = gridcounter.at(y_loc).at(x_loc); //grid->into rows->into cells
    
    //Change
    countervalue += 1;
    
    //Update
    gridcounter.at(y_loc).at(x_loc);
}


/////////////////////////////////////////////////////////////
//Population
class Policy{
public:
	int Time;
	double Exploration;

	void init();
	void eval();
	void downselect();
	void mutate();

	vector<int> Actions;
};

void Policy::init(){
	for (int i = 0; i < 1000; i++){
		int q = rand()%6;
		Actions.push_back(q);
		if(Actions.at(i) = 5){
			break;
		}
	}
}


int main(){
	srand(time(NULL));
	
	GridWorld Map;
    	Map.mapinit();
	
	Agent A;
	A.init();
	cout << A.x_loc << "\t" << A.y_loc << endl;
	
	Policy P;
	P.init();

	//Somewhere in the loop operation when the simulation makes the agent travel to a specific location...
    	Map.gridcountereval(x_loc, y_loc); //Here: x_loc and y_loc refer to the

	return 0;
}
