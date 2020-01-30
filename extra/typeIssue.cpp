///////////////////////////////////////////////////////////////
//  c++ type stuff
//
// compile and run with:
//
//  ``` g++ typeIssue.cpp -o typeIssue && ./typeIssue ```
///////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

void all_ints() {
    cout << "\n Using all ints... \n";
    int rmin = 0;
    int rmax = 1024;
    for (int i = rmin; i <= rmax; i+=100) {
        float val = (i - rmin) / (rmax - rmin) * 254;
        cout << val;
        cout << "\n";
    }
}

void float_iterator() {
    cout << "\n Using ints with a float iterator... \n";
    int rmin = 0;
    int rmax = 1024;
    for (float i = rmin; i <= rmax; i+=100) {
        int val = (i - rmin) / (rmax - rmin) * 254;
        cout << val;
        cout << "\n";
    }
}

void all_floats() {
    cout << "\n Using all floats... \n";
    float rmin = 0;
    float rmax = 1024;
    for (float i = rmin; i <= rmax; i+=100) {
        float val = (i - rmin) / (rmax - rmin) * 254;
        cout << val;
        cout << "\n";
    }
}

int main() {
    all_ints();
    float_iterator();
    all_floats();
}