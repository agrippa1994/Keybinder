#pragma once
#define IMPORT extern "C" __declspec(dllimport)

IMPORT int ReadStats(char *& text, size_t allocationSize);
