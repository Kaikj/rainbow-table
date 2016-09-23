/*--------
(1) Read in a Rainbow table (built using B.cpp)
(2) Read 1000 digests from standard input and output the preimage.
------------*/

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "sha1.h"
#include "rainbow.hpp"

using namespace std;
unsigned long TOTAL_SHA=0;       // Count the number of hashes performed.

unsigned char M[1024*512][3];    // array to store the word read from the table (head of chain)
unsigned int  D[1024*512][5];    // array to store the digest read from the table  (end of chain)

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


//------------  Read in the Table ------------------//
//   Store the result in M and D                    //
int ReadT() {
    ifstream infile;
    infile.open("rainbow_table");
    infile.setf(ios::hex,ios::basefield);    // format the output to be hex
    infile.setf(ios::uppercase);

    int i = 0;
    unsigned int input;

    while (true) {

        infile >> hex >> input;

        // Check the first input
        if (infile.eof()) {
            break;
        }

        M[i][0] = (unsigned char) input;
        infile >> hex >> input;
        M[i][1] = (unsigned char) input;
        infile >> hex >> input;
        M[i][2] = (unsigned char) input;
        struct Text t = {
            M[i][0],
            M[i][1],
            M[i][2]
        };

        infile >> hex >> D[i][0];
        infile >> hex >> D[i][1];
        infile >> hex >> D[i][2];
        infile >> hex >> D[i][3];
        infile >> hex >> D[i][4];
        struct Digest d = {
            D[i][0],
            D[i][1],
            D[i][2],
            D[i][3],
            D[i][4],
        };

        std::pair<Digest, Text> p (d, t);
        HashTable.insert(p);
        i++;
    }

    infile.close();
    return(0);
}


//------------------------------------------------------------------------------------
//      Given a digest,  search for the pre-image   answer_m[3].
//------------------------------------------------------------------------------------
int search(unsigned int target_d[5], unsigned char answer_m[3]) {
    unsigned int j, i;
    unsigned char Colour_m[Rainbow::CHAIN_LEN][3];
    unsigned int  Colour_d[Rainbow::CHAIN_LEN][5];
    // unsigned int  flag    [Rainbow::CHAIN_LEN];

    for (j = 0; j < Rainbow::CHAIN_LEN; j++) {
        Colour_d[j][0] = target_d[0];
        Colour_d[j][1] = target_d[1];
        Colour_d[j][2] = target_d[2];
        Colour_d[j][3] = target_d[3];
        Colour_d[j][4] = target_d[4];
    }

    for (j = 0; j < Rainbow::CHAIN_LEN; j++) {
        for (int k = 0; k < (j + 1); k++) {
            Rainbow::reduce(Colour_d[k], Colour_m[k], j);
            Rainbow::hash(Colour_m[k], Colour_d[k]);

            //-------- search for the digest Colour_d[k] in the data structure. 
            struct Digest d = {
                Colour_d[k][0],
                Colour_d[k][1],
                Colour_d[k][2],
                Colour_d[k][3],
                Colour_d[k][4],
            };
            G = HashTable.find(d);

            //-------- if found, call transverse the chain starting from the head to find the pre-image.
            if (G != HashTable.end()) {
                answer_m[0] = G->second.M[0];
                answer_m[1] = G->second.M[1];
                answer_m[2] = G->second.M[2];
                unsigned int d[5];
                for (int n = 0; n < Rainbow::CHAIN_LEN; n++) {
                    Rainbow::hash(answer_m, d);
                    if (d[0] == target_d[0]
                        && d[1] == target_d[1]
                        && d[2] == target_d[2]
                        && d[3] == target_d[3]
                        && d[4] == target_d[4]) {
                        cout << "Found!" << endl;
                        return true;
                    }
                    Rainbow::reduce(d, answer_m, n);
                }
            }
        }
    }

    return 0;
}


//-----------   reading the next digest from the standard input  ----------------//
void readnextd (unsigned int d[5]) {
    cin.setf(ios::hex,ios::basefield); cin.setf(ios::uppercase);
    cin >> d[0]; cin >> d[1]; cin >> d[2]; cin >> d[3]; cin >> d[4];
}


int main(int argc, char*argv[]) {
    int found;
    int total_found;
    int total_not_found;

    SHA1        sha;
    unsigned int d[5];   // 32 x 5 = 160 bits
    unsigned char m[3];   // 24-bit word

    //------------ R E A D     R A I N B O W    T A B L E  --------//
    ReadT();       cout << "READ DONE" << endl;


    //--------  PROJECT  INPUT/OUTPUT FORMAT ----------------//

    total_found=0;
    total_not_found=0;

    cout.setf(ios::hex,ios::basefield);       //   setting display to Hexdecimal format. (this is the irritating part of using C++).
    cout.setf(ios::uppercase);

    for (int i = 0; i < 5000; i++) {
        readnextd(d);
        if (::search (d, m)) {
            total_found ++;
            //------   print the word in hexdecimal format   -----------
            cout << setw(1) << (unsigned int)m[0]/16;
            cout << setw(1) << (unsigned int)m[0]%16;
            cout << setw(1) << (unsigned int)m[1]/16;
            cout << setw(1) << (unsigned int)m[1]%16;
            cout << setw(1) << (unsigned int)m[2]/16;
            cout << setw(1) << (unsigned int)m[2]%16 << endl;
        } else {
            total_not_found ++;
            cout << setw(6)<< 0 << endl;
        }
    }

    cout.setf(ios::dec);
    cout << "Accuracy       C is: " << total_found/5000.0 << endl;
    cout << "Speedup factor F is: " << (5000.0/Rainbow::get_total_sha())*8388608 << endl;
}

