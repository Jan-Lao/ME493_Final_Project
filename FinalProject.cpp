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
#include <algorithm>
#include <string>

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

////NSGA-II info from:
////https://www.iitk.ac.in/kangal/Deb_NSGA-II.pdf

class Policy{
public:
	int Time;
	double Exploration;
	int domination_count;
	double distance;

	void init();
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
	domination_count = 0;
	distance = 0;
}

vector<Policy> eval(vector<Policy> Population, int i ,vector<vector<int> > gridcounter, int ymapdim, int xmapdim){
	for (int h = 0; h < ymapdim; h++){
		for (int y = 0; y < xmapdim; y++){
			//Evaluating Exploration values for each policy i
			Population.at(i).Exploration = Population.at(i).Exploration + (1-exp(-.9*gridcounter.at(h).at(y)));
		}
	}
	return Population;
}

vector<vector<Policy> > find_fronts(vector<Policy> Population,vector<vector<Policy> > nondominated_sets){
	for (int z = 0 ; z < Population.size(); z++){
		for (int j = 0; j < Population.size(); j++){
			if((Population.at(z).Exploration > Population.at(j).Exploration)&&(Population.at(z).Time < Population.at(j).Time)){
				Population.at(j).domination_count++;
			}
		}
	}
	for (int front_num = 0; front_num < Population.size(); front_num++){
		vector<Policy> front;
		for (int i = 0; i < Population.size(); i++){
			if(Population.at(i).domination_count-front_num == 0){
				Policy N;
				N = Population.at(i);
				front.push_back(N);
			}
		}
		nondominated_sets.push_back(front);
	}
	return nondominated_sets;
}

struct less_than_key_time{
	inline bool operator() (const Policy& a, const Policy& b){
			return(a.Time < b.Time);
		}
};

struct less_than_key_exp{
	inline bool operator() (const Policy& a, const Policy& b){
			return(a.Exploration < b.Exploration);
		}
};

struct less_than_key_dist{
	inline bool operator() (const Policy& a, const Policy& b){
			return(a.distance < b.distance);
		}
};

vector<vector<Policy> > calc_distance(vector<vector<Policy> > nondominated_sets){
	for (int i = 0; i < nondominated_sets.size(); i++){
		if (nondominated_sets.at(i).size() > 2){
			sort(nondominated_sets.at(i).begin(), nondominated_sets.at(i).end(), less_than_key_time());
			nondominated_sets.at(i).at(0).distance = 100000;
			nondominated_sets.at(i).at((nondominated_sets.at(i).size())-1).distance = 100000;
			for (int j = 1; j < (nondominated_sets.at(i).size())-1; j++){
				nondominated_sets.at(i).at(j).distance = nondominated_sets.at(i).at(j).distance + (nondominated_sets.at(i).at(j+1).Time - nondominated_sets.at(i).at(j-1).Time)/(4999);
			}
			sort(nondominated_sets.at(i).begin(), nondominated_sets.at(i).end(), less_than_key_exp());
			nondominated_sets.at(i).at(0).distance = 100000;
			nondominated_sets.at(i).at((nondominated_sets.at(i).size())-1).distance = 100000;
			for (int j = 1; j < nondominated_sets.at(i).size()-1; j++){
				nondominated_sets.at(i).at(j).distance = nondominated_sets.at(i).at(j).distance + (nondominated_sets.at(i).at(j+1).Exploration - nondominated_sets.at(i).at(j-1).Exploration)/(3000);
			}
		}
	}
	return nondominated_sets;
}

vector<Policy> downselect(vector<vector<Policy> > nondominated_sets, int pop_size){
	vector<Policy> newPop;
	vector<Policy> Population;
	Policy P;
	while(newPop.size() < (pop_size/2)){
			for (int i = 0; i < nondominated_sets.size(); i++){
					if ((newPop.size() < nondominated_sets.at(i).size() + pop_size/2)){
						for (int j = 0; j < nondominated_sets.at(i).size(); j++){
						cout << i << "\t" << nondominated_sets.at(i).at(j).domination_count << endl;
						newPop.push_back(nondominated_sets.at(i).at(j));
						}
					}
					else if (newPop.size() > nondominated_sets.at(i).size() + pop_size/2){
						//cout << "get here?" << endl;
						for (int j = 0; j < nondominated_sets.at(i).size(); j++){
							sort(nondominated_sets.at(i).begin(), nondominated_sets.at(i).end(), less_than_key_dist());
							newPop.push_back(nondominated_sets.at(i).at(j));
						}
						//cout << newPop.at(i).domination_count << endl;
					}
			}
		}
		Population = newPop;
		//assert(Population.size() == pop_size/2);
	return Population;
}

int main(){
	srand(time(NULL));
	int pop_size = 100;
	int num_gen = 1;

	Agent A;
	A.init();
	vector<Policy> Population;
	vector<vector<Policy> > nondominated_sets;
	for (int i = 0; i < pop_size; i++){
		Policy P;
		P.init();
		Population.push_back(P);
	}
	for (int g = 0; g < num_gen; g++){
		for (int i = 0; i < pop_size; i++){
			Policy P;
			A.Reset();
			GridWorld Map;
			Map.mapinit();
			for (int j = 0; j < Population.at(i).Actions.size(); j++){
				A.Action = Population.at(i).Actions.at(j);
				A.GoHome = 0;
				A.Simulate();
				Population.at(i).Actions.at(j) = A.Action; //Replace old action with new action altered during simulaton
				//Somewhere in the loop operation when the simulation makes the agent travel to a specific location...
				Map.gridcountereval(A.x_loc, A.y_loc); //Here: x_loc and y_loc refer to the
				Population.at(i).Time++;
				if(A.GoHome == 1){
					break;
				}
			}
			Population = eval(Population, i, Map.gridcounter, Map.ymapdim, Map.xmapdim);
		}
		//assert(Population.size() == pop_size);

		//Nondominated sorting and density preservation
		nondominated_sets = find_fronts(Population, nondominated_sets);
		nondominated_sets = calc_distance(nondominated_sets);

		Population = downselect(nondominated_sets, pop_size);
		cout << Population.size() << endl;
		cout << nondominated_sets.at(0).size() << endl;
		cout << nondominated_sets.at(1).size() << endl;
		cout << nondominated_sets.at(2).size() << endl;
		for (int i = 0; i < Population.size(); i++){
			//cout << i << "\t" <<Population.at(i).domination_count << endl;
		}

		//Mutate
	}
	return 0;
}
