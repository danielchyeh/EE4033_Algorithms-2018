// **************************************************************************
//  File       [main.cpp]
//  Author     [Yun-Hsiang Liang]
//  Synopsis   [The main program for 2018 ICCAD Problem E]
//  Modify     [2018/05/24 Yun-Hsiang Liang]
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
    cout << "usage: Routing <input_file_pins> <input_file_blockages> <input_file_nets> <output_file>" << endl;
}

int main(int argc, char* argv[]) {

    if (argc != 5) {
       help_message();
       return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;


    //////////// read the input file /////////////

    vector<int> pin_x;
  	vector<int> pin_y;
  	vector<int> block_x;
  	vector<int> block_y;
    vector<int> metal;
    vector<bool> criticalnet;

    int** Net;
    int n = 100;
  	Net = new int *[n];
  	for (int i = 0; i < n; i++) {
  		Net[i] = new int[n];
  	}

    char line[1000];
  	fstream fin1(argv[1]), fin2(argv[2]), fin3(argv[3]);

    // Read pins
    int id, layer, x1, y1, x2, y2;
    while (fin1 >> id >> layer >> x1 >> y1) {
      metal.push_back(layer);
      pin_x.push_back(x1);
      pin_y.push_back(y1);
    }

    // Read blockages
    while (fin2 >> x1 >> y1 >> x2 >> y2) {
      block_x.push_back(x1);
      block_x.push_back(x2);
      block_y.push_back(y1);
      block_y.push_back(y2);
    }

    // Read nets
    int i = 0, j = 0, num;
    char c;
    string buffer;
  	while (getline(fin3, buffer)) {
      istringstream iss(buffer);
      while (iss >> num){
        Net[i][j] = num;
        j++;
      }
      iss >> c;
      if (c == 'Y') {
        criticalnet.push_back(true);
      } else if (c == 'N') {
        criticalnet.push_back(false);
      } else {
        cout << "Encountered a problem when reading nets." << endl;
      }
    }



    //////////// perform routing on the nets ////

    tmusg.periodStart();

    Routing Router;

    // TODO



    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////

    fstream fout(argv[4], ios::out);
    // TODO



    // Deallocate dynamic memory
    for (int = 0; i < n; i++) {
      delete [] Net[i];
    }
    delete [] Net;

    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;

    return 0;
}
