// **************************************************************************
//  File       [main.cpp]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The implementation of worker ant function]
//  Modify     [2018/03/20 Chun-Hsiao Yeh]
// **************************************************************************

#include "WorkerAnt.h"
#include <iostream>
#include <stdlib.h>

#include <cmath> 

WorkerAnt::WorkerAnt() {}
using namespace std;
//Implementation of Greedy Algorithm based on worker ant problem
void WorkerAnt::Greedy(int maxW, vector<int>& Dis_x, vector<int>& Dis_y, vector<int>& Weight, vector<int>& DistGD) {
	int CarryLimit = 0;
	int TotalDis = 0;
	int keyLoca_x = 0;
	int keyLoca_y = 0;
	vector<int> BackPoint;
	int CarryLimitPre = 0;

	for (int i = 0; i < Dis_x.size(); i++){
		TotalDis = TotalDis + (abs(Dis_x[i] - keyLoca_x) + abs(Dis_y[i] - keyLoca_y));
		keyLoca_x = Dis_x[i];
		keyLoca_y = Dis_y[i];

		CarryLimit = CarryLimit + Weight[i];

		if (i < Dis_x.size() - 1){ CarryLimitPre = CarryLimit + Weight[i + 1]; }
		if (CarryLimit == maxW || CarryLimitPre > maxW){

			CarryLimit = 0;
			keyLoca_x = 0;
			keyLoca_y = 0;
			TotalDis = TotalDis + (abs(Dis_x[i] - keyLoca_x) + abs(Dis_y[i] - keyLoca_y));
			BackPoint.push_back(i + 1);
			DistGD.push_back(i + 1);

		}
		CarryLimitPre = 0;

		if (i == (Dis_x.size() - 1)){
			TotalDis = TotalDis + (abs(Dis_x[i] - 0) + abs(Dis_y[i] - 0));
			BackPoint.push_back(i + 1);
			DistGD.push_back(i + 1);
			cout << TotalDis << endl;
			DistGD.push_back(TotalDis);
		}

	}

}

//Implementation of Dynamic Programming based on worker ant problem
void WorkerAnt::DynamicP(int maxW, vector<int>& Dis_x, vector<int>& Dis_y, vector<int>& Weight, vector<int>& DistDP) {
	
	
	int dis = 0;
	//int D = 0;
	vector<int> DP;
	//D.push_back(0);
	int D[4] = { 0 };
	int val = 0;
	int min = 1000000000;
	int ds = 0;
	int Carry = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int gg = 0;
	int CarryAll = 0;

	vector<int> Dis_x2;
	vector<int> Dis_y2;
	vector<int> Weight2;

	vector<int> Minn;
	vector<int> Minnj;

	Dis_x2.push_back(0);
	Dis_y2.push_back(0);
	Weight2.push_back(0);
	int couti = 0;
	for (int w = 0; w < Dis_x.size(); w++){
		Dis_x2.push_back(Dis_x[w]);
		Dis_y2.push_back(Dis_y[w]);
		Weight2.push_back(Weight[w]);
	}


	const int N = 100000;//Dis_x.size();
	int arryDP[N] = { 0 };

	for (int k = 0; k <= Dis_x.size(); k++){
		arryDP[k] = -1;
	}

	for (int i = 1; i < Dis_x2.size(); i++){

		arryDP[0] = 0;
		for (int j = 0 ; j <= i-1; j++){

			for (int ww = j +1; j<i && ww <= i; ww++){
				
				CarryAll = CarryAll + Weight2[ww];
			}
			if (CarryAll<=maxW){
				A = (abs(Dis_x2[j+1] - 0) + abs(Dis_y2[j+1] - 0));
				
				for (int st = j + 1; st < i; st++){
					B = B + (abs(Dis_x2[st + 1] - Dis_x2[st]) + abs(Dis_y2[st + 1] - Dis_y2[st]));
				}

				C = (abs(Dis_x2[i] - 0) + abs(Dis_y2[i] - 0));
				val = arryDP[j] + A + B + C;
				B = 0;
				couti++;

				Minn.push_back(val);
				Minnj.push_back(j);

				//DP.push_back(val);
				if (val < min){
					min = val;
				}
				CarryAll = 0;
			}
			else{
				CarryAll = 0;
			if (i == (Dis_x2.size() - 1)){
				if (j != 0){
					DistDP.push_back(j+1);
				}
				//printf("J = %d", j);
			}
			}						
		}
		couti = 0;
		ds = min;
		arryDP[i] = ds;	
		min = 100000000;//set min value bigger than all conditions
		
	}

	DistDP.push_back(Dis_x2.size()-1);
	DistDP.push_back(arryDP[Dis_x2.size() - 1]);
	cout << arryDP[Dis_x2.size() - 1] << endl;
	//printf("arrDP = %d\n", arryDP[Dis_x2.size() - 1]);



}