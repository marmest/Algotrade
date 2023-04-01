#include<bits/stdc++.h>
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

string Register(string user){
    string request_name = "register/" + user;
    string response = getRequest(request_name);
    return response;
}

string resetBalance(string user, string secret){
    string request_name = "resetBalance/" + user + "/" + secret;
    string response = getRequest(request_name);
    return response;
}

string balance(string user){
    string request_name = "balance/" + user;
    string response = getRequest(request_name);
    return response;
}

string getPairs(int len, string pairs[]){
    string request_name = "getPairs/";
    for(int i = 0;i < len; i++){
        request_name += pairs[i];
        if(i < len - 1){
            request_name += "|";
        }
    }
    string response = getRequest(request_name);
    return response;
}

string getHistoricalPairs(int time, int len, string pairs[]){
    string request_name = "getHistoricalPairs/" + to_string(time) + "/";
    for(int i = 0;i < len; i++){
        request_name += pairs[i];
        if(i < len - 1){
            request_name += "|";
        }
    }
    string response = getRequest(request_name);
    return response;
}

string createOrders(string user, string secret, int len, string trades[]){
    string request_name = "createOrders/" + user + "/" + secret + "/";
    for(int i = 0;i < len; i++){
        request_name += trades[i];
        if(i < len - 1){
            request_name += "|";
        }
    }
    string response = getRequest(request_name);
    return response;
}



int main() {

    //string getTimeResponse = getTime();

    //string getAllPairsResponse = getAllPairs();

    //string RegisterResponse = Register("pzspmf");

    // tu moram odrediti stvarni secret
    //string resetBalanceResponse = resetBalance("pzspmf", "e0974e7c3a81ca56b9fca2445907ed0f");

    //string balanceResponse = balance("pzspmf");

    int len = 6;
    string pairs[len];

    pairs[0] = "MBOX,BTC";
    pairs[1] = "BTC,MBOX";
    pairs[2] = "VET,BTC";
    pairs[3] = "BTC,VET";
    pairs[4] = "MBOX,BNB";
    pairs[5] = "BNB,MBOX";

    //string getPairsResponse = getPairs(len, pairs);

    int time = 10;
    //string getHistoricalPairsResponse = getHistoricalPairs(10, len, pairs);

    string user = "pzspmf";
    string secret = "e0974e7c3a81ca56b9fca2445907ed0f";
    string trades[len];

    trades[0] = "USDT,BTC,20000000000";
    trades[1] = "USDT,ETH,20000000000";
    trades[2] = "USDT,XRP,20000000000";
    trades[3] = "USDT,ADA,20000000000";
    trades[4] = "USDT,SOL,10000000000";
    trades[5] = "USDT,MATIC,10000000000";

    //string createOrdersResponse = createOrders(user, secret, len, trades);

    //cout << getTimeResponse << "\n";
    //cout << getAllPairsResponse << "\n";
    //cout << RegisterResponse << "\n";
    //cout << resetBalanceResponse << "\n";
    //cout << balanceResponse << "\n";
    //cout << getPairsResponse << "\n";
    //cout << getHistoricalPairsResponse << "\n";
    //cout << createOrdersResponse << "\n";

    return 0;
}
