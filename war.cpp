#include <iostream>
#include <queue>

// https://www.codingame.com/training/medium/winamax-battle

using namespace std;

int get_card_val(char c) {
    switch(c){
        case '1' : return 10;
        case 'J' : return 11;
        case 'Q' : return 12;
        case 'K' : return 13;
        case 'A' : return 14;
        default: return c-48;
    }
}

int main()
{
    queue<char> p1cards;
    queue<char> p2cards;
    int n; // the number of cards for player 1
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        char card_val; //value of card
        string card;
        cin >> card; cin.ignore();
        card_val = get_card_val(card[0]);
        p1cards.push(card_val);
    }
    int m; // the number of cards for player 2
    cin >> m; cin.ignore();
    for (int i = 0; i < m; i++) {
        char card_val;
        string card;
        cin >> card; cin.ignore();
        card_val = get_card_val(card[0]);
        p2cards.push(card_val);
    }
    int rounds = 0;
    while(p1cards.size() > 0 && p2cards.size() > 0) {
        queue<char> *winner;
        if (p1cards.front() > p2cards.front()) {
            winner = &p1cards;
        }
        else if (p2cards.front() > p1cards.front()) {
            winner = &p2cards;
        }
        else {
            bool x = true;
            queue<char> p1_stack;
            queue<char> p2_stack; 
            while (x) {
                for (char i = 0; i < 3; i++) {
                    p1_stack.push(p1cards.front());
                    p1cards.pop();
                    p2_stack.push(p1cards.front());
                    p2cards.pop();
                }
                if (p1cards.front() > p2_stack.front()) {
                    winner = &p1cards;
                    x = false;
                }
                else if (p2cards.front() > p1_stack.front()) {
                    winner = &p2cards;
                    x = false;
                }
                else {
                    p1_stack.push(p1cards.front());
                    p1cards.pop();
                    p2_stack.push(p1cards.front());
                    p2cards.pop();
                }
            }
            for (int i = 0; i < p1_stack.size(); i ++) {
                winner->push(p1_stack.front());
            }
            for (int i = 0; i < p2_stack.size(); i ++) {
                winner->push(p1_stack.front());
            }
        }
        winner->push(p1cards.front());
        winner->push(p2cards.front());
        p1cards.pop();
        p2cards.pop();
        //cerr << p1cards.size() << " " << p2cards.size() << endl;
        rounds++;
    }
    if (p1cards.size() > 0) {
        cout << 1 << " " << rounds << endl;
    }
    else {
        cout << 2 << " " << rounds << endl;
    }
}