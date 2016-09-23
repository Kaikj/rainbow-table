/**
 * rainbow.hpp
 *
 * Description:
 *      This class contains utility functions used in
 *      the construction of a rainbow table.
 *
 */

#ifndef _RAINBOW_H_
#define _RAINBOW_H_

namespace Rainbow {

    static const unsigned int CHAIN_NUM = 75000;
    static const unsigned int CHAIN_LEN = 224;
    static const unsigned int MAX_SIZE = 1048576;
    static const unsigned int TABLE_NUM = 1;

    static unsigned long TOTAL_SHA = 0;

    unsigned long get_total_sha();

    //-----   H A S H      ------------------------------------//
    //   getrandom (m) :
    //      generate a random word. 
    //---------------------------------------------------------//
    int get_random(unsigned char m[3]);

    //-----   H A S H      ------------------------------------//
    //   Hash (m, d) :
    //      compute the hash of m. The digest is stored in d.
    //---------------------------------------------------------//
    int hash(unsigned char m[3], unsigned int d[5]);

    //----  return the next word to be considered
    void next_word(unsigned char m[3]);

    //-----   R E D U C E  -----------------------------//
    //   d:   input digest
    //   m:   output word
    //   i:   the index of the reduce function
    //---------------------------------------------------//
    int reduce(unsigned int d[5], unsigned char m[3], int i);
}

#endif