#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

int main() {

	int n, m;
	int ret;
	while ((ret = scanf("%d", &n)) > 0) {
		printf("%d %d\n", n, ret);
	} 
	printf("%d\n", ret);

	system("pause");
	return 0;
}