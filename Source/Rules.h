#pragma once
#include <string>
#include <iostream>
#include <map>
using namespace std;


class rule{
public:
	rule();
	rule(string filename);
	~rule();

	int lookup[10][35750];
	int result(int current, int n[8]);
};