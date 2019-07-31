#pragma once

#include "HeaderFiles.h"

int Request(SOCKET s);
void Response(SOCKET s);

string getContentType(const string &fileType);