/**
 * rainbow.cpp
 *
 * This program holds utility methods required
 * for the building of the rainbow table.
 *
 */

#include <iostream>
#include <cstdlib>
#include "rainbow.hpp"
#include "sha1.h"

//-----   H A S H      ------------------------------------//
//   get_random(m) :
//      Generate a random word. 
//---------------------------------------------------------//
int Rainbow::get_random(unsigned char m[3]) {
    m[0] = (unsigned char) (random() % 256);
    m[1] = (unsigned char) (random() % 256);
    m[2] = (unsigned char) (random() % 256);

    return 0;
}

//-----   H A S H      ------------------------------------//
//   hash(m, d) :
//      Compute the hash of m. The digest is stored in d.
//---------------------------------------------------------//
int Rainbow::hash(unsigned char m[3], unsigned int d[5]) {
    SHA1 sha;

    sha.Reset();

    sha.Input(m[0]);
    sha.Input(m[1]);
    sha.Input(m[2]);

    sha.Result(d);
    return(0);
}

//----  return the next word to be considered
void Rainbow::next_word(unsigned char m[3]) {
    Rainbow::get_random(m);
}

//-----   R E D U C E  -----------------------------//
//   d:   input digest
//   m:   output word
//   i:   the index of the reduce function
//---------------------------------------------------//
int Rainbow::reduce(unsigned int d[5], unsigned char m[3], int i) {
    m[0] = (unsigned char)((d[0]+i ) %256);   //8 bits
    m[1] = (unsigned char)((d[1]   ) %256);   //8 bits
    m[2] = (unsigned char)((d[2]   ) %256);   //8 bits

    return(0);
}