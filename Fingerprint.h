#pragma once
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class Fingerprint {
private:
	int kgram;
	int win_width;
	int max_length = 100000;
	int file_length = 0;
	int fp_length;
	void generate_hash(string fn);
	long long rolling_hash(unsigned char* beg, unsigned char* end, int idx);
	void generate_fingerprint();
	long long* hashes;
	vector<long long> fingerprint;

public:
	Fingerprint(string fn, int step, int win_width);
	int len();
	long long fp(int idx);
	long long hash(int idx);
	~Fingerprint();
};