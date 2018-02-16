#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include "identify.h"
#include<vector> 


// api url
const static std::string identify_url = "https://aip.baidubce.com/rest/2.0/face/v2/identify";
static std::string identify_result;

inline std::string int_to_string(int integer) {
	std::stringstream stream;
	stream << integer;
	return stream.str();
}

/**
* curl send http post, return result stored in identify_result;
* @param show in libcurl
* @return show in libcurl
*/
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// turn josn body into string
	identify_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}

/**
* identify face and store result in string
* @param json_result 
* @param group_id 
* @param images decode in base64
* @param access_token 
* @param user_top_num defult:1
* @param face_top_num defult:1
* @return success = 0;
*/
int identify(std::string &json_result, const std::string &group_id, const std::vector<std::string> &images, const std::string &access_token, int user_top_num, int face_top_num) {
	std::string url = identify_url + "?access_token=" + access_token;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		std::string image_plain = images[0];
		for (int i = 1; i < images.size(); i++) {
			image_plain.append("," + images[i]);
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_httppost *post = NULL;
		curl_httppost *last = NULL;
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "group_id", CURLFORM_COPYCONTENTS, group_id.data(),
			CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "images", CURLFORM_COPYCONTENTS, image_plain.data(),
			CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "user_top_num", CURLFORM_COPYCONTENTS,
			int_to_string(user_top_num).data(),
			CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "face_top_num", CURLFORM_COPYCONTENTS,
			int_to_string(face_top_num).data(),
			CURLFORM_END);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
			return is_success;
		}
		json_result = identify_result;
		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
	return is_success;
}