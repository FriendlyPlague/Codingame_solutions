#include <iostream>
#include <string>

// https://www.codingame.com/training/medium/light-bulbs

using namespace std;

string bulbS;
string bulbT;
int numBulbs;

int setBulb(int ind, char val);

int main()
{
    getline(cin, bulbS); // starting bulbs
    getline(cin, bulbT); // target bulbs
    numBulbs = bulbS.length();

    int answer = 0;
    for (int i = 0; i < numBulbs; i++){
        if (bulbS[i] != bulbT[i]) {
            answer += setBulb(i, bulbT[i]);
        }
    }
    cout << answer << endl;
}

int setBulb(int ind, char val) {
    int bulbCount = 0;
    if (ind == numBulbs-1) {
        bulbS[ind] = val;
        return bulbCount + 1;
    }
    if (bulbS[ind+1] != '1') {
        bulbCount += setBulb(ind+1, '1');
    }
    for (int j = ind+2; j < numBulbs; j++) {
        if (bulbS[j] != '0') {
            bulbCount += setBulb(j, '0');
        }
    }
    bulbS[ind] = val;
    return bulbCount + 1;
}