/*--------
    This program generates random digests

to compile:
    g++ Generate.cpp sha1.cpp -o Generate

to run
    ./Generate 0 5000

------------*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "sha1.h"
#include "rainbow.hpp"

using namespace std;

int main(int argc, char*argv[]) {
    SHA1          sha;
    unsigned int  d[5];   // 32 x 5 = 160 bits  digest
    unsigned char m[3];   // 24-bit word

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " seed number [plaintxt.out]" << endl;
        return false;
    }

    int seed = atoi(argv[1]);
    int number = atoi(argv[2]);
    string outfile_name = argv[3] == NULL ? "plaintxt.out" : argv[3];

    srandom(seed);

    ofstream outfile;
    outfile.open(outfile_name.c_str());
    outfile.setf(ios::hex,ios::basefield);    // format the output to be hex
    outfile.setf(ios::uppercase);

    cout.setf(ios::hex,ios::basefield);    // format the output to be hex
    cout.setf(ios::uppercase);

    for (int i = 0; i < number; i++) {
        getrandom(m); Hash(m,d);

        // When generating random number, store the plain text
        outfile << setw(1) << (unsigned int)m[0]/16;
        outfile << setw(1) << (unsigned int)m[0]%16;
        outfile << setw(1) << (unsigned int)m[1]/16;
        outfile << setw(1) << (unsigned int)m[1]%16;
        outfile << setw(1) << (unsigned int)m[2]/16;
        outfile << setw(1) << (unsigned int)m[2]%16 << endl;

        cout << "  " << setw(8) << setfill('0') << d[0]          // setw(8) set the width to be 8.
             << "  " << setw(8) << d[1]
             << "  " << setw(8) << d[2]
             << "  " << setw(8) << d[3]
             << "  " << setw(8) << d[4] << endl;
    }

    outfile.close();
    return true;
}

