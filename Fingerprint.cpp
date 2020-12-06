#include "Fingerprint.h"
using namespace std;

Fingerprint::Fingerprint(string fn, int s, int ww) {
	kgram = s;
	win_width = ww;
	
	unsigned char* buffer = new unsigned char[max_length];
	FILE * fp = fopen(fn.c_str(), "rb");   //binary
	if (!fp) {
		fprintf(stderr, "Error when reading files\n");
		exit(-1);
	}
	while (!feof(fp)) {
		int cnt = fread(buffer, 1, max_length, fp);
		file_length += cnt;
	}
	fclose(fp);
	hashes = new long long[file_length - kgram + 1];
	delete[] buffer;
	generate_hash(fn);
	generate_fingerprint();
}

void Fingerprint::generate_hash(string fn) {
	FILE* fp = fopen(fn.c_str(), "rb");
	unsigned char* buffer = new unsigned char[file_length];
	int cnt = fread(buffer, 1, file_length, fp);
	rolling_hash(buffer, buffer + kgram, 0);
	for (int i = 1; i < file_length - kgram + 1; i++) {
		rolling_hash(buffer + i, buffer + i + kgram, i);
	}
	fclose(fp);
	delete[] buffer;
}

long long Fingerprint::rolling_hash(unsigned char* beg, unsigned char* end, int idx) {
	long long sum = 0;
	long long b = 3;
	long long cons = 1;
	long long mod = 1e10;
	for (int i = 1; i <= kgram - 1; i++) {
		cons = (cons * b) % mod;
	}
	if (idx == 0) {
		sum += (int)*beg;
		for (unsigned char* i = beg + 1; i < end; i++) {
			sum = (sum * b + (int)*i) % mod;
		}
		hashes[0] = sum;
	}
	else {
		long long fix = hashes[idx - 1] - *(beg - 1);
		if (fix < 0)
			fix += mod;
		hashes[idx] = ((fix * cons) % mod * b) % mod + *(end - 1);
	}
	return hashes[idx];
}

void Fingerprint::generate_fingerprint() {
	bool* selected = new bool[file_length - kgram + 1];
	memset(selected, 0, file_length - kgram + 1);

	long long min = hashes[0];
	int idx = 0;

	for (int j = 0; j < win_width; j++) {
		if (hashes[j] <= min) {
			min = hashes[j];
			idx = j;
		}
	}
	if (min != 0)
		fingerprint.push_back(min);

	for (int i = 1; i < file_length - kgram + 1 - win_width + 1; i++) {
		if (idx >= i && idx < i + win_width - 1) {
			if (hashes[i + win_width - 1] <= min) {
				min = hashes[i + win_width - 1];
				idx = i + win_width - 1;
				if (min != 0)
					fingerprint.push_back(min);
			}
		}
		else {
			min = hashes[i];
			idx = i;
			for (int j = i; j < i + win_width; j++) {
				if (hashes[j] <= min) {
					min = hashes[j];
					idx = j;
				}
			}
			if (min != 0)
				fingerprint.push_back(min);
		}
	}
	fp_length = distance(fingerprint.begin(), fingerprint.end());
}

int Fingerprint::len() {
	return fp_length;
}
long long Fingerprint::fp(int idx) {
	return fingerprint[idx];
}

long long Fingerprint::hash(int idx) {
	return hashes[idx];
}

Fingerprint::~Fingerprint() {
	delete[] hashes;
}
