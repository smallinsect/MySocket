#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <windows.h>

int main() {
	char uri[1024] = "/hello.txt?";
	char fileType1[64] = "";
	char fileType2[64] = "";

	sscanf(uri, "%[^.]%[^ ?]", fileType1, fileType1);
	printf("%s %s\n", fileType1, fileType1);

	system("pause");
	return 0;
}