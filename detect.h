#pragma once
#include "access_token.h"
int face_detect(std::string &json_result, const std::string &image_base64, std::string &access_token);
std::string readFileIntoString(char * filename);