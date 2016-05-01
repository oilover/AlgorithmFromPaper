#pragma once
#include "Dict.h"


vector<uint> LZW_encipher(const char *S, int S_n, string key = "password", int b = 10)
{
	vector<uint> B;
	int b_2 = b / 2;
	int N = 1 << b;
	int M = 1 << (b / 2);
	D = Dict(b);
	for (int i = 0; i <= 255; i++)
		D.add_entry(string(1,i), key);

	int dict_n = 256;
	vector<uint> PRBS = RC4_PRBS(key, S_n * 3 * b);
	vector<uint> PRNS_b_2 = bit2num(PRBS, b_2);
	int S_i = 0;
	int PRNS_i = 0;
	int ciphertext_n = 0;
	while (S_i < S_n) {
		int index, index_old = -1;
		string I, Ix = "";
		int i = S_i;
		string tmp = "";
		while (i < S_n) {
			tmp = tmp + string(1, S[i]);
			index = D.get_index(tmp);
			if (index < 0) {
				Ix = tmp;
				index = index_old;
				break;
			}
			I = tmp;
			index_old = index;
			i++;
		}
		S_i = i;
		auto X_i = index;
		uint high = PRNS_b_2[PRNS_i++];
		uint low = PRNS_b_2[PRNS_i++];
		auto K_i = (high << b_2) + low;
		auto B_i = X_i ^ K_i;
		B.push_back(B_i);
		if (Ix != "") {
			if (dict_n < N) {
				index = D.add_entry(Ix, key);
				dict_n++;
			}
		}
		for (int i = PRNS_i; i <= PRNS_i + 3; i++) vec[i-PRNS_i] = PRNS_b_2[i];
		D.permute(vec);
		PRNS_i += 4;
		ciphertext_n++;
	}
	return B;
}
