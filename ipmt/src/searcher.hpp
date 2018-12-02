#ifndef SEARCHER_H
#define SEARCHER_H

#include <string>
#include <vector>


class Searcher
{
public:
    virtual bool search(std::string text, bool isCompleteLine) = 0;
    virtual void reset() = 0;
    virtual void resetPattern(std:: string pattern) = 0;
    virtual int count() = 0;
};

#endif

