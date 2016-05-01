#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <unordered_map>
#include "RC4.h"
#include "HMAC.h"
#define prt(k) cout<<#k" = "<<k<<endl
typedef long long LL;
using namespace std;
typedef unsigned long long uint;
static int vec[5];
//const static int MAXLEN = 1600300;
const static int MAXSIZE = 4203;
int tmp_id[MAXSIZE];
int tmp_rid[MAXSIZE];
vector<uint> bit2num(vector<uint> x, int L = 8, string endianness = "big")
{
	vector<uint> ret;
	if (L > 64) L = 64;
	int x_n = x.size();
	int y_n = (x_n + L - 1) / L;
	string type;
	ret.resize(y_n);
	for (int i = 0; i< y_n * L - x_n; i++) x.push_back(0);
	if (L > 32) {
		type = "uint64";
	}
	else if (L > 16) type = "uint32";
	else if (L > 8) type = "uint16";
	else type = "uint8";
	for (int i = 0; i<x.size(); i += L) {
		uint t = 0;
		if (endianness == "big")  //大端法
			for (int j = i; j < i + L; j++)
				t = t * 2 + x[j];
		else 
			for (int j = i + L - 1; j >= i; j--)
				t = t * 2 + x[j];
		ret[i / L] = t;
	}
	return ret;
}
class Dict
{
public:
	int b;
	string dict[MAXSIZE];
	unordered_map<string, int> ID;
	int id[MAXSIZE];
	int rid[MAXSIZE];
public:
	Dict(int _b = 10)
	{
		ID.clear();
		b = _b;
		for (int i = 0; i < (1 << b); i++) dict[i] = "", id[i] = rid[i] = i;
	}
	int size() const
	{
		return ID.size();
	}
	void permute(int c1, int c2, int r1, int r2)
	{
		int m = 1 << (b / 2);
		for (int x = 0; x < (1 << b); x++)
			tmp_id[x] = id[x], tmp_rid[x] = rid[x];
		for (int x = 0; x < (1 << b); x++) {
			int y = id[x];
			int i = y / m, j = y % m;
			int c = (j & 1) ? c1 : c2;
			i = (i + c) % m;
			int r = (i & 1) ? r1 : r2;
			j = (j + r) % m;
			rid[i*m + j] = x;
			id[x] = i*m + j;
		}
	}
	void cancel_permute()
	{
		for (int i = 0; i < (1 << b); i++)
			id[i] = tmp_id[i], rid[i] = tmp_rid[i];
	}
	void permute(int *v)
	{
		permute(v[0], v[1], v[2], v[3]);
	}
	int get_index(string word)
	{
		if (ID.count(word) == 0) return -1;
		return id[ID[word]];
	}
	int add_entry(string word, string key) // b��2�ı���
	{
		unsigned char *tmp = NULL; unsigned int len;
		HMAC(word, key, tmp, len);  //利用HMAC消息签名算法
		int index = 0;
		for (int i = 0; i < b; i++) //计算word的哈希值
		{
			index = index * 2 + ( int(tmp[i >> 3]) >> (7 - (i&7)) & 1);
		}
		if (dict[rid[index]] == "") {
			dict[rid[index]] = word;
			ID[word] = rid[index];
			return index;
		}
		int cnt = 0;
		vector<int> indices;
		for (int i = 0; i < (1 << b); i++)
			if (dict[rid[i]] == "")
			{
				cnt++;
				indices.push_back(i);
			}
		if (cnt == 0) return -1;
		int t = index % cnt;
		index = indices[t];
		dict[rid[index]] = word;
		ID[word] = rid[index];
		return index;
	}
}D;
