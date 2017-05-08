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
		int options = rand()%10;
		if (options > 6){
			int q = rand()%5;
			Actions.push_back(q);
		}
		else if (options <= 6){
			int q = rand()%6;
			Actions.push_back(q);
		}
	}
}




int main(){
	srand(time(NULL));

	Agent A;
	A.init();
	cout << A.x_loc << "\t" << A.y_loc << endl;
	Policy P;
	P.init();
	

	return 0;
}