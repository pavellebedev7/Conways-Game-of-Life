#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>

#define PLANE_SIZE 10
#define WINDOW_SIZE 3
#define PLANE vector<vector<bool> >
#define uint uint32_t
#define DELAY 200

using namespace std;

void Clear(){
    system("cls");
}

void Delay(uint time=1000){
    chrono::milliseconds t(time);
    this_thread::sleep_for(t);
}

void PrintStep(const PLANE &grid){
    for(uint i = 0; i < PLANE_SIZE; i++){
        for(uint j = 0; j < PLANE_SIZE; j++){
            if(grid[i][j])cout << "+";
            else cout << ".";
        }
        cout << "\n";
    }
}

void SetToZero(PLANE &grid){
    for(uint i = 0; i < PLANE_SIZE; i++){
        for(uint j = 0; j < PLANE_SIZE; j++){
            grid[i][j] = 0;
        }
    }
}

uint FindNeighbours(PLANE &x, uint i, uint j){
    uint neighbours = 0;
    PLANE w(WINDOW_SIZE, vector<bool>(WINDOW_SIZE, 0));
    for(uint k = 0; k < WINDOW_SIZE; k++){
        for(uint n = 0; n < WINDOW_SIZE; n++){
            if((!i && !k) || (!j && !n) || (i == PLANE_SIZE-1 && k == WINDOW_SIZE-1) || (j == PLANE_SIZE-1 && n == WINDOW_SIZE-1)){ //if(i == 0 || j == 0 || i == PLANE_SIZE || j == PLANE_SIZE){
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
    for(uint i = 0; i < PLANE_SIZE; i++){
        for(uint j = 0; j < PLANE_SIZE; j++){
            neighbours = FindNeighbours(x, i, j);
            if(x[i][j] == 0 && neighbours == 3){
                y[i][j] = 1;
            }
            if(x[i][j] == 1 && (neighbours == 2 || neighbours == 3)){
                y[i][j] = 1;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    // Previous and current states
    PLANE grid0(PLANE_SIZE, vector<bool>(PLANE_SIZE, 0));
    PLANE grid1(PLANE_SIZE, vector<bool>(PLANE_SIZE, 0));

    // Stable figure
    grid0[0][PLANE_SIZE-2] = 1;
    grid0[1][PLANE_SIZE-2] = 1;
    grid0[2][PLANE_SIZE-2] = 1;

    // Moving figure
    grid0[0][1] = 1;
    grid0[1][2] = 1;
    grid0[2][0] = 1;
    grid0[2][1] = 1;
    grid0[2][2] = 1;

    while(1){
        PrintStep(grid0);
        Delay(DELAY);
        Clear();
        Simulate(grid0, grid1);
        grid0 = grid1;
        SetToZero(grid1);
    }
}

