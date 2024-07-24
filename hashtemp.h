
#include <iostream>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;


const unsigned scale = 225;


// * * * replace with your own personal modulus * * * //
const unsigned M = 3691;



// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
    private:
        T * A[M];



        // * * * Add attributes and helper methods here if you wish. * * * //
        int helperInsert(char c);
        long long helperPower(int value, int j);

        //unsigned size; // * * * remove this * * * //
        


    public:
        // Desc:  Default constructor.  Initialize table to NULL.
        Set() { 
            memset(A, 0, sizeof(T *)*M); 
            //size = 0;
            for (int i = 0; i < M; i++) {
                A[i] = NULL;
            }
        }


        // Desc:  Insert x into the Set, according to its ->key.
        //        If ->key is already present, then replace it by x.
        //        Collisions are resolved via quadratic probing sequence.
        // Post:  returns the table index where x was inserted
        //        returns -1 if x was not inserted
        int insert(T * x);


        // Desc:  Returns T * x such that x->key == key, if and only if 
        //        key is in the Set.
        //        Returns NULL if and only if key is not in the Set.
        // Post:  Set is unchanged
        T * search(string key) const;

};


// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T * x) {
    int stringSize = x->key.size();
    //int j = stringSize-1;
    string str = x->key;
    unsigned long long sum = 0;
    
    if (stringSize >= 1) {
       sum = helperInsert(str[0]);
    }
    for (int i = 1; i < stringSize; i++) {
        // Horner's method for polynomials
        sum = sum*64 + helperInsert(str[i]);
    }

    // for (int i = 0; i < stringSize; i++) {
    //     sum += (helperInsert(str[i]) * helperPower(64, j));
    //     j--;
    // }


    sum = (scale * sum) % M;
    if (A[sum] != NULL) {
        if (A[sum]->key == x->key) {
            A[sum] = x;
            return sum;
        }
    }

    int pos;
    for (int i = 0; i < M; i++) {
        pos = (sum + i*i) % M;
        if (A[pos] == NULL) {
            A[pos] = x;
            return pos;
        }
    }
    return -1;
}


// Desc:  Returns T * x such that x->key == key, if and only if 
//        key is in the Set.
//        Returns NULL if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T * Set<T>::search(string key) const {
    for (int i = 0; i < M; i++) {
        if (A[i] != NULL) {
            if (A[i]->key == key) {
                return A[i];
            }
        }
    }
    return NULL;
}


// Insert helper function to convert ASCII values to table in pdf
template <class T>
int Set<T>::helperInsert(char c) {
    // if char is a number
    if (c >= 48 && c <= 57) {
        return c - 48;
    }

    // if char is lower case letters
    if (c >= 97 && c <= 122) {
        return c - 87;
    }

    // if char is upper case letters
    if (c >= 65 && c <= 90) {
        return c - 29;
    }

    // if char is an other value
    return 62;
}

// Helper function to calculate the value to the power of j
template <class T>
long long Set<T>::helperPower(int value, int j) {
    if (j == 0) {
        return 1;
    }
    long long sum = value;
    for (int i = 1; i < j; i++) {
        sum *= value;
    }
    return sum;
}