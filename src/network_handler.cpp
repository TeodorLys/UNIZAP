#include "network_handler.h"
#include <string>
#include <fstream>
#include "error_handler.h"

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
	((std::string*) stream)->append((const char*)ptr, (size_t)size * nmemb);
	return size * nmemb;
}

bool network_handler::upload_to_dropbox(std::filesystem::path path, std::string access) {
	CURL* curl;
	CURLcode res = CURLE_READ_ERROR;
	struct curl_slist* list = 0;
	std::string _path = path.string();
	std::fstream f(_path, std::fstream::in | std::fstream::binary);
	if (!f.is_open()) {
		error_handler::call_error_and_exit("Could not open file...exiting..." + _path);
	}

	std::string content;
	content.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	f.close();

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	std::string out;
	std::string args = "Dropbox-API-Arg: {\"path\":\"/";
	args += path.filename().string() + "\", \"mode\":{\".tag\":\"overwrite\"}}";
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");
		std::string c_access = "Authorization: Bearer " + access;
		list = curl_slist_append(list, c_access.c_str());
		list = curl_slist_append(list, "Content-Type: application/octet-stream");
		list = curl_slist_append(list, args.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, content.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			printf("Could not connect to the internet!\n");
			return false;
		}
	}
	/*
	Formats the dropbox api response
	and prints it :)
	*/
	for (char c : out) {
		if (c == '}')
			printf("\n");
		printf("%c", c);
		if (c == ',' || c == '{')
			printf("\n");
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return true;
}