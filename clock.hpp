#pragma once
// #include <windows.h>
// SetConsoleOutputCP(437); // Enables extended ASCII chars (╔═╗║ etc.)
#include <iostream>
#include <string>

using namespace std;

// Box drawing characters (UTF-8)
extern const char SYMBOL_HORIZONTAL[];
extern const char SYMBOL_VERTICAL[];
extern const char SYMBOL_TOP_LEFT[];
extern const char SYMBOL_TOP_RIGHT[];
extern const char SYMBOL_BOTTOM_LEFT[];
extern const char SYMBOL_BOTTOM_RIGHT[];
extern const char SYMBOL_T_LEFT[];
extern const char SYMBOL_T_RIGHT[];
extern const char SYMBOL_T_TOP[];
extern const char SYMBOL_T_BOTTOM[];
extern const char SYMBOL_INTERSECT[];

// Double line box drawing characters (UTF-8)
extern const char SYMBOL_DOUBLE_HORIZONTAL[];
extern const char SYMBOL_DOUBLE_VERTICAL[];
extern const char SYMBOL_DOUBLE_TOP_LEFT[];         
extern const char SYMBOL_DOUBLE_TOP_RIGHT[];
extern const char SYMBOL_DOUBLE_BOTTOM_LEFT[];
extern const char SYMBOL_DOUBLE_BOTTOM_RIGHT[];
extern const char SYMBOL_DOUBLE_T_LEFT[];
extern const char SYMBOL_DOUBLE_T_RIGHT[];
extern const char SYMBOL_DOUBLE_T_TOP[];
extern const char SYMBOL_DOUBLE_T_BOTTOM[];
extern const char SYMBOL_DOUBLE_INTERSECT[];

extern const char BLOCK_FULL[];
extern const char BLOCK_HALF[];