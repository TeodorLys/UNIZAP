#include "network_handler.h"
#include <string>
#include <fstream>
#include "user_communication/error_handler.h"

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
		error_handler::call_error_and_exit("[RUNTIME_ERROR]Could not open file...exiting..." + _path);
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
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());

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

bool network_handler::download_from_dropbox(std::filesystem::path path, std::string output,std::string access) {
	CURL* curl;
	CURLcode res;
	struct curl_slist* list = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	std::string out;
	std::string args = "Dropbox-API-Arg: {\"path\":\"/";  // Dropbox:es api, default stuff...
	if (output.find(path.filename().string()) == std::string::npos)
		output += "/" + path.filename().string();
	/*
	If the request was NOT successful, I still want the program to continue.
	Butt? if it WAS successful, cleanup and create the file.
	*/
	bool success = false;

	args += path.string() + "\"}";
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/download");
		//yes i know... should really not use a "static" access token, but github was not very reliable...
		std::string _a = "Authorization: Bearer " + access;
		list = curl_slist_append(list, _a.c_str());
		list = curl_slist_append(list, "Content-Type:");
		list = curl_slist_append(list, args.c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			return false;
		}
		else {
			success = true;
		}
	}
	else {
		return false;
	}
	if (success) {
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		if (out.find("error_summary") != std::string::npos) {
			for (char c : out) {
				if (c == '}')
					printf("\n");
				printf("%c", c);
				if (c == ',' || c == '{')
					printf("\n");
			}
			error_handler::call_error_and_exit("\n[DROPBOX_ERROR] Could not find file");
		}
		std::fstream f;
		f.open(output, std::fstream::binary | std::fstream::out);
		f << out;
		f.close();
		return true;
	}
	return false;
}