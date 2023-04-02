#include <bits/stdc++.h>
#include <curl/curl.h>
#include "parser.cpp"

#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

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

string Balance(string user){
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

void trade(){
    string response = getAllPairs();
    price_pair market[market_size];
    parseGetAllPairs(response, market);

    vector<vector<price_pair> > graph = create_graph(market, market_len);

    unordered_map<string, double> dist;
    unordered_map<string, string> predecessor;

    for (auto& row : graph) {
        for (auto& edge : row) {
            dist[edge.name_sell] = INFINITY;
            dist[edge.name_buy] = INFINITY;
        }
    }

    string start_currency = "USDT"
    predecessor[start_currency] = "0000";
    dist[start_currency] = 0;

    int V = graph.size();
    for (int i = 1; i < V; ++i) {
        for (auto& row : graph) {
            for (auto& edge : row) {
                if (dist[edge.name_sell] + edge.price < dist[edge.name_buy]) {
                    if(dist[edge.name_sell] + edge.price >= 0 || edge.name_buy == start_currency) {
                        predecessor[edge.name_buy] = edge.name_sell;
                    }
                    dist[edge.name_buy] = dist[edge.name_sell] + edge.price;
                }
            }
        }
    }

    
    vector<string> negative_cycle;
    string curr_node = predecessor[start_currency];
    cout << start_currency << " ";
    if(curr_node != "0000") {
        negative_cycle.push_back(start_currency);
        while (find(negative_cycle.begin(), negative_cycle.end(), curr_node) == negative_cycle.end()) {
            cout << curr_node << " ";
            negative_cycle.push_back(curr_node);
            curr_node = predecessor[curr_node];
        }
        negative_cycle.push_back(curr_node);
        cout << curr_node << endl;
    }
}

const int tick = 30;
const int MAX = 1e5;



int main() {

    trade();

    return 0;

    string time[MAX];
    int cnt = 0, last_cnt = 0;
    int found_cnt = 0;

    auto start_time = high_resolution_clock::now();
    auto last_time = start_time;
    int step = 250;
    auto left = start_time, right = start_time;
    time[cnt] = getTime();

    bool found = false;
    while (true) {
        auto current_time = high_resolution_clock::now();
        auto elapsed_time_full = duration_cast<milliseconds>(current_time - start_time).count();
        auto elapsed_time_step = duration_cast<milliseconds>(current_time - last_time).count();

        if (!found && elapsed_time_step >= step) {
            time[++cnt] = getTime();
            if(time[cnt] != time[cnt - 1]){
                left = last_time;
                right = current_time;
                step /= 4;
                start_time = last_time;
                found = true;
                ++found_cnt;
                cout << "First tick detected!\n";
                cout << duration_cast<milliseconds>(right - left).count() << "\n";
            }
        }

        if(found && elapsed_time_full >= (1000 * tick) && elapsed_time_full <= (1000 * tick) + (4 * step) && elapsed_time_step >= step){
            time[++cnt] = getTime();
            if(time[cnt] != time[cnt - 1]){
                auto tick_time = high_resolution_clock::now();
                auto left = elapsed_time_full - step;
                auto right = elapsed_time_full;
                //cout << duration_cast<milliseconds>(right - left).count() << "\n";
                step /= 4;
                start_time = last_time;
                cout << "New tick detected!\n";
                cout << left << " " << right << "\n";
                ++found_cnt;
                if(found_cnt == 3){
                    start_time = tick_time;
                    break;
                }
            }
        }

        if(elapsed_time_step >= step){
            last_time = current_time;
        }

        // add a small sleep to avoid high CPU usage
        this_thread::sleep_for(milliseconds(10));
    }

    while (true) {
        auto current_time = high_resolution_clock::now();
        auto elapsed_time = duration_cast<milliseconds>(current_time - start_time).count();

        if (elapsed_time >= (1000 * tick)) {
            string time = getTime();
            start_time = current_time;
            cout << time << "\n";

            trade();
        }
        // add a small sleep to avoid high CPU usage
        this_thread::sleep_for(milliseconds(10));
    }







    return 0;
}

