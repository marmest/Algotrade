/*
#include <iostream>
#include <curl/curl.h>

using namespace std;

int main() {
    CURL *curl;
    CURLcode res;
 
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.101:3000/getTime");
        res = curl_easy_perform(curl);
        cout << "\n" << res << "\n";
 
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
 
        curl_easy_cleanup(curl);
    }
 
    return 0;
}
*/
#include <iostream>
#include <string.h>
#include <curl/curl.h>

using namespace std;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    ((string*)userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

string getRequest(string request_name){
    CURL *curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();
    if (curl) {
        string request = "http://192.168.1.101:3000/" + request_name;
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

string getTime(){
    string response = getRequest("getTime");
    return response;
}

string getAllPairs(){
    string response = getRequest("getAllPairs");
    return response;
}

int main() {
    /*
    CURL *curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.101:3000/getTime");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
    }

    cout << response << "\n";
    */

    string time = getTime();

    string all_pairs = getAllPairs();

    cout << time << "\n" << all_pairs << "\n";

    return 0;
}
