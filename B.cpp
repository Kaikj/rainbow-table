/*----------------------------------
This program builds the rainbow table for the other program F.cpp.
------------------------------------*/

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include "sha1.h"
#include "rainbow.hpp"

#define HT 75000
#define N_CHAIN 1048676

using namespace std;

//  A table to store all the words and digests.
//    infeasible to have such large table in practice.
//    for programming convenient, we store the whole table in memory.
unsigned char M[1048576][3];
unsigned int  D[1048576][5];

struct Digest {
    unsigned int D[5];

    bool operator==(const Digest &other) const {
        return (D[0] == other.D[0]
            && D[1] == other.D[1]
            && D[2] == other.D[2]
            && D[3] == other.D[3]
            && D[4] == other.D[4]);
    }
};

namespace std {
    template <>
    struct hash<Digest> {
        std::size_t operator()(const Digest &k) const {
            std::stringstream ss;
            ss << k.D[0];
            ss << k.D[1];
            ss << k.D[2];
            ss << k.D[3];
            ss << k.D[4];

            return std::hash<string>() (ss.str());
    }
  };
}

struct Text {
    unsigned char M[3];

    bool operator==(const Text &other) const {
        return (M[0] == other.M[0]
            && M[1] == other.M[1]
            && M[2] == other.M[2]);
    }
};

//-------   Data Structure for searching    -----------//
unordered_map <Digest, Text> HashTable;
unordered_map <Digest, Text>::const_iterator G;

int buildT() {
    unsigned int  d[5];
    unsigned char m[3];

    ofstream outfile;
    outfile.open("rainbow_table");
    outfile.setf(ios::hex,ios::basefield);    // format the output to be hex
    outfile.setf(ios::uppercase);

    int chain_len = pow(2, 24)/HT;

    for (int i = 0; i < HT; i++) {
        Rainbow::next_word(M[i]);

        // build the chain.
        Rainbow::hash(M[i], d);

        for (int j = 0; j < chain_len - 1; j++) {
            Rainbow::reduce(d, m, i);

            if (j < (chain_len - 2)) {
                Rainbow::hash(m, d);
            }
        }

        Rainbow::hash(m, D[i]);

        outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][0];
        outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][1];
        outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][2];

        outfile << " " << setw(8) << setfill('0') << d[0];          // setw(8) set the width to be 8.
        outfile << " " << setw(8) << setfill('0') << d[1];
        outfile << " " << setw(8) << setfill('0') << d[2];
        outfile << " " << setw(8) << setfill('0') << d[3];
        outfile << " " << setw(8) << setfill('0') << d[4];

        outfile << endl;

        // check whether to keep the chain.
        // You may want to drop the chain, for e.g. if the digest is already in the table.
        // This form the main component of your program.
    }


    //---    Write to the output file
    //note that to reduce the size of the table, it is not neccessary to write the full digest.

    // for (long i = 0; i < N_CHAIN; i++) {
    //     fwrite(&(M[i][0]), sizeof( unsigned char), 1, stdout);
    //     fwrite(&(M[i][1]), sizeof( unsigned char), 1, stdout);
    //     fwrite(&(M[i][2]), sizeof( unsigned char), 1, stdout);

    //     fwrite(&(D[i][0]), sizeof( unsigned int), 1, stdout);
    //     fwrite(&(D[i][1]), sizeof( unsigned int), 1, stdout);
    //     fwrite(&(D[i][2]), sizeof( unsigned int), 1, stdout);
    //     fwrite(&(D[i][3]), sizeof( unsigned int), 1, stdout);
    //     fwrite(&(D[i][4]), sizeof( unsigned int), 1, stdout);
    // }
    outfile.close();
    return(0);
}

int main(int argc, char*argv[]) {

    SHA1        sha;

    //----  Setting the parameters

    //----   Build the table.
    buildT();
}

