#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

typedef struct Test {

    int num[6];
    int goal;

    void generateTest() {
        for (int i = 0; i < 4; i++)
            num[i] = rand() % 9 + 1;
        num[4] = (rand() % 3) * 5 + 10;
        num[5] = (rand() % 4) * 25 + 25;
        goal = rand() % 999 + 1;
    }


    string toString() {

        string s = "";
        for (int i = 0; i < 6; i++) {
            s += to_string(num[i]);
            s += " ";
        } s += to_string(goal);
        return s;
    }

} Test;

typedef struct State {

    int val;
    string exp;
    vector <int> unused;

    bool operator < (const State& other) const { return false; }

} State;


string solveTestBFS(Test test) {

    queue <State> q;
    for (int i = 0; i < 6; i++) {

        State cur;
        cur.val = test.num[i];
        cur.exp = to_string(test.num[i]);
        for (int j = 0; j < 6; j++) {
            if (i == j) continue;
            cur.unused.push_back(test.num[j]);
        }

        if (cur.val == test.goal)
            return cur.exp + " = " + to_string(cur.val);
        q.push(cur);
    }

    while (!q.empty()) {

        State cur = q.front();
        q.pop();
        int unusedCount = cur.unused.size();
        for (int i = 0; i < unusedCount; i++) {
            for (int j = 0; j < 4; j++) {

                State next;
                bool divisible = true;
                switch(j) {

                    case 0: {
                        next.val = cur.val + cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " +";
                        break;
                    }

                    case 1: {
                        next.val = cur.val - cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " -";
                        break;
                    }

                    case 2: {
                        next.val = cur.val * cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " *";
                        break;
                    }

                    case 3: {
                        divisible = cur.val % cur.unused[i] == 0;
                        next.val = cur.val / cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " /";
                        break;
                    }
                }

                for (int k = 0; k < unusedCount; k++) {
                    if (i == k) continue;
                    next.unused.push_back(cur.unused[k]);
                }

                if (divisible) {
                    if (next.val == test.goal)
                        return next.exp + " = " + to_string(next.val);

                    if (unusedCount > 1)
                        q.push(next);
                }
            }
        }

    }

    return "No solution" ;
}


string solveTestBeamSearch(Test test) {

    queue <State> q;
    // ovde moze da se menja k za pretragu po snopu
    // prvi put kad bude imao dovoljno u redu pokrece pretragu
    // da bih izbegao generisanje k random stanja sa uniformnom raspodelom
    int k = 50;
    for (int i = 0; i < 6; i++) {

        State cur;
        cur.val = test.num[i];
        cur.exp = to_string(test.num[i]);
        for (int j = 0; j < 6; j++) {
            if (i == j) continue;
            cur.unused.push_back(test.num[j]);
        }

        if (cur.val == test.goal)
            return cur.exp + " = " + to_string(cur.val);
        q.push(cur);
    }

    while (!q.empty() && q.size() < k) {

        State cur = q.front();
        q.pop();
        int unusedCount = cur.unused.size();
        for (int i = 0; i < unusedCount; i++) {
            for (int j = 0; j < 4; j++) {

                State next;
                bool divisible = true;
                switch(j) {

                    case 0: {
                        next.val = cur.val + cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " +";
                        break;
                    }

                    case 1: {
                        next.val = cur.val - cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " -";
                        break;
                    }

                    case 2: {
                        next.val = cur.val * cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " *";
                        break;
                    }

                    case 3: {
                        divisible = cur.val % cur.unused[i] == 0;
                        next.val = cur.val / cur.unused[i];
                        next.exp = cur.exp + " " + to_string(cur.unused[i]) + " /";
                        break;
                    }
                }

                for (int k = 0; k < unusedCount; k++) {
                    if (i == k) continue;
                    next.unused.push_back(cur.unused[k]);
                }

                if (divisible) {
                    if (next.val == test.goal)
                        return next.exp + " = " + to_string(next.val);

                    if (unusedCount > 1) {
                        q.push(next);
                    }
                }
            }
        }
    }

    State best;
    best.exp = "";
    best.val = -1;

    while (!q.empty()) {

        vector <State> adjacent;
        while (!q.empty()) {

            State cur = q.front();
            q.pop();
            int unusedCount = cur.unused.size();
            for (int i = 0; i < unusedCount; i++) {
                for (int j = 0; j < 4; j++) {

                    State next;
                    bool divisible = true;
                    switch(j) {

                        case 0: {
                            next.val = cur.val + cur.unused[i];
                            next.exp = cur.exp + " " + to_string(cur.unused[i]) + " +";
                            break;
                        }

                        case 1: {
                            next.val = cur.val - cur.unused[i];
                            next.exp = cur.exp + " " + to_string(cur.unused[i]) + " -";
                            break;
                        }

                        case 2: {
                            next.val = cur.val * cur.unused[i];
                            next.exp = cur.exp + " " + to_string(cur.unused[i]) + " *";
                            break;
                        }

                        case 3: {
                            divisible = cur.val % cur.unused[i] == 0;
                            next.val = cur.val / cur.unused[i];
                            next.exp = cur.exp + " " + to_string(cur.unused[i]) + " /";
                            break;
                        }
                    }

                    for (int k = 0; k < unusedCount; k++) {
                        if (i == k) continue;
                        next.unused.push_back(cur.unused[k]);
                    }

                    if (divisible) {
                        if (next.val == test.goal)
                            return next.exp + " = " + to_string(next.val);

                        adjacent.push_back(next);

                    }
                }
            }
        }

        vector <pair <int, State> > wrapper;
        for (int i = 0; i < adjacent.size(); i++) {
            wrapper.push_back({abs(adjacent[i].val - test.goal), adjacent[i]});
        }

        // uzima opet k najboljih, sortira po daljini od cilja
        if (wrapper.size())
        {
            sort(wrapper.begin(), wrapper.end());
            best = wrapper[0].second;
            for (size_t i = 0; i < min(wrapper.size(), (size_t) k); i++) {
                q.push(wrapper[i].second);
            }
        }
    }



    return best.exp + " = " + to_string(best.val);;
}

void startInteractiveInput() {

    Test test;
    for (int i = 0; i < 6; i++)
        cin >> test.num[i];
    cin >> test.goal;
    cout << solveTestBFS(test);
}

void startInteractiveAuto() {

    Test test;
    test.generateTest();
    cout << test.toString() << endl;
    cout << solveTestBFS(test);
}

void startTest() {

    int t;
    cout << "Choose number of tests: ";
    cin >> t;
    auto test = vector <Test>(t);
    auto output1 = vector <string>(t);
    auto output2 = vector <string>(t);
    ofstream os;
    os.open("test.txt");
    for (int i = 0; i < t; i++) {
        test[i].generateTest();
        output1[i] = solveTestBFS(test[i]);
        output2[i] = solveTestBeamSearch(test[i]);
        os << test[i].toString() << "\n" << output1[i] << "\n" << output2[i] << "\n\n";
    }
    os.close();
}


void start() {

    cout << "Type in number to choose mode: " << endl
         << "1) Interactive with test input" << endl
         << "2) Interactive with automatic test" << endl
         << "3) Test" << endl;

    int mode;
    cin >> mode;
    switch (mode) {
        case 1: {startInteractiveInput(); break;}
        case 2: {startInteractiveAuto(); break;}
        case 3: {startTest(); break;}
    }
}

int main() {
    srand(time(NULL));
    start();
    return 0;
}

