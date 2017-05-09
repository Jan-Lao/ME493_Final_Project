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
#include <assert.h>
#include <cmath>

using namespace std;

////////////////////////////////////////////////////////
//Agent
class Agent{
public:
	double x_loc;
	double y_loc;
	double x_loc_initial;
	double y_loc_initial;
	int Action;
	int GoHome;

	int x_max = 100;
	int y_max = 100;

	void init();
	void Simulate();
	void Reset();
};

void Agent::init(){

	x_loc_initial = rand()%x_max;
	y_loc_initial = rand()%y_max;
	x_loc = (double)x_loc_initial;
	y_loc = (double)y_loc_initial;
}

void Agent::Reset(){
	x_loc = x_loc_initial;
	y_loc = y_loc_initial;
}

void Agent::Simulate(){
	while((Action == 0)&&(y_loc == y_max-1)){
		Action++;
	}
	while((Action == 1)&&(x_loc == x_max-1)){
		Action++;
		}
	while((Action == 2)&&(y_loc < 1 )){
		Action++;
	}
	while((Action == 3)&&(x_loc < 1)){
		Action++;
	}
	if(Action == 0){
		y_loc++;
	}
	else if(Action == 1){
		x_loc++;
	}
	else if(Action == 2){
		y_loc--;
	}
	else if(Action == 3){
		x_loc--;
	}
	else if(Action ==4){
		//Do nothing
	}
	else if(Action == 5){
		GoHome = 1;
	}
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
    gridcounter.at(y_loc).at(x_loc) = countervalue;
}


/////////////////////////////////////////////////////////////
//Population
class Policy{
public:
	int Time;
	double Exploration;

	void init();
	void eval(vector<vector<int> > gridcounter);
	void downselect();
	void mutate();

	vector<int> Actions;
};

void Policy::init(){
	for (int i = 0; i < 5000; i++){
		int options = rand()%100;
		if (options < 99){
			int q = rand()%5;
			Actions.push_back(q);
		}
		else if (options >= 99){
			int q = rand()%6;
			Actions.push_back(q);
		}
	}
	Time = 0;
	Exploration = 0;
}








int main(){
	srand(time(NULL));
	int pop_size = 10;
	int num_gen = 1;

	Agent A;
	A.init();

	vector<Policy> Population;
	for (int i = 0; i < pop_size; i++){
		Policy P;
		P.init();
		Population.push_back(P);
	}
	for (int g = 0; g < num_gen; g++){
		for (int i = 0; i < pop_size; i++){
			A.Reset();
			GridWorld Map;
			Map.mapinit();
			for (int j = 0; j < Population.at(i).Actions.size(); j++){
				A.Action = Population.at(i).Actions.at(j);
				A.GoHome = 0;
				A.Simulate();
				Population.at(i).Actions.at(j) = A.Action;
				//Somewhere in the loop operation when the simulation makes the agent travel to a specific location...
				Map.gridcountereval(A.x_loc, A.y_loc); //Here: x_loc and y_loc refer to the
				Population.at(i).Time++;
				if(A.GoHome == 1){
					break;
				}
			}
			for (int h = 0; h < Map.ymapdim; h++){
				for (int y = 0; y < Map.xmapdim; y++){
					Population.at(i).Exploration = Population.at(i).Exploration + (1-exp(-.9*Map.gridcounter.at(h).at(y)));
				}
			}
			cout << Population.at(i).Exploration << "\t" << Population.at(i).Time << endl;
		}
		//Downselect

		//Mutate
	}
	return 0;
}
