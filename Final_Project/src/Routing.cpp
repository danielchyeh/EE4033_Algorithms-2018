// **************************************************************************
//  File       [Routing.cpp]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The implementation of 2018 CAD E]
//  Modify     [2018/05/26 Chun-Hsiao Yeh et al.]
// **************************************************************************

#include "Routing.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue> 
#include <cmath> 

Routing::Routing() {}
using namespace std;


typedef struct {
	int x;
	int y;
} Point;

Point pt(int, int);
int visit(int **,int **, Point, Point, int c);

Point pt(int x, int y) {
	Point p = { x, y };
	return p;
}

const int MAX_N = 11;
const int MAX_M = 11;
const int INF = 0x3f3f3f3f;
typedef pair<int, int> P;

int N, M; //size of layout
int sx, sy; //the position of start 
int gx, gy; //the position of ternimal


int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 }; //the movement of x, y 

void bfs(int **maze, int **d, int **trace, int **update, vector<int>& netX, vector<int>& netY, int sx, int sy, int gx, int gy, int N, int M, vector<int>& metal, int me_in, int me_out)
{
	//the prefer direction of metal 1,3 is horizontal, metal 2,4 is vertical
	int metal_s = metal[me_in-1];//metal of start pin
	int metal_g = metal[me_out-1];//metal of terminal pin

	int temp_s_b1, temp_s_b2, temp_g_b1, temp_g_b2;//record the temp of initial value of block


	queue<P> que;
	for (int i = 0; i < N; i++)
	for (int j = 0; j < M; j++)
		d[i][j] = INF;  //initial all point of distance to be INF
	que.push(P(sx, sy));
	d[sx][sy] = 0;  //set distance of start point be 0, and put into queue

	maze[sx][sy] = 9;
	maze[gx][gy] = 9;

	//set the temp blockage to limit the outward direction of pin under metal condition
	if (metal_s == 1 || metal_s == 3){
		temp_s_b1 = maze[sx][sy+1];
		temp_s_b2 = maze[sx][sy-1];
		maze[sx][sy+1] = 2;
		maze[sx][sy-1] = 2;

	}
	else{
		temp_s_b1 = maze[sx+1][sy];
		temp_s_b2 = maze[sx-1][sy];
		maze[sx+1][sy] = 2;
		maze[sx-1][sy] = 2;

	}
	//set the temp blockage to limit the inward direction of pin under metal condition
	if (metal_g == 1 || metal_g == 3){
		temp_g_b1 = maze[gx][gy+1];
		temp_g_b2 = maze[gx][gy-1];
		maze[gx][gy+1] = 2;
		maze[gx][gy-1] = 2;
	}
	else{
		temp_g_b1 = maze[gx+1][gy];
		temp_g_b2 = maze[gx-1][gy];
		maze[gx+1][gy] = 2;
		maze[gx-1][gy] = 2;

	}

	//start to do BFS!
	while (que.size()) //promise it can get terminal
	{
		P p = que.front(); que.pop();//pop out the first value in queue 
		int i;
		for (i = 0; i < 4; i++)//go out with 4 directions
		{
			int nx = p.first + dx[i];
			int ny = p.second + dy[i];//update the position

			if (0 <= nx&&nx < N
				&& 0 <= ny&&ny < M
				&&maze[nx][ny] != 2
				&&d[nx][ny] == INF)//no worry the position which passed 
			{
				que.push(P(nx, ny));    //update the position  
				maze[nx][ny] = 1;
				d[nx][ny] = d[p.first][p.second] + 1;
				
				//we set 4(left), 5(down), 6(right), 8(up) in order to trace back!
				if (nx>p.first&&ny == p.second){//up to down
					maze[nx][ny] = 5;
				}
				else if (nx == p.first&&ny > p.second){//left to right
					maze[nx][ny] = 6;
				}
				else if (nx < p.first&&ny == p.second){//down to up
					maze[nx][ny] = 8;
				}
				else if (nx == p.first&&ny < p.second){//right to left
					maze[nx][ny] = 4;
				}

				if (nx == gx && ny == gy){

					break;
				}

			}
		}
		if (i != 4) break;
		
	}

	//remove the block near pin which limits the path direction 
	if (metal_s == 2 || metal_s == 4){
		maze[sx + 1][sy] = temp_s_b1;
		maze[sx - 1][sy] = temp_s_b2;
	}
	else{
		maze[sx][sy + 1] = temp_s_b1;
		maze[sx][sy - 1] = temp_s_b2;
	}

	if (metal_g == 2 || metal_g == 4){
		maze[gx + 1][gy] = temp_g_b1;
		maze[gx - 1][gy] = temp_g_b2;
	}
	else{
		maze[gx][gy + 1] = temp_g_b1;
		maze[gx][gy - 1] = temp_g_b2;
	}


	int trig1 = 0;
	int trig2 = 0;

	if (netX.empty() == true){
		netX.push_back(sx);
		netY.push_back(sy);
	}

	else if (!(netX.back() == sx && netY.back() == sy)){
		netX.push_back(sx);
		netY.push_back(sy);
	}
	int tri = 0;
	int finagx = gx, finagy = gy;

	vector <int> nettmpX;
	vector <int> nettmpY;
	
	//we have to trace back to start position from terminal
	//so we remember the path and set the direction of each step
	//while tracing back, we set 4(left),5(down),6(right),8(up).
	//we double the blockage size to prevent color conflit 
	int blox[8] = { 1, 0, -1, 0, -1, 1, -1, 1}, bloy[8] = { 0, 1, 0, -1, 1, -1 , -1, 1}; //the movement of x, y 
	int bloF = 0;

	while (!(sx == gx && sy == gy)){

		if (maze[gx][gy] == 5){
			trace[gx][gy] = 2;
			if (bloF > 0){
				for (int i = 0; i < 8; i++){
					if (trace[gx + blox[i]][gy + bloy[i]] != 2){
						trace[gx + blox[i]][gy + bloy[i]] = 2;
					}
				}
			}

			if (maze[gx][gy] != maze[gx - 1][gy]){
				//printf("T[%d,%d]", gx-1, gy);
				if (!((gx - 1) == sx&&gy == sy)){

					nettmpX.push_back((gx - 1));
					nettmpY.push_back(gy);

				}
			}
			gx--;

		}
		else if (maze[gx][gy] == 6){
			trace[gx][gy] = 2;
			if (bloF > 0){
				for (int i = 0; i < 8; i++){
					if (trace[gx + blox[i]][gy + bloy[i]] != 2){
						trace[gx + blox[i]][gy + bloy[i]] = 2;
					}
				}
			}
			if (maze[gx][gy] != maze[gx][gy - 1]){
				//printf("T[%d,%d]", gx, gy-1);
				if (!((gx) == sx&&(gy-1) == sy)){

					nettmpX.push_back((gx));
					nettmpY.push_back(gy-1);

				}
			}
			gy--;

		}
		else if (maze[gx][gy] == 8){
			trace[gx][gy] = 2;
			if (bloF > 0){
				for (int i = 0; i < 8; i++){
					if (trace[gx + blox[i]][gy + bloy[i]] != 2){
						trace[gx + blox[i]][gy + bloy[i]] = 2;
					}
				}
			}
			if (maze[gx][gy] != maze[gx+1][gy]){
				//printf("T[%d,%d]", gx+1, gy);
				if (!((gx+1) == sx && (gy) == sy)){

					nettmpX.push_back((gx + 1));
					nettmpY.push_back(gy);

				}
			}
			gx++;

		}
		else if (maze[gx][gy] == 4){
			trace[gx][gy] = 2;
			if (bloF > 0){
				for (int i = 0; i < 8; i++){
					if (trace[gx + blox[i]][gy + bloy[i]] != 2){
						trace[gx + blox[i]][gy + bloy[i]] = 2;
					}
				}
			}
			if (maze[gx][gy] != maze[gx][gy+1]){
				//printf("T[%d,%d]", gx, gy+1);
				if (!((gx) == sx && (gy+1) == sy)){

					nettmpX.push_back((gx));
					nettmpY.push_back(gy+1);

				}
			}
			gy++;

		}

		bloF++;//avoid the ternimal to be blocked
	}
	bloF = 0;

	for (int i = nettmpX.size()-1; i > -1; i--){
		netX.push_back(nettmpX[i]);
		netY.push_back(nettmpY[i]);
	}
	nettmpX.clear();
	nettmpY.clear();


	netX.push_back((finagx));
	netY.push_back(finagy);

	//we make double block size of each path to avoid color conflict


	trace[sx][sy] = 9;
	trace[finagx][finagy] = 9;
}


int LAYOUTSIZE = 300;// size of layout
void writeLayout(fstream& fout, int** layout){//layout visualization on txt file
	for (int i = 0; i < LAYOUTSIZE; i++) {
		for (int j = 0; j < LAYOUTSIZE; j++) {
			fout << layout[i][j];
		}
		fout << endl;
	}
}


void Routing::routing(vector<int>& Pin_x, vector<int>& Pin_y, vector<int>& metal, vector<int>& Bond_x, vector<int>& Bond_y,
	int **Net, int num_nets, vector<int>& netX, vector<int>& netY) {

	
	//set graph of 1000*1000 to be layout
	int n = 1001;
	int m = 1001;
	int ** Rgraph;
	Rgraph = new int *[n];

	for (int i = 0; i<n; i++)
	{
		Rgraph[i] = new int[m];

	}

	int ** RgraphDis;//graph of distance of path. can just ignore
	RgraphDis = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphDis[i] = new int[m];

	}

	int ** RgraphTrace;//Trace back the existed path and set it to be blockage
	RgraphTrace = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphTrace[i] = new int[m];

	}

	int ** RgraphUpdate;//Get the block positions from RgraphTrace and update to next Rgraph 
	RgraphUpdate = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphUpdate[i] = new int[m];

	}

	//Initializaiton of layout (unexplored position all = 0)
	for (int i = 0; i <= n-1; i++){
		for (int j = 0; j <= m-1; j++){
			Rgraph[i][j] = 0;
			RgraphTrace[i][j] = 0;
			RgraphUpdate[i][j] = 0;
		}
	}

	//set the block of the boundry. (boundry = 2)
	for (int i = 0; i <= n-1; i++){
		for (int j = 0; j <= m-1; j++){
			if (i == 0&&j<2000){
				Rgraph[i][j] = 2;
				RgraphTrace[i][j] = 2; 
				RgraphUpdate[i][j] = 2;
			}
			if (j == 0 && i<2000){
				Rgraph[i][j] = 2;
				RgraphTrace[i][j] = 2;
				RgraphUpdate[i][j] = 2;
			}
			if (i == n-1 && j<2000){
				Rgraph[i][j] = 2;
				RgraphTrace[i][j] = 2;
				RgraphUpdate[i][j] = 2;
			}
			if (j == m-1 && i<2000){
				Rgraph[i][j] = 2;
				RgraphTrace[i][j] = 2;
				RgraphUpdate[i][j] = 2;
			}
		}
	}
	
	//Set the blockage of layout (blockage = 2)
	for (int i = 0; i < Bond_x.size(); i++){
		for (int j = Bond_x[i]; j <= Bond_x[i + 1]; j++){
			for (int k = Bond_y[i]; k <= Bond_y[i + 1]; k++){
				Rgraph[j][k] = 2;
				RgraphTrace[j][k] = 2;
				RgraphUpdate[j][k] = 2;
			}
		}
		i+=1;
	}


	int metal_in, metal_out;
	//num_nets = 1;
	for (int i = 0; i < num_nets; i++){//iteration of each net
		for (int j = 0; j < 60; j++){//iteration of each pin in a net
			if (Net[i][j+1]>0){
				metal_in = Net[i][j];//limit the path direction of outward pin
				metal_out = Net[i][j + 1];//limit the path dirction of inward pin

				//Start to use BFS to find shortest path between pin to pin under conditions
				bfs(Rgraph, RgraphDis, RgraphTrace, RgraphUpdate, netX, netY, Pin_x[Net[i][j]-1], Pin_y[Net[i][j]-1], Pin_x[Net[i][j+1]-1], Pin_y[Net[i][j+1]-1], 1001, 1001, metal, metal_in, metal_out);

				//update the blockage of Rgraph (new blockage = explored path)
				for (int i = 0; i < N; i++){
					for (int j = 0; j < M; j++){
						if (RgraphTrace[i][j] != RgraphUpdate[i][j]){
							RgraphUpdate[i][j] = RgraphTrace[i][j];
						}
					}
				}
			}
			else{ break; }

			Rgraph = RgraphUpdate;
		}
		
	}

	/*
	fstream fout;
	string layoutvisual = "./cases_0509/case1_layout.out";
	fout.open(layoutvisual, ios::out);

	writeLayout(fout, RgraphTrace);
	*/
	

	/*
	//show and save result to output file
	int netk = 0;
	for (int i = 0; i < num_nets; i++){
		cout << "Net " << i+1 << endl;

		int fla = 0;

		int color_major, color_minor;//the output format of metal should be like 2(color_major) 1(color_minor)

		for (int j = 1; j < 20; j++){//20 is just constant, means no more than 20 pins in a net
			if (Net[i][j]>0){
				for (int k = netk; k < netX.size(); k++){
					if (!((Pin_x[Net[i][j]-1] == netX[k]) && (Pin_y[Net[i][j]-1] == netY[k]))){
						if (i == 0){
							//compare the path with pin array to see if path includes pin position, if yes, we assign the metal of pin into path.
							int index_w = 0;
							int flag_w = 0;
							for (int w = 0; w<Pin_x.size(); w++){
								if (((netX[k] == Pin_x[w]) && (netY[k] == Pin_y[w])) || ((netX[k + 1] == Pin_x[w]) && (netY[k + 1] == Pin_y[w]))){//comparasion between path and pin array
									index_w = w;
									flag_w = 1;
								}
							}
							if (flag_w==1){
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


							cout <<"+ "<< float(netX[k])/2 << " " << float(netY[k])/2 << " " <<
								float(netX[k + 1])/2 << " " << float(netY[k + 1])/2 <<" "<<color_major<<" "<<color_minor<<endl;
						}
						else{
							if (fla>0){
								int index_w = 0;
								int flag_w = 0;
								for (int w = 0; w<Pin_x.size(); w++){
									if (((netX[k] == Pin_x[w]) && (netY[k] == Pin_y[w])) || ((netX[k + 1] == Pin_x[w]) && (netY[k + 1] == Pin_y[w]))){
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

								cout <<"+ "<< float(netX[k])/2 << " " << float(netY[k])/2 << " " <<
									float(netX[k + 1]) / 2 << " " << float(netY[k + 1]) / 2 << " " << color_major << " " << color_minor <<endl;
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

	*/




	for (int i = 0; i<n; i++)
	{
		delete[] Rgraph[i];
	}
	delete[] Rgraph;


}