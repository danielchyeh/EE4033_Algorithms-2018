// **************************************************************************
//  File       [main.cpp]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The main program of 2018 Spring Algorithm PA2]
//  Modify     [2018/03/20 Chun-Hsiao Yeh]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "../lib/tm_usage.h"
#include "WorkerAnt.h"

using namespace std;

void help_message() {
    cout << "usage: WorkerAnt -[GD|DP] <input_file> <output_file>" << endl;
    cout << "options:" << endl;
    cout << "   GD  - Greedy" << endl;
    cout << "   DP  - Dynamic Programming" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 4) {
       help_message();
       return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;


    //////////// read the input file /////////////
	int maxW = 0;
	int numIndex = 0;
	vector<int> Dis_x;
	vector<int> Dis_y;
	vector<int> Weight;

	//int DistGD = 0;
	vector<int> DistGD;

	char buffer[200];
	char line[100];

	//max capacity of ant
	fstream fin(argv[2]);
	fin.getline(line, sizeof(line));

	string str(line);
	maxW = atoi(str.c_str());

	//number of items
	fin.getline(line, sizeof(line));
	string strnum(line);
	numIndex = atoi(strnum.c_str());

	int contL = 0;
	while (contL<numIndex){
		fin.getline(line, sizeof(line));
		//cout << line << endl;
		string conb = "";
		int mode = 0;
		for (int i = 0; i < sizeof(line); i++){
			if (mode == 0){
				if (line[i] == ' '){
					mode++;
					i++;
					Dis_x.push_back(atoi(conb.c_str()));
					conb = "";
				}
				if (mode == 0){
					conb = conb + line[i];

				}
			}
			if (mode == 1){
				if (line[i] == ' '){
					i++;
					mode++;
					Dis_y.push_back(atoi(conb.c_str()));
					conb = "";
				}
				if (mode == 1){
					conb = conb + line[i];

				}
			}
			if (mode == 2){
				if (line[i] == '\0'){
					Weight.push_back(atoi(conb.c_str()));
					conb = "";
					break;
				}
				if (mode == 2){
					conb = conb + line[i];
				}
			}
		}
		contL++;
	}


	fstream fout;
	fout.open(argv[3], ios::out);

	cout << Dis_y[1] << endl;

    //////////// find the solution to pick up the food ////
    tmusg.periodStart();
	WorkerAnt WorkMethod;

    if(strcmp(argv[1], "-GD") == 0) {
       // greedy 
		WorkMethod.Greedy(maxW, Dis_x, Dis_y, Weight, DistGD);
    }
    else if(strcmp(argv[1], "-DP") == 0) {
      // dynamic programming  
		WorkMethod.DynamicP(maxW, Dis_x, Dis_y, Weight, DistGD);
    }
    else {
        help_message();
        return 0;
    }

    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////


    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;


	for (int i = 0; i < DistGD.size(); i++)
		fout << DistGD[i] << endl;
	fin.close();
	fout.close();


    return 0;
}

