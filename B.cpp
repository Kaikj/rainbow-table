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

using namespace std;

//  A table to store all the words and digests.
//    infeasible to have such large table in practice.
//    for programming convenient, we store the whole table in memory.
unsigned char M[Rainbow::MAX_SIZE][3];
unsigned int  D[Rainbow::MAX_SIZE][5];

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

    for (int i = 0; i < Rainbow::CHAIN_NUM; i++) {
        Rainbow::next_word(M[i]);

        // build the chain.
        Rainbow::hash(M[i], d);

        for (int j = 0; j < Rainbow::CHAIN_LEN - 1; j++) {
            Rainbow::reduce(d, m, j);
            Rainbow::hash(m, d);
        }

        struct Digest di = {
            d[0],
            d[1],
            d[2],
            d[3],
            d[4],
        };
        G = HashTable.find(di);

        // check whether to keep the chain.
        // You may want to drop the chain, for e.g. if the digest is already in the table.
        // This form the main component of your program.

        // The digest already exists. Throw the chain away
        if (G != HashTable.end()) {
            i--;
        } else {
            struct Text t = {
                M[i][0],
                M[i][1],
                M[i][2]
            };
            std::pair<Digest, Text> p (di, t);
            HashTable.insert(p);

            outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][0];
            outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][1];
            outfile << " " << setw(2) << setfill('0') << (unsigned int) M[i][2];

            outfile << " " << setw(8) << setfill('0') << d[0];          // setw(8) set the width to be 8.
            outfile << " " << setw(8) << setfill('0') << d[1];
            outfile << " " << setw(8) << setfill('0') << d[2];
            outfile << " " << setw(8) << setfill('0') << d[3];
            outfile << " " << setw(8) << setfill('0') << d[4];

            outfile << endl;
        }
    }

    outfile.close();
    return(0);
}

int main(int argc, char*argv[]) {

    SHA1        sha;

    //----  Setting the parameters

    //----   Build the table.
    buildT();
}

