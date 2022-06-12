#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// https://www.codingame.com/training/medium/conway-sequence

using namespace std;

int main()
{
    int R;
    cin >> R; cin.ignore();
    int L;
    cin >> L; cin.ignore();
    vector<int> sequence = {R};
    for (int i = 1; i < L; i++) {
        int p_num = sequence[0];
        int num_count = 1;
        vector<int> new_sequence;
        for (int j = 1; j < sequence.size(); j++) {
            if (p_num == sequence[j]) {
                num_count += 1;
            }
            else {
                new_sequence.push_back(num_count);
                new_sequence.push_back(p_num);
                p_num = sequence[j];
                num_count = 1;
            }
        }
        new_sequence.push_back(num_count);
        new_sequence.push_back(p_num);
        sequence = new_sequence;
    }
    for (int i = 0; i < sequence.size()-1; i++) {
        cout << int(sequence[i]) << ' ';
    }
    cout << sequence[sequence.size()-1];
    cout << endl;
}