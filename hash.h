// Name: Parmveer Dayal
// Date: August 5, 2022

#include <iostream>
#include <cstring>

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
    string str = x->key;
    unsigned long long sum = 0;
    
    // make sure string is not empty before getting 0th index
    if (stringSize >= 1) {
       sum = helperInsert(str[0]);
    }
    for (int i = 1; i < stringSize; i++) {
        // Horner's method for polynomials
        sum = sum*64 + helperInsert(str[i]);
    }

    sum = (scale * sum) % M;
    // if key is already inside of hash table then replace old with new
    if (A[sum] != NULL) {
        if (A[sum]->key == x->key) {
            A[sum] = x;
            return sum;
        }
    }

    int pos;
    // quadratic probing to find open index to insert and return index
    for (int i = 0; i < M; i++) {
        pos = (sum + i*i) % M;
        if (A[pos] == NULL) {
            A[pos] = x;
            return pos;
        }
    }
    // return -1 if cannot find open index
    return -1;
}


// Desc:  Returns T * x such that x->key == key, if and only if 
//        key is in the Set.
//        Returns NULL if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T * Set<T>::search(string key) const {
    // loop through hash table to find key and return object if found
    for (int i = 0; i < M; i++) {
        if (A[i] != NULL) {
            if (A[i]->key == key) {
                return A[i];
            }
        }
    }
    // return NULL if key not inside of hash table
    return NULL;
}


// Insert helper function to convert ASCII values to table given in pdf
template <class T>
int Set<T>::helperInsert(char c) {
    // if char is a number
    if (c >= 48 && c <= 57) {
        return c - 48;
    }

    // if char is a lower case letter
    if (c >= 97 && c <= 122) {
        return c - 87;
    }

    // if char is an upper case letter
    if (c >= 65 && c <= 90) {
        return c - 29;
    }

    // if char is an other value
    return 62;
}
