#ifndef __MyRequest_H__
#define __MyRequest_H__

#include "common.h"


class MyRequest {
public:
	std::string method;//���󷽷�
	std::string URI;//��������
	std::string httpver;//�汾
	std::string type;//�ļ�������
	std::string file;//���·�����ļ���
	std::string arg;//����
public:
	void setParam(char* buf, int length);
	std::string getContentType();
};


#endif // !__MyRequest_H__