#pragma once
#include <string>
#include <vector>

#define IMPORT extern "C" __declspec(dllimport)

IMPORT int ReadStats(char *& text, size_t allocationSize);


std::string readStats()
{
    const size_t allocation = 4096;
    std::vector<char> data(allocation, 0);

    char *p1 = (char *)data.data();
    char *&p = p1;

    if(ReadStats(p, allocation))
        return std::string(p);

    return {};
}
