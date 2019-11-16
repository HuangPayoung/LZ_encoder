#include "pch.h"
#include "Lz_encode.h"
#include "time.h"
#include <iostream>

using namespace std;

int main()
{	
	Lz_encode a("a.txt", "a1.lz", "a_decode.txt"), b("b.docx", "b1.lz", "b_decode.docx");
	clock_t t1, t2; 
	double time;

	t1 = clock();
	a.encode();
	t2 = clock();
	time = (double)(t2 - t1) / CLOCKS_PER_SEC;
	cout << "txt file encode time:" << time << "s" << endl;
	
	t1 = clock();
	a.decode();
	t2 = clock();
	time = (double)(t2 - t1) / CLOCKS_PER_SEC;
	cout << "txt file decode time:" << time << "s" << endl;

	t1 = clock();
	b.encode();
	t2 = clock();
	time = (double)(t2 - t1) / CLOCKS_PER_SEC;
	cout << "docx file encode time:" << time << "s" << endl;
	
	t1 = clock();
	b.decode();
	t2 = clock();
	time = (double)(t2 - t1) / CLOCKS_PER_SEC;
	cout << "docx file decode time:" << time << "s" << endl;

	return 0;
}