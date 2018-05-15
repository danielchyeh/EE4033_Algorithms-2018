// **************************************************************************
//  File       [WorkerAnt.h]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The header file of worker ant]
//  Modify     [2018/03/20 Chun-Hsiao Yeh]
// **************************************************************************

#ifndef _WORKERANT_H
#define _WORKERANT_H

#include<vector>
using namespace std;

class WorkerAnt {
public:

	WorkerAnt(); // constructor

	void        Greedy(int, vector<int>&, vector<int>&, vector<int>&, vector<int>&);
	void        DynamicP(int, vector<int>&, vector<int>&, vector<int>&, vector<int>&);

private:


};

#endif
