// **************************************************************************
//  File       [main.cpp]
//  Author     [Yun-Hsiang Liang]
//  Synopsis   [The main program for 2018 ICCAD Problem E]
//  Modify     [2018/05/26 Yun-Hsiang Liang, Chun-Hsiao Yeh, Chi-Fan Juang]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "../lib/tm_usage.h"
#include "Routing.h"

using namespace std;

void help_message() {
	cout << "usage: Routing <input_file_pins> <input_file_nets> <input_file_blockages> <output_file>" << endl;
}

int main(int argc, char* argv[])
{
	
    if(argc != 5) {
       help_message();
       return 0;
    }
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;

	//////////// read the input file /////////////

	/*string input_pin = argv[1];//"./cases_0509/case1_pin.in";
	string input_net = argv[2];//"./cases_0509/case1_net.in";
	string input_block = argv[3];//"./cases_0509/case1_blockage.in";

	string output = argv[4];//"./cases_0509/case1.out";
	*/

	//
	vector<int> pin_x;
	vector<int> pin_y;
	vector<int> block_x;
	vector<int> block_y;
	vector<int> metal;
	vector<bool> criticalnet;

	vector<int> netX;
	vector<int> netY;

	int** Net;
	int n = 100;
	Net = new int *[n];
	for (int i = 0; i < n; i++) {
		Net[i] = new int[n];
	}

	char line[1000];
	fstream fin1(argv[1]), fin2(argv[2]), fin3(argv[3]);

	// Read pins
	int id, layer;
	float x1, y1, x2, y2;
	while (fin1 >> id >> layer >> x1 >> y1) {
		metal.push_back(layer);
		pin_x.push_back(int(2 * x1));
		pin_y.push_back(int(2 * y1));
	}

	// Read blockages
	while (fin3 >> x1 >> y1 >> x2 >> y2) {
		block_x.push_back(int(2 * x1));
		block_x.push_back(int(2 * x2));
		block_y.push_back(int(2 * y1));
		block_y.push_back(int(2 * y2));
	}

	// Read nets
	int i = 0, j = 0, num;
	char c;
	int num_nets = 0;
	string buffer;
	while (getline(fin2, buffer)) {
		istringstream iss(buffer);
		int in_flag = 0;

		while (iss >> num){
			if (in_flag > 0){
				Net[i][j] = num;
				j++;
			}
			in_flag++;
		}
		in_flag = 0;
		j = 0;
		i++;
		/*
		iss >> c;
		if (c == 'Y') {
			criticalnet.push_back(true);
		}
		else if (c == 'N') {
			criticalnet.push_back(false);
		}
		else {
			cout << "Encountered a problem when reading nets." << endl;
		}
		i++;
		printf("%d ", Net[0][1]);
		*/
		num_nets++;
	}

	//////////// perform routing on the nets ////
	tmusg.periodStart();

	Routing RoutMethod;

	RoutMethod.routing(pin_x, pin_y, metal, block_x, block_y, Net, num_nets, netX, netY);


    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////
	fstream fout;
	fout.open(argv[4], ios::out);

	//show and save result to output file
	int netk = 0;
	for (int i = 0; i < num_nets; i++){
		cout << "Net " << i + 1 << endl;
		fout << "Net " << i + 1 << endl;

		int fla = 0;

		int color_major, color_minor;//the output format of metal should be like 2(color_major) 1(color_minor)

		for (int j = 1; j < 20; j++){//20 is just constant, means no more than 20 pins in a net
			if (Net[i][j]>0){
				for (int k = netk; k < netX.size(); k++){
					if (!((pin_x[Net[i][j] - 1] == netX[k]) && (pin_y[Net[i][j] - 1] == netY[k]))){
						if (i == 0){
							//compare the path with pin array to see if path includes pin position, if yes, we assign the metal of pin into path.
							int index_w = 0;
							int flag_w = 0;
							for (int w = 0; w<pin_x.size(); w++){
								if (((netX[k] == pin_x[w]) && (netY[k] == pin_y[w])) || ((netX[k + 1] == pin_x[w]) && (netY[k + 1] == pin_y[w]))){//comparasion between path and pin array
									index_w = w;
									flag_w = 1;
								}
							}
							if (flag_w == 1){
								color_major = metal[index_w];
								color_minor = 2;
							}
							else{
								if (abs(netX[k] - netX[k + 1])>0){
									color_major = 1;
									color_minor = 1;
								}
								else{
									color_major = 2;
									color_minor = 1;
								}
							}


							cout << "+ " << float(netX[k]) / 2 << " " << float(netY[k]) / 2 << " " <<
								float(netX[k + 1]) / 2 << " " << float(netY[k + 1]) / 2 << " " << color_major << " " << color_minor << endl;
							fout << "+ " << float(netX[k]) / 2 << " " << float(netY[k]) / 2 << " " <<
								float(netX[k + 1]) / 2 << " " << float(netY[k + 1]) / 2 << " " << color_major << " " << color_minor << endl;
						}
						else{
							if (fla>0){
								int index_w = 0;
								int flag_w = 0;
								for (int w = 0; w<pin_x.size(); w++){
									if (((netX[k] == pin_x[w]) && (netY[k] == pin_y[w])) || ((netX[k + 1] == pin_x[w]) && (netY[k + 1] == pin_y[w]))){
										index_w = w;
										flag_w = 1;
									}
								}
								if (flag_w == 1){
									color_major = metal[index_w];
									color_minor = 2;
								}
								else{
									if (abs(netX[k] - netX[k + 1])>0){
										color_major = 1;
										color_minor = 1;
									}
									else{
										color_major = 2;
										color_minor = 1;
									}
								}

								cout << "+ " << float(netX[k]) / 2 << " " << float(netY[k]) / 2 << " " <<
									float(netX[k + 1]) / 2 << " " << float(netY[k + 1]) / 2 << " " << color_major << " " << color_minor << endl;
								fout << "+ " << float(netX[k]) / 2 << " " << float(netY[k]) / 2 << " " <<
									float(netX[k + 1]) / 2 << " " << float(netY[k + 1]) / 2 << " " << color_major << " " << color_minor << endl;
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


	fin1.close();
	fin2.close();
	fin3.close();
	fout.close();


    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;
	
    return 0;
}

