// **************************************************************************
//  File       [main.cpp]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The main program of 2018 Spring Algorithm PA3]
//  Modify     [2018/04/08 Chun-Hsiao Yeh]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "../lib/tm_usage.h"
#include "Routing.h"

using namespace std;

void help_message() {
    cout << "usage: Routing <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[])
{
	
    if(argc != 3) {
       help_message();
       return 0;
    }
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;

	//string input = "./inputs/case4.in";
	//string output = "./outputs/case4.out";


	//////////// read the input file /////////////


	//////////// perform routing on the nets ////

	int num_pins = 0;
	int num_bonds = 0;
	int num_nets = 0;
	
	vector<int> Pin_x;
	vector<int> Pin_y;
	vector<int> Bond_x;
	vector<int> Bond_y;

	//int DistGD = 0;
	vector<int> Net_in;
	vector<int> Net_out;

	vector<int> netX;
	vector<int> netY;

	char line[1000];

	//max capacity of ant
	fstream fin(argv[1]);
	//Num of items
	fin.getline(line, sizeof(line));
	string str(line);
	str = str + " ";
	string num = "";
	bool num_flag = false;
	int num_con = 0;
	for (int i = 0; i < str.size(); i++){
		num = num + str[i];
		num_flag = false;
		if (str[i] == ' '&&num_con==0){
			num_pins = atoi(num.c_str());
			num_con++;
			num_flag = true;
			num = "";
		}
		if (str[i] == ' '&&num_con == 1 && num_flag == false){
			num_bonds = atoi(num.c_str());
			num = "";
			num_con++;
			num_flag = true;
		}
		if (str[i] == ' '&&num_con == 2 && num_flag == false){
			num_nets = atoi(num.c_str());
			num = "";
			
		}
	}

	//Pins
	string comb = "";
	bool comb_flag = false;
	int comb_con = 0;
	int ax = 0;
	int ay = 0;

	for (int i = 0; i < num_pins; i++){
		fin.getline(line, sizeof(line));
		string axis_pin(line);
		axis_pin = axis_pin + " ";
		for (int j = 0; j < axis_pin.size(); j++){
			comb = comb + axis_pin[j];
			comb_flag = false;
			if (axis_pin[j] == ' '&& comb_con==0){
				comb_flag = true;
				comb = "";
				comb_con++;
			}
			if (axis_pin[j] == ' '&&comb_con == 1&&comb_flag == false){
				comb_flag = true;
				ax = atoi(comb.c_str());
				Pin_x.push_back(ax);
				comb_con++;
				comb = "";
			}
			if (axis_pin[j] == ' '&&comb_con == 2 && comb_flag ==false){
				ay = atoi(comb.c_str());
				Pin_y.push_back(ay);
			}			

		}

		ax = 0;
		ay = 0;
		comb = "";
		comb_con = 0;
		//char str11[] = str1;		
	}

	//Bond
	string bond = "";
	bool bond_flag = false;
	int bond_con = 0;

	for (int i = 0; i < num_bonds; i++){
		fin.getline(line, sizeof(line));
		string axis_bond(line);
		axis_bond = axis_bond + " ";
		for (int j = 0; j < axis_bond.size(); j++){
			bond = bond + axis_bond[j];
			bond_flag = false;
			if (axis_bond[j] == ' '&&bond_con == 0){
				Bond_x.push_back(atoi(bond.c_str()));
				bond_con++;
				bond_flag = true;
				bond = "";
			}
			if (axis_bond[j] == ' '&&bond_con == 1 && bond_flag == false){
				Bond_y.push_back(atoi(bond.c_str()));
				bond_con++;
				bond_flag = true;
				bond = "";
			}
			if (axis_bond[j] == ' '&&bond_con == 2 && bond_flag == false){
				Bond_x.push_back(atoi(bond.c_str()));
				bond_con++;
				bond_flag = true;
				bond = "";
			}
			if (axis_bond[j] == ' '&&bond_con == 3 && bond_flag == false){
				Bond_y.push_back(atoi(bond.c_str()));
				bond_con++;
				bond_flag = true;
				bond = "";
			}

		}
		bond = "";
		bond_con = 0;
	}


	//Nets
	string net = "";
	bool net_flag = false;
	int net_con = 0;

	int ** Net;
	int n = 100;
	int jcont = 0;
	Net = new int *[n];

	for (int i = 0; i<n; i++)
	{
		Net[i] = new int[n];
		
	}

	for (int i = 0; i < num_nets; i++){
		fin.getline(line, sizeof(line));
		string axis_net(line);
		axis_net = axis_net + " ";
		//axis_net = "1 2 3 4 5 ";
		for (int j = 0; j < axis_net.size(); j++){
			net = net + axis_net[j];
			if (axis_net[j] == ' '){
				if (jcont>0){
					Net[i][jcont-1] = atoi(net.c_str());
					
					//printf("num[%d %d] = %d\n", i, jcont-1, Net[i][jcont-1]);
				}
				net = "";
				jcont++;
	
			}

		}
		jcont = 0;
		
	}
	

	tmusg.periodStart();

	Routing RoutMethod;

	RoutMethod.routing(Pin_x, Pin_y, Bond_x, Bond_y, Net_in, Net_out, Net, num_nets,netX,netY);
	


	fstream fout;
	fout.open(argv[2], ios::out);

	int netk = 0;
	for (int i = 0; i < num_nets; i++){
		fout << "Net " << i + 1 << endl;
		int fla = 0;

		for (int j = 1; j < 20; j++){
			if (Net[i][j]>0){
				for (int k = netk; k < netX.size(); k++){
					if (!((Pin_x[Net[i][j] - 1] == netX[k]) && (Pin_y[Net[i][j] - 1] == netY[k]))){
						if (i == 0){
							fout << netX[k] << " " << netY[k] << " " << netX[k + 1] << " " << netY[k + 1] << endl;
						}
						else{
							if (fla>0){
								fout << netX[k] << " " << netY[k] << " " << netX[k + 1] << " " << netY[k + 1] << endl;
							}
							fla++;
						}
					}
					else{
						netk = k;
						break;
					}
				}
			}
			else{ break; }
		}
		fla = 0;

	}



	fin.close();
	fout.close();

	//system("pause");

    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////


    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;
	
    return 0;
}

