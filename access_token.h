#include <atlenc.h>
#pragma once
int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK);
errno_t fileBase64Encode(const char *srcFile, const char *dstFile);
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream);