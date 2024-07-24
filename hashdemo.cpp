
#include "hash.h"
#include <cstdlib>
#include <string>
#include <assert.h>

using namespace std;

class pod {
    public:
        string key;
        pod() { key = ""; }
        pod(string key) { this->key = key; }
};


int main () {
    Set<pod> set;
    pod a[10];
    a[0] = pod("hello");
    a[1] = pod("banana");
    a[2] = pod("apple");
    a[3] = pod("apricot");
    a[4] = pod("candy");
    a[5] = pod("carkey");
    a[6] = pod("godefroy");
    a[7] = pod("airy");
    a[8] = pod("hugs");
    a[9] = pod("can");
    int pos[10];

    for (int i = 0; i < 10; i++) {
        pos[i] = set.insert(a + i);
        cout << pos[i] << ' ' << a[i].key << endl;
    }

    for (int i = 0; i < 10; i++) {
        assert (set.search(a[i].key) == a + i);
    }
    cout << set.search("kumquat") << endl;
    cout << set.search("cand") << endl;
    cout << set.search("hello") << endl;
    cout << set.search("godefroy")->key << endl;
} // main

