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

class Agent{
public:
	int x_loc;
	int y_loc;

	void init();
	void Simulate();
	void Reset();
};

void Agent::init(){
	int x_max = 10;
	int y_max = 10;
	x_loc = rand()%x_max;
	y_loc = rand()%y_max;
}





int main(){
	srand(time(NULL));


	Agent A;
	A.init();
	cout << A.x_loc << "\t" << A.y_loc << endl;

	return 0;
}