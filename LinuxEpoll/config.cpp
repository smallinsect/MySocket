
#include "config.h"

#include <stdio.h>

void readData(char *szIp, char *szPort){
	FILE *fp = fopen("config.xml", "r");
	if(fp == NULL){
		printf("open config.xml error ...\n");
		return ;
	}
	printf("open config.xml success ...\n");
	fscanf(fp, "%s", szIp);
	printf("ip=%s\n", szIp);
	fscanf(fp, "%s", szPort);
	printf("port=%s\n", szPort);
	fclose(fp);
}

