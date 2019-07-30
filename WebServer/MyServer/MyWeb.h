#pragma once

#include "HeaderFiles.h"

int Request(SOCKET s);
void Response(SOCKET s);

std::string getContentType(const std::string &fileType);