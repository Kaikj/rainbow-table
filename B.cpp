/*----------------------------------
This program builds the rainbow table for the other program F.cpp.
------------------------------------*/

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include "sha1.h"

using namespace std;
#define HT 75000
#define N_CHAIN 1048676


//  A table to store all the words and digests.
//    infeasible to have such large table in practice.
//    for programming convenient, we store the whole table in memory.
unsigned char M[1048576][3];
unsigned int  D[1048576][5];

unordered_map <unsigned long, unsigned int> HashTable;
unordered_map <unsigned long, unsigned int>::const_iterator G;

//----  return the next word to be considered 
void next_word(unsigned char m[3]) {
    //   fill in the detail.
}


//-------   Hash
int hash(unsigned char m[3], unsigned int d[5]) {
    SHA1 sha;
    sha.Reset(); sha.Input(m[0]); sha.Input(m[1]); sha.Input(m[2]);
    sha.Result(d);
    return(0);
}

//-------  Reduce
int reduce(unsigned int d[5], unsigned char m[3],  int i ) {
    m[0] = (unsigned char)((d[0] + i) %256);  //8 bits
    m[1] = (unsigned char)((d[1]   ) %256);   //8 bits
    m[2] = (unsigned char)((d[2]   ) %256);   //8 bits

    return(0);
}

int buildT() {
    unsigned int  d[5];
    unsigned char m[3];

    ofstream outfile;
    outfile.open("rainbow_table");
    outfile.setf(ios::hex,ios::basefield);    // format the output to be hex
    outfile.setf(ios::uppercase);

    int chain_len = pow(2, 24)/HT;

    for (int i = 0; i < HT; i++) {
        next_word(m);

        // build the chain.
        ::hash(M[i], d);

        for (int j = 0; j < chain_len - 1; j++) {
            ::reduce(d, m, i);

            if (j < (chain_len - 2)) {
                ::hash(m, d);
            }
        }

        ::hash(m, D[i]);

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

