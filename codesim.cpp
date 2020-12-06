#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Fingerprint.h"
using namespace std;

float find_match(Fingerprint& fp1, Fingerprint& fp2) {
	int* dp_temp = new int[fp1.len()];
	int* dp = new int[fp1.len()];
	memset(dp_temp, 0, fp1.len() * sizeof(int));
	for (int i = 1; i < fp2.len(); i++) {
		
		for (int j = 0; j < fp1.len(); j++) {
			if (i == 0 || j == 0)
				dp[j] = 0;
			else if (fp2.fp(i - 1) == fp1.fp(j - 1)) {
				dp[j] = dp_temp[j - 1] + 1;
			}
			else {
				dp[j] = max(dp[j - 1], dp_temp[j]);
			}
		}
		int* temp = dp_temp;
		dp_temp = dp;
		dp = temp;
		memset(dp, 0, fp1.len() * sizeof(int));
	}
	int match = dp_temp[fp1.len() - 1];
	delete[] dp;
	delete[] dp_temp;
	return ((float)match / fp1.len() + (float)match / fp2.len()) / 2;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Please input: codesim code1 code2\n");
		exit(-1);
	}
	ifstream f1(argv[1], ios::in);
	ifstream f2(argv[2], ios::in);
	if (!f1 || !f2) {
		fprintf(stderr, "open file error\n");
		exit(-1);
	}
	string cpp_fn1 = basename(argv[1]);
	string cpp_fn2 = basename(argv[2]);
	
	system(("gcc -c -w -std=c++11 -pedantic " + (string)argv[1]).c_str());
	system(("gcc -c -w -std=c++11 -pedantic " + (string)argv[2]).c_str());
	string obj_fn1 = cpp_fn1.substr(0, cpp_fn1.length() - 4) + ".o";
	string obj_fn2 = cpp_fn2.substr(0, cpp_fn2.length() - 4) + ".o";
	
	Fingerprint fp1 = Fingerprint(obj_fn1, 1, 10);
	Fingerprint fp2 = Fingerprint(obj_fn2, 1, 10);
	cout << find_match(fp1, fp2) << endl;

	system(("rm " + obj_fn1).c_str());
	system(("rm " + obj_fn2).c_str());
	return 0;
}