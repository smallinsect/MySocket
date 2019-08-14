#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <windows.h>

#include <iostream>
using namespace std;

char fileType[16];
int main() {

	char URL[1024] = "/GameProject";
	const char mat[] = ".";

	char *p = strtok(URL, mat);
	while (p) {
		cout << p << endl;
		p = strtok(NULL, mat);
	}
	system("pause");
	return 0;
}