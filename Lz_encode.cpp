#include "pch.h"
#include "Lz_encode.h"

string Lz_encode::prefix_num_to_string(int prefix, int cur_num)  
{
	//turn the paragraph number from decimal int to binary string number with the limited length
	int prefix_length = 0, num = 1;
	for (; num < cur_num; prefix_length++) num <<= 1;
	if (prefix_length == 0) prefix_length++;
	string re = "";
	while (prefix > 0)
	{
		if (prefix & 1) re = '1' + re;
		else re = '0' + re;
		prefix >>= 1;
	}
	while (prefix_length > re.length())
		re = '0' + re;
	return re;
}

void Lz_encode::read_file(const char *file_name)    
{
	//read from the file and save in string s
	s = "";
	char ch;
	ifstream itxt(file_name, ios::binary);
	while (itxt.get(ch))
	{
		bitset<8> t(ch);
		s += t.to_string();
	}
	itxt.close();
}

void Lz_encode::write_file(const char *file_name)   
{
	//write into the file from string re
	ofstream otxt(file_name, ios::binary);
	for (int i = 0; i < re.length(); i += 8)
	{
		int num = 0;
		for (int j = 0; j < 8 && j < re.length() - i; j++)
		{
			num <<= 1;
			num += int(re[i + j] - '0');
		}
		otxt.write(reinterpret_cast<char *>(&num), sizeof(char));
	}
	otxt.close();
}

void Lz_encode::encode()
{
	read_file(file);

	map<string, int> m;
	int cur_para_num = 1;          //current paragraph number
	string cur_word;
	re = "";
	for (int i = 0; i < s.size(); i++)
	{
		cur_word += s[i];
		if (i == s.size() - 1)    //when finish reading the file
		{
			int prefix;
			string prefix_string;
			if (m.count(cur_word))
			{
				prefix = m[cur_word];
				prefix_string = prefix_num_to_string(prefix, cur_para_num);
			}
			else
			{
				if (cur_word.size() == 1) prefix = 0;
				else prefix = m[cur_word.substr(0, cur_word.size() - 1)];
				prefix_string = prefix_num_to_string(prefix, cur_para_num);
				if (cur_word[cur_word.size() - 1] == '1')
					prefix_string += '1';
				else
					prefix_string += '0';
				m[cur_word] = cur_para_num;
			}
			re += prefix_string;
			continue;
		}
		if (m.count(cur_word)) continue;
		else
		{
			int prefix;
			string prefix_string;
			if (cur_word.size() == 1)
				prefix = 0;
			else
				prefix = m[cur_word.substr(0, cur_word.size() - 1)];
			prefix_string = prefix_num_to_string(prefix, cur_para_num);
			if (cur_word[cur_word.length() - 1] == '1')
				prefix_string += '1';
			else
				prefix_string += '0';
			re += prefix_string;
			m[cur_word] = cur_para_num;
			cur_para_num++;
			cur_word = "";
		}
	}
	//add one byte to solve this situation: if the last word is not enough for one byte
	int rest_len = re.length() % 8;
	if (rest_len)
	{
		int add_len = 8 - rest_len;
		for (int i = 0; i < add_len; i++) re += '0';
		string tail = "00000";
		if (add_len & 4) tail += '1';
		else tail += '0';
		if (add_len & 2) tail += '1';
		else tail += '0';
		if (add_len & 1) tail += '1';
		else tail += '0';
		re += tail;
	}
	else re += "00000000";
	write_file(encode_file);
}

void Lz_encode::decode()
{
	read_file(encode_file);
	//use the last sign byte to recover
	int len = s.length(), add_len = 0;
	if (s[len - 1] == '1') add_len += 1;
	if (s[len - 2] == '1') add_len += 2;
	if (s[len - 3] == '1') add_len += 4;
	s = s.substr(0, len - add_len - 8);

	map<int, string> m;
	int cur_para_num = 1, word_length = 2;          //current paragraph number
	string cur_word;
	re = "";
	for (int i = 0; i < s.size(); i++)
	{
		cur_word += s[i];
		if (i == s.size() - 1)
		{
			int index = 0;
			string predix_string;
			if (cur_word.length() == word_length)
			{
				for (int j = 0; j < word_length - 1; j++)
				{
					index <<= 1;
					if (cur_word[j] == '1') index++;
				}
				if (index)
					predix_string = m[index];
				else
					predix_string = "";
				if (cur_word[word_length - 1] == '1') predix_string += '1';
				else predix_string += '0';
				m[cur_para_num] = predix_string;
			}
			else
			{
				for (int j = 0; j < cur_word.size(); j++)
				{
					index <<= 1;
					if (cur_word[j] == '1') index++;
				}
				if (index) 
					predix_string = m[index];
				else 
					predix_string = "";
			}
			re += predix_string;
			continue;
		}
		if (cur_word.length() == word_length)
		{
			int index = 0;
			string predix_string;
			for (int j = 0; j < word_length - 1; j++)
			{
				index <<= 1;
				if (cur_word[j] == '1') index++;
			}
			if (index)
				predix_string = m[index];
			else
				predix_string = "";
			if (cur_word[word_length - 1] == '1') predix_string += '1';
			else predix_string += '0';
			re += predix_string;

			m[cur_para_num] = predix_string;
			cur_para_num++;
			cur_word = "";
			int prefix_length = 0, num = 1;
			for (; num < cur_para_num; prefix_length++)
			{
				num <<= 1;
			}
			word_length = prefix_length + 1;
		}
	}
	write_file(decode_file);
}
