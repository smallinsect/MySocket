#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

int main() {

	//int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//int sum = 0;
	//for (int *p = array; p < array + 10; ++p) {
	//	sum = sum + *p;
	//}
	//printf("%d\n", sum);

	double darray[10] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1 };
	double sum = 0;
	for (double *p = darray; p < darray + 10; ++p) {
		sum = sum + *p;
	}
	printf("%lf\n", sum);

	system("pause");
	return 0;
}