#include <iostream>
#include <thread>
#include <vector>
#include <ctime>
#include <curses.h>

#define PLANE_WIDTH 80
#define PLANE_HEIGHT 20
#define WINDOW_SIZE 3
#define PLANE vector<vector<bool> >
#define uint uint32_t
#define DELAY 100

using namespace std;

// run flag
int run = 1;
// Previous and current states
PLANE grid0(PLANE_HEIGHT, vector<bool>(PLANE_WIDTH, 0));
PLANE grid1(PLANE_HEIGHT, vector<bool>(PLANE_WIDTH, 0));

void PrintStep(const PLANE &grid){
    for(uint i = 0; i < PLANE_HEIGHT; i++){
        for(uint j = 0; j < PLANE_WIDTH; j++){
	    move(i, j);
            if(grid[i][j]){
		addch('#');
	    }
            else{
		addch('.');
	    } 
        }
    }
    refresh();
}

void SetToZero(PLANE &grid){
    for(uint i = 0; i < PLANE_HEIGHT; i++){
        for(uint j = 0; j < PLANE_WIDTH; j++){
            grid[i][j] = 0;
        }
    }
}

uint FindNeighbours(PLANE &x, uint i, uint j){
    uint neighbours = 0;
    PLANE w(WINDOW_SIZE, vector<bool>(WINDOW_SIZE, 0));
    for(uint k = 0; k < WINDOW_SIZE; k++){
        for(uint n = 0; n < WINDOW_SIZE; n++){
            if((!i && !k) || (!j && !n) || (i == PLANE_HEIGHT-1 && k == WINDOW_SIZE-1) || (j == PLANE_WIDTH-1 && n == WINDOW_SIZE-1)){
                continue;
            }
            else{
                w[k][n] = x[i-1+k][j-1+n];
            }
            if(w[k][n] && !(k==1 && n==1))neighbours++;
        }
    }
    return neighbours;
}

void Simulate(PLANE &x, PLANE &y){
    uint neighbours;
    for(uint i = 0; i < PLANE_HEIGHT; i++){
        for(uint j = 0; j < PLANE_WIDTH; j++){
            neighbours = FindNeighbours(x, i, j);
            if(x[i][j] == 0 && neighbours == 3){
                y[i][j] = 1;
            }
            if(x[i][j] == 1 && (neighbours == 2 || neighbours == 3)){
                y[i][j] = 1;
            }
        }
    }
}

void Start(){
	initscr();
	savetty();
	nonl();
	cbreak();
	noecho();
	timeout(0);
	leaveok(stdscr, TRUE);
	curs_set(0);
}

void Stop(){
	curs_set(1);
	clear();
	refresh();
	resetty();
	endwin();
	exit(0);
}

void Control(){
	int keypress = wgetch(stdscr);
	switch(keypress){
		case 'q':// exit
			Stop();
			break;
		case ' ':// start/stop
		case 's':
			if(run)run = 0;
			else run = 1;
			break;
		case 'r':// edit
			break;
		default:
			Start();
			PrintStep(grid0);
			napms(DELAY);
        		Simulate(grid0, grid1);
			if(run){
				clear();
        			grid0 = grid1;
        			SetToZero(grid1);
			}
			break;
	}
}

void AddBlinker(int x, int y){
	grid0[y][x+0] = 1;
	grid0[y][x+1] = 1;
	grid0[y][x+2] = 1;
}

void AddGlider(int x, int y){
	grid0[y+1][x+0] = 1;
	grid0[y+2][x+1] = 1;
	grid0[y+0][x+2] = 1;
	grid0[y+1][x+2] = 1;
	grid0[y+2][x+2] = 1;
}

void AddBeacon(int x, int y){
	grid0[y+0][x+0] = 1;
	grid0[y+0][x+1] = 1;
	grid0[y+1][x+0] = 1;
	grid0[y+1][x+1] = 1;
	grid0[y+2][x+2] = 1;
	grid0[y+3][x+2] = 1;
	grid0[y+2][x+3] = 1;
	grid0[y+3][x+3] = 1;
}

void AddLWSS(int x, int y){
	grid0[y+0][x+1] = 1;
	grid0[y+0][x+4] = 1;
	grid0[y+1][x+0] = 1;
	grid0[y+2][x+0] = 1;
	grid0[y+2][x+4] = 1;
	grid0[y+3][x+0] = 1;
	grid0[y+3][x+1] = 1;
	grid0[y+3][x+2] = 1; 
	grid0[y+3][x+3] = 1;     
}

int main()
{
    AddBlinker(PLANE_WIDTH-3, 1);
    AddBeacon(60, 10);
    AddGlider(0, 0);
    AddLWSS(10, 10);    
    while(1){
	Control();
    }
}
