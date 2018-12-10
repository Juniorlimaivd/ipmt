#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <string>
#include <math.h>
#include <vector>
#include <iostream>

#define lli long long int 

typedef struct SuffixInfo {
    int index;
    int rank;
    int previous_rank;
} SuffixInfo;

int suffixComp(SuffixInfo a, SuffixInfo b) {
    if (a.rank > b.rank) return 0;
    else if (a.rank < b.rank) return 1;
    else if (a.previous_rank < b.previous_rank) return 1;
    else return 0;
}

void printArray(std::vector<int> a) {
    for (int i = 0; i < a.size(); i++) 
        std::cout << a[i] << " "; 
    std::cout << std::endl; 
}

class SuffixArray
{
private:
    std::vector<int> _suffixArray;
    std::string _text;
    std::vector<int> _leftLCP, _rightLCP;
    std::vector< std::vector<int> > _prefix;

    std::vector<int> buildSuffixArray(std::string text) {
        int n = text.size();
        int l = (int) ceil(log2(n));
        int original_index[n];
        this->_prefix.assign(l + 1, std::vector<int>(n, -1));

        SuffixInfo suffixes[n];

        for (int i = 0 ; i < n ; i++) {
            suffixes[i].index = i;
            suffixes[i].rank = text[i];
            suffixes[i].previous_rank = (i+1 < n) ? text[i + 1] : -1; 
        }

        std::sort(suffixes, suffixes + n, suffixComp);

        int aux = 0;
        for (int i=0 ; i < n ; i ++) {
            if ( i > 0 && suffixes[i].rank != suffixes[i-1].rank) ++aux;
            this->_prefix[0][suffixes[i].index] = aux;
        }

        for (int k = 1; k < l+1; k++) {
            int j = 1 << (k-1);

            for (int i = 0; i < n; i++) {
                suffixes[i].rank = this->_prefix[k - 1][i];
                suffixes[i].index = i;
                if (i+j >= n) suffixes[i].previous_rank = -1;
                else suffixes[i].previous_rank = _prefix[k-1][i + j];
            }

            std::sort(suffixes, suffixes + n, suffixComp);

            int rank = 0;

            _prefix[k][suffixes[0].index] = rank;
            for (int i = 1; i < n; i++) {
                if (suffixes[i].rank != suffixes[i-1].rank 
                    || suffixes[i].previous_rank != suffixes[i-1].previous_rank) rank++;

                _prefix[k][suffixes[i].index] = rank;
            }
        }

        std::vector<int> suffixArray(n);

        for(int i=0; i < n; i++) {
            suffixArray[_prefix[l][i]] = i;
        }

        // for(int i=0; i < l+1; i++ ) {
        //     printArray(_prefix[i]);
        // }

        // printArray(suffixArray);

        return suffixArray;
    }

    void buildLR_LCP(){
        int n = _text.size();
        _leftLCP.assign(n, 0);
        _rightLCP.assign(n, 0);
        fillLCP(0, n - 1);

        // printArray(_leftLCP);
        // printArray(_rightLCP);
    };

    void fillLCP(int left, int right) {
        if (right - left > 1) {
            int mid = left + (right - left) / 2;
            _leftLCP[mid] = this->lcp_prefix(_suffixArray[left], _suffixArray[mid]);
            _rightLCP[mid] = this->lcp_prefix(_suffixArray[right], _suffixArray[mid]);
            fillLCP(left,mid);
            fillLCP(mid, right);
        }
    }

    int lcp_prefix(int left, int right) {
        // std::cout << "computing lcp i = " << left << " j= " << right << std::endl;
        int n = _text.size();
        if (left == right) return n - left ;
        int lcp = 0;

        for(int k = ceil(log2(n)); k >= 0 && left < n && right < n; k--) {
            if (_prefix[k][left] == _prefix[k][right]) {
                lcp += 1 << k;
                left += 1 << k;
                right += 1 << k;
            }
        }

        return lcp;
    }

    int lcp(const char * a, const char * b) {
        int l =0;
        while (a[l] != '\0' && b[l] != '\0' && a[l] == b[l]) l++;
        return l;
    }

    int successor(std::string pattern) {
        int m = pattern.size();
        int n = _text.size();

        const char* pat = pattern.c_str();
        const char* txt = _text.c_str();

        int l = lcp(pat, txt + _suffixArray[0]), r = lcp(pat, txt + _suffixArray[n-1]); 

        if (l == m || _text[_suffixArray[0] + l] > pattern[l]) return 0;

        if (r < m && _text[_suffixArray[n-1] + r] < pattern[r]) return n;

        int left = 0, right = n-1;

        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            int aux = -1;

            // std::cout << "l= " << left << " suf_l " << txt+_suffixArray[left] << std::endl;
            // std::cout << "h= " << mid << " suf_h " << txt+_suffixArray[mid] << std::endl;
            // std::cout << "r= " << right << " suf_r " << txt+_suffixArray[right] << std::endl;
            if (l >= r) {
                if (_leftLCP[mid] >= l) {
                    aux = l + lcp(pat + l, txt + _suffixArray[mid] + l);
                } else {
                    aux = _leftLCP[mid];
                }
            } else {
                if (_rightLCP[mid] >= r) {
                    aux = r + lcp(pat + r, txt + _suffixArray[mid] + r);
                } else {
                    aux = _rightLCP[mid];
                }
            }

            if (aux == m || pattern[aux] < _text[_suffixArray[mid] + aux]) {
                right = mid;
                r = aux;
            } else {
                left = mid;
                l = aux;
            }
        }

        return right;
    }

    int predecessor(std::string pattern) {
        // std::cout << "prd" << std::endl;
        int m = pattern.size();
        int n = _text.size();

        const char* pat = pattern.c_str();
        const char* txt = _text.c_str();

        int l = lcp(pat, txt + _suffixArray[0]), r = lcp(pat, txt + _suffixArray[n-1]); 

        if (r == m || (unsigned char)_text[_suffixArray[n-1] + r] < (unsigned char)pattern[r]) return n - 1;
        
        if (l < m && (unsigned char)_text[_suffixArray[0] + l] > (unsigned char)pattern[l]) return -1;

        int left = 0, right = n-1;

        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            int aux = -1;
            // std::cout << "l= " << left << " suf_l " << txt+_suffixArray[left] << std::endl;
            // std::cout << "h= " << mid << " suf_h " << txt+_suffixArray[mid] << std::endl;
            // std::cout << "r= " << right << " suf_r " << txt+_suffixArray[right] << std::endl;

            if (l >= r) {
                if (_leftLCP[mid] >= l) {
                    aux = l + lcp(pat + l, txt + _suffixArray[mid] + l);
                } else {
                    aux = _leftLCP[mid];
                }
            } else {
                if (_rightLCP[mid] >= r) {
                    aux = r + lcp(pat + r, txt + _suffixArray[mid] + r);
                } else {
                    aux = _rightLCP[mid];
                }
            }

            if (aux == m || (unsigned char)pattern[aux] > (unsigned char)_text[_suffixArray[mid] + aux]) {
                left = mid;
                l = aux;
            } else {
                right = mid;
                r = aux;
            }
        }

        return left;
    }

public:
    SuffixArray(std::string text) {

        _suffixArray = this->buildSuffixArray(text);

        _text = text;
        this->buildLR_LCP();
    };

    SuffixArray(std::vector<int> suffixArray, std::string text, std::vector<int> leftLCP, std::vector<int> rightLCP) {
        _suffixArray = suffixArray;
        _leftLCP = leftLCP;
        _rightLCP = rightLCP;
        _text = text;
    };

    ~SuffixArray() {

    };

    int search(std::string pattern) {
        int l = this->successor(pattern);
        int r = this->predecessor(pattern);

        // std::cout << l << " , " << r << std::endl;

        if (l > r) return 0;
        else return r - l + 1;
    };
};


#endif