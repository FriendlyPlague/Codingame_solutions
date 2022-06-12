#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
using namespace std;

// https://www.codingame.com/training/medium/blunder-episode-1

struct Bender {
    char dir; 
    char x;
    char y;
    bool breaker;
};
int turns = 0;
int main()
{
    string output;
    int L;
    int C;
    array<char, 4> dir_order = {'S','E','N','W'};
    array<char, 4> rdir_order = {'W','N','E','S'};
    map<char, string> dirW = {
        {'N',"NORTH"},
        {'E',"EAST"},
        {'S',"SOUTH"},
        {'W',"WEST"}
    };
    Bender b {'S',0,0,false};
    vector<string> cmap;
    cin >> L >> C; cin.ignore();
    // Loop creates 2d envirement and finds start position
    for (char i = 0; i < L; i++) {
        string row;
        getline(cin, row);
        for (char j = 0; j < C; j++) {
            if (row[j] == '@') {
                b.x = j;
                b.y = i;
            }
            //cerr << row[j];
        }
        //cerr << endl;
        cmap.push_back(row);
    }
    struct Front_obj {
        char obj;
        char x;
        char y;
    };
    char ind = 0;
    bool moving = true;
    while(moving) {
        start:
        Front_obj fo;
        // Switch finds what's in front of the robot
        switch(b.dir){
            case 'N':
                fo.obj = cmap[b.y-1][b.x];
                fo.x = b.x; fo.y = b.y-1;
                break;
            case 'S':
                fo.obj = cmap[b.y+1][b.x];
                fo.x = b.x; fo.y = b.y+1;
                break;
            case 'E':
                fo.obj = cmap[b.y][b.x+1];
                fo.x = b.x+1; fo.y = b.y;
                break;
            case 'W':
                fo.obj = cmap[b.y][b.x-1];
                fo.x = b.x-1; fo.y = b.y;
        }
        switch(fo.obj) {
            case '#':
                b.dir = dir_order[ind];
                ind++;
                goto start;
                break;
            case 'X':
                if (b.breaker == false) {
                    b.dir = dir_order[ind];
                    ind++;
                    goto start;
                }
                else {cmap[fo.y][fo.x] = ' ';}
                break;
            case 'I':
                swap(dir_order,rdir_order);
                break;
            case 'B':
                if (b.breaker) {b.breaker = false;}
                else {b.breaker = true;}
                break;
            case 'T':
                for (char i = 1; i < L; i++) {
                    for (char j = 1; j < C; j++) {
                        if (cmap[i][j] == 'T' && (fo.x != j || fo.y != i)) {
                            cerr << "Test" << endl;
                            output += dirW[b.dir];
                            output += "\n";
                            b.x = j; b.y = i;
                            goto start;
                        }
                    }
                }
                break;
            case '$':
                moving = false;
                break;
            case ' ':
                break;
            case '@':
                cout << "LOOP";
                return 0;
                break;
            default:
                output += dirW[b.dir];
                b.x = fo.x; b.y = fo.y;
                b.dir = fo.obj;
                output += "\n";
                goto start;
                break;
        }
        turns++;
        ind = 0;
        output += dirW[b.dir];
        output += "\n";
        b.x = fo.x; b.y = fo.y;
        if (turns > (L-1)*(C-1)) {
            output = "LOOP";
            moving = false;
        }
    }
    cout << output;
}