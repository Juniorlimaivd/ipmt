#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <string>
#include <cstring>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>

#define lli long long int 

typedef struct SuffixInfo {
    lli index;
    lli rank;
    lli previous_rank;
} SuffixInfo;

int suffixComp(SuffixInfo a, SuffixInfo b) {
    if (a.rank > b.rank) return 0;
    else if (a.rank < b.rank) return 1;
    else if (a.previous_rank < b.previous_rank) return 1;
    else return 0;
}

void printArray(std::vector<lli> a) {
    for (int i = 0; i < a.size(); i++) 
        std::cout << a[i] << " " << std::endl; 
    
}

class SuffixArray
{
private:


    std::vector<lli> buildSuffixArray(std::string text) {
        lli n = text.size();
        lli l = (lli) ceil(log2(n));
        std::vector<lli> original_index(n, 0);

        this->_prefix.assign(l + 1, std::vector<lli>(n, -1));

        std::vector<SuffixInfo> suffixes(n);

        for (lli i = 0 ; i < n ; i++) {
            suffixes[i].index = i;
            suffixes[i].rank = text[i];
            suffixes[i].previous_rank = (i+1 < n) ? text[i + 1] : -1; 
        }

        std::sort(suffixes.begin(), suffixes.end(), suffixComp);

        lli aux = 0;
        for (lli i=0 ; i < n ; i ++) {
            if ( i > 0 && suffixes[i].rank != suffixes[i-1].rank) ++aux;
            this->_prefix[0][suffixes[i].index] = aux;
        }

        for (lli k = 1; k < l+1; k++) {
            lli j = 1 << (k-1);

            for (lli i = 0; i < n; i++) {
                suffixes[i].rank = this->_prefix[k - 1][i];
                suffixes[i].index = i;
                if (i + j >= n) suffixes[i].previous_rank = -1;
                else suffixes[i].previous_rank = _prefix[k-1][i + j];
            }

            std::sort(suffixes.begin(), suffixes.end(), suffixComp);

            lli rank = 0;

            _prefix[k][suffixes[0].index] = rank;
            for (lli i = 1; i < n; i++) {
                if (suffixes[i].rank != suffixes[i-1].rank 
                    || suffixes[i].previous_rank != suffixes[i-1].previous_rank) rank++;

                _prefix[k][suffixes[i].index] = rank;
            }
        }

        std::vector<lli> suffixArray(n);

        for(lli i=0; i < n; i++) {
            suffixArray[_prefix[l][i]] = i;
        }

        return suffixArray;
    }

    void buildLR_LCP(){
        lli n = _text.size();
        _leftLCP.assign(n, 0);
        _rightLCP.assign(n, 0);
        fillLCP(0, n - 1);
    };

    void fillLCP(lli left, lli right) {
        if (right - left > 1) {
            lli mid = (left + right) / 2;
            _leftLCP[mid] = this->lcp_prefix(_suffixArray[left], _suffixArray[mid]);
            _rightLCP[mid] = this->lcp_prefix(_suffixArray[right], _suffixArray[mid]);
            fillLCP(left, mid);
            fillLCP(mid, right);
        }
    }

    lli lcp_prefix(lli left, lli right) {
        lli n = _text.size();
        if (left == right) return n - left ;
        lli lcp = 0;

        for(lli k = ceil(log2(n)); k >= 0 && left < n && right < n; k--) {
            if (_prefix[k][left] == _prefix[k][right]) {
                lcp += 1 << k;
                left += 1 << k;
                right += 1 << k;
            }
        }

        return lcp;
    }

    lli lcp(const char * a, const char * b) {
        lli l =0;
        while (a[l] != '\0' && b[l] != '\0' && a[l] == b[l]) l++;
        return l;
    }

    lli successor(std::string pattern) {
        lli m = pattern.size();
        lli n = _text.size();

        const char* pat = pattern.c_str();
        const char* txt = _text.c_str();
        m = std::strlen(pat);

        lli l = lcp(pat, txt + _suffixArray[0]), r = lcp(pat, txt + _suffixArray[n-1]); 

        if (r < m && (unsigned char)_text[_suffixArray[n-1] + r] < (unsigned char)pattern[r]){
            return n;
        } 
        
        if (l == m || _text[_suffixArray[0] + l] > pattern[l]) return 0;

        

        lli left = 0, right = n-1;

        while (right - left > 1) {
            lli mid = (left + right) / 2;
            lli aux = -1;

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

    lli predecessor(std::string pattern) {

        lli m = pattern.size();
        lli n = _text.size();

        const char* pat = pattern.c_str();
        const char* txt = _text.c_str();

        lli l = lcp(pat, txt + _suffixArray[0]),
         r = lcp(pat, txt + _suffixArray[n-1]); 

        if (r == m || (unsigned char)_text[_suffixArray[n-1] + r] < (unsigned char)pattern[r]) return n - 1;
        
        if (l < m && (unsigned char)_text[_suffixArray[0] + l] > (unsigned char)pattern[l]) return -1;

        lli left = 0, right = n-1;

        while (right - left > 1) {
            lli mid = (left + right) / 2;
            lli aux = -1;


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

    void showLines(lli l, lli r){
        lli n = _text.size();
        lli start, line, end;
        for (lli i = l; i <= r; i++)
        {
            start = _suffixArray[i];
            line = start;

            while (line > 0) {
                if (_text[line] == '\n') {
                    line++;
                    break;
                }
                line--;
            }
            end = start;
            while (end < n) {
                if (_text[end] == '\n') {
                    end--;
                    break;
                }
                end++;
            }


            printf("%s\n", _text.substr(line, end - line).c_str());
        }
    }

public:
    SuffixArray(std::string text) {
        _suffixArray = this->buildSuffixArray(text);
        _text = text;
        this->buildLR_LCP();
    };

    SuffixArray(std::vector<lli> suffixArray, std::string text, std::vector<lli> leftLCP, std::vector<lli> rightLCP) {
        _suffixArray = suffixArray;
        // printArray(suffixArray);
        _leftLCP = leftLCP;
        _rightLCP = rightLCP;

         if(text[text.size()-1] == '\0'){
            text.pop_back(); // remove o <0x00> no fim do texto
         }  
        _text = text;
    };

    ~SuffixArray() {};

    lli search(std::string pattern, bool shouldPrintLines) {

        lli l = this->successor(pattern);

        lli r = this->predecessor(pattern);

        if (shouldPrintLines) showLines(l, r);
        //std::cout << "l: " << l << "r:" << r << std::endl;
        if (l > r) return 0;
        else return r - l + 1;
    };

    std::vector<lli> _suffixArray;
    std::string _text;
    std::vector<lli> _leftLCP, _rightLCP;
    std::vector< std::vector<lli> > _prefix;
};


#endif