#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <windows.h>

int main() {

	TCHAR szPath[1024] = TEXT("E:/smallinsect");
	TCHAR szFilePath[MAX_PATH];

	lstrcpy(szFilePath, szPath);
	//lstrcat(szFilePath, "/*.txt");//可以用于查找.txt结尾的文件
	lstrcat(szFilePath, "/*");

	WIN32_FIND_DATA findFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &findFileData);
	if (hListFile == INVALID_HANDLE_VALUE) {
		printf("FindFirstFile error %d...\n", GetLastError());
		system("pause");
	}

	while (true) {
		printf("%s\n", findFileData.cFileName);

		if (!FindNextFile(hListFile, &findFileData)) {
			break;
		}
	}

	system("pause");
	return 0;
}