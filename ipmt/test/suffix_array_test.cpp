#include <stdio.h>
#include <iostream>
#include <string>
#include "../src/suffix_array.hpp"

using namespace std;

int main() {
    string txt = "banana";
    string pat = "na";

    SuffixArray s(txt);
    cout << s.search(pat) << endl;

    return 0;
}