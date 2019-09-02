#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
using namespace std;

void helloworld() {
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
	std::cout << "hello world\n";
}

int main() {

	std::thread t(helloworld);
	std::cout << "heheheh" << endl;
	t.join();

	system("pause");
	return 0;
}