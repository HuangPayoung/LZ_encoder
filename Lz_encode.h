#ifndef LZ_ENCODE_H
#define LZ_ENCODE_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

class Lz_encode
{
private:
	string s, re;      //s is binary file, re is the encode file
	const char *file, *encode_file, *decode_file;
	void read_file(const char *file_name);
	void write_file(const char *file_name);
	string prefix_num_to_string(int prefix, int cur_num);
public:
	void encode();
	void decode();
	Lz_encode(const char *f, const char *e, const char *d):file(f), encode_file(e), decode_file(d){}
};

#endif
