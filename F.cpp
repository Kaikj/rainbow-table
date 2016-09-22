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

#define HT 75000
#define N_CHAIN 1048676
#define MAX_LEN 224

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


//-----------    Hash     ----------------------------//
int Hash (unsigned char m[3], unsigned int d[5]) {
    SHA1 sha;
    sha.Reset(); sha.Input(m[0]); sha.Input(m[1]); sha.Input(m[2]);
    sha.Result(d);

    TOTAL_SHA = TOTAL_SHA +1;
    return(0);
}

//-----------    Reduce  -----------------------------//
//   d:   input digest
//   m:   output word
//   i:   the index of the reduce function
//---------------------------------------------------//
int Reduce (unsigned int d[5], unsigned char m[3], int i) {
    m[0] = (unsigned char)( (d[0]+i ) %256);   //8 bits
    m[1] = (unsigned char)( (d[1]   ) %256);   //8 bits
    m[2] = (unsigned char)( (d[2]   ) %256);   //8 bits

    return(0);
}


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
    unsigned char Colour_m[MAX_LEN][3];
    unsigned int  Colour_d[MAX_LEN][5];
    // unsigned int  flag    [MAX_LEN];

    for (j = 0; j < MAX_LEN; j++) {
        Colour_d[j][0] = target_d[0];
        Colour_d[j][1] = target_d[1];
        Colour_d[j][2] = target_d[2];
        Colour_d[j][3] = target_d[3];
        Colour_d[j][4] = target_d[4];
    }

    for (j = 0; j < MAX_LEN; j++) {
        for (int k = 0; k < (j + 1); k++) {
            Reduce(Colour_d[k], Colour_m[k], j);
            Hash(Colour_m[k], Colour_d[k]);

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
                for (int n = 0; n < MAX_LEN; n++) {
                    Hash(Colour_m[0], Colour_d[0]);
                    if (Colour_d[0][0] == target_d[0]
                        && Colour_d[0][1] == target_d[1]
                        && Colour_d[0][2] == target_d[2]
                        && Colour_d[0][3] == target_d[3]
                        && Colour_d[0][4] == target_d[4]) {
                        cout << "Found!" << endl;
                        return true;
                    }
                    Reduce(Colour_d[0], answer_m, n);
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
    cout << "Speedup factor F is: " << (5000.0/TOTAL_SHA)*8388608 << endl;
}

