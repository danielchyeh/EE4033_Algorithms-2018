// **************************************************************************
//  File       [Routing.cpp]
//  Author     [Chun-Hsiao Yeh]
//  Synopsis   [The implementation of routing]
//  Modify     [2018/04/06 Chun-Hsiao Yeh]
// **************************************************************************

#include "Routing.h"
#include <iostream>
#include <stdlib.h>
#include <queue> 
#include <cmath> 

Routing::Routing() {}
using namespace std;


#define SIZE 100

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

int N, M;
int sx, sy; //the position of start 
int gx, gy; //the position of ternimal


int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 }; //the movement of x, y 

void bfs(int **maze, int **d, int **trace, int **update, vector<int>& netX, vector<int>& netY, int sx, int sy, int gx, int gy, int N, int M)
{


	queue<P> que;
	for (int i = 0; i < N; i++)
	for (int j = 0; j < M; j++)
		d[i][j] = INF;  //initial all point of distance to be INF
	que.push(P(sx, sy));
	d[sx][sy] = 0;  //set distance of start point be 0, and put into queue

	maze[sx][sy] = 9;
	maze[gx][gy] = 9;

	while (que.size()) //promise it can get terminal
	{
		P p = que.front(); que.pop();//pop out the first value in queue 
		int i;
		for (i = 0; i < 4; i++)
		{
			int nx = p.first + dx[i];
			int ny = p.second + dy[i];//update the position
			//
			//printf("ff");
			if (0 <= nx&&nx < N
				&& 0 <= ny&&ny < M
				&&maze[nx][ny] != 2
				&&d[nx][ny] == INF)//no worry the position which passed 
			{
				que.push(P(nx, ny));    //update the position  
				maze[nx][ny] = 1;
				d[nx][ny] = d[p.first][p.second] + 1;
				//printf("A[%d][%d] = %d from A[%d][%d]\n",nx,ny, d[nx][ny],p.first,p.second);
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
	while (!(sx == gx && sy == gy)){
		if (maze[gx][gy] == 5){
			trace[gx][gy] = 2;
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
			if (maze[gx][gy] != maze[gx][gy+1]){
				//printf("T[%d,%d]", gx, gy+1);
				if (!((gx) == sx && (gy+1) == sy)){

					nettmpX.push_back((gx));
					nettmpY.push_back(gy+1);

				}
			}
			gy++;

		}


	}

	for (int i = nettmpX.size()-1; i > -1; i--){
		netX.push_back(nettmpX[i]);
		netY.push_back(nettmpY[i]);
	}
	nettmpX.clear();
	nettmpY.clear();


	netX.push_back((finagx));
	netY.push_back(finagy);
	trace[sx][sy] = 2;
	trace[finagx][finagy] = 2;
}


void Routing::routing(vector<int>& Pin_x, vector<int>& Pin_y, vector<int>& Bond_x, vector<int>& Bond_y,vector<int>& Net_in, vector<int>& Net_out,
	int **Net, int num_nets, vector<int>& netX, vector<int>& netY) {

	
	//set graph of 1000*1000
	int n = 1001;
	int m = 1001;
	int ** Rgraph;
	Rgraph = new int *[n];

	for (int i = 0; i<n; i++)
	{
		Rgraph[i] = new int[m];

	}

	int ** RgraphDis;
	RgraphDis = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphDis[i] = new int[m];

	}

	int ** RgraphTrace;
	RgraphTrace = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphTrace[i] = new int[m];

	}

	int ** RgraphUpdate;
	RgraphUpdate = new int *[n];

	for (int i = 0; i<n; i++)
	{
		RgraphUpdate[i] = new int[m];

	}

	for (int i = 0; i <= n-1; i++){
		for (int j = 0; j <= m-1; j++){
			Rgraph[i][j] = 0;
			RgraphTrace[i][j] = 0;
			RgraphUpdate[i][j] = 0;
		}
	}

	//set the block of the boundry. (2)
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

	
	for (int i = 0; i < num_nets; i++){
		for (int j = 0; j < 60; j++){
			if (Net[i][j+1]>0){
				bfs(Rgraph, RgraphDis, RgraphTrace, RgraphUpdate, netX, netY, Pin_x[Net[i][j]-1], Pin_y[Net[i][j]-1], Pin_x[Net[i][j+1]-1], Pin_y[Net[i][j+1]-1], 1001, 1001);

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
	
	int netc = 0;
	int netk = 0;
	for (int i = 0; i < num_nets; i++){
		cout << "Net " << i+1 << endl;

		int fla = 0;

		for (int j = 1; j < 20; j++){
			if (Net[i][j]>0){
				for (int k = netk; k < netX.size(); k++){
					if (!((Pin_x[Net[i][j]-1] == netX[k]) && (Pin_y[Net[i][j]-1] == netY[k]))){
						if (i == 0){
							cout << netX[k] << " " << netY[k] << " " << netX[k + 1] << " " << netY[k + 1] << endl;
						}
						else{
							if (fla>0){
								cout << netX[k] << " " << netY[k] << " " << netX[k + 1] << " " << netY[k + 1] << endl;
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


	for (int i = 0; i<n; i++)
	{
		delete[] Rgraph[i];
	}
	delete[] Rgraph;


}