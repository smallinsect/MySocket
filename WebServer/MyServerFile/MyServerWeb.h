#pragma once

#include "HeaderFiles.h"

int request(SOCKET);
void response(SOCKET);

string getContentType(const string &fileType);