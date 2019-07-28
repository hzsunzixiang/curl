#pragma once

#include <string>
#include <string.h>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

typedef map<string,string> CStr2Map;

std::string regUrlEncode(const std::string& src);
std::string regUrlDecode(const std::string& src);

