#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

#define start_position 8 // idemo do prvog imena neke valute
#define market_size 2800
#define num_of_currencies 500

struct price_pair {
    string name_sell;
    string name_buy;
    double price;
    long volume;
};


struct balance {
    string currency;
    long amount;
};


void swap(price_pair& a, price_pair& b) {
    price_pair temp = a;
    a = b;
    b = temp;
}


void quicksort_name_sell(price_pair* pairs, int start, int end) {
    if (start < end) {
        // Choose a pivot element
        int pivot = end;

        // Partition the array around the pivot
        int i = start - 1;
        for (int j = start; j < end; j++) {
            if (pairs[j].name_sell >= pairs[pivot].name_sell) {
                i++;
                swap(pairs[i], pairs[j]);
            }
        }
        swap(pairs[i+1], pairs[pivot]);
        pivot = i+1;

        // Recursively sort the sub-arrays
        quicksort_name_sell(pairs, start, pivot-1);
        quicksort_name_sell(pairs, pivot+1, end);
    }
}


void quicksort_volume(price_pair* pairs, int start, int end) {
    if (start < end) {
        // Choose a pivot element
        int pivot = end;

        // Partition the array around the pivot
        int i = start - 1;
        for (int j = start; j < end; j++) {
            if (pairs[j].volume >= pairs[pivot].volume) {
                i++;
                swap(pairs[i], pairs[j]);
            }
        }
        swap(pairs[i+1], pairs[pivot]);
        pivot = i+1;

        // Recursively sort the sub-arrays
        quicksort_name_sell(pairs, start, pivot-1);
        quicksort_name_sell(pairs, pivot+1, end);
    }
}


vector<vector<price_pair>> create_graph(price_pair* pairs, int n) {
    vector<vector<price_pair>> sales_list;
    string curr_name_sell = pairs[0].name_sell;
    vector<price_pair> curr_list;
    for (int i = 0; i < n; i++) {
        if (pairs[i].name_sell != curr_name_sell) {
            sales_list.push_back(curr_list);
            curr_list.clear();
            curr_name_sell = pairs[i].name_sell;
        }
        price_pair curr_sale = {pairs[i].name_sell, pairs[i].name_buy, pairs[i].price, pairs[i].volume};
        curr_list.push_back(curr_sale);
    }
    sales_list.push_back(curr_list);
    return sales_list;
}


// parametri: json string, polje strukture price_pair
// vraca: ispunjeno polje strukture price_pair
void parseGetAllPairs(string text, price_pair* market) {
    int position = start_position;
    int prev_position = 0;
    for(int mark=0; mark < market_size; ++mark) {
        prev_position = position;
        while(text[position] != ',') {
            ++position;
        }
        market[mark].name_sell = text.substr(prev_position, position - prev_position);

        ++position;
        prev_position = position;
        while(text[position] != '"')
            ++position;
        market[mark].name_buy = text.substr(prev_position, position - prev_position);

        position += 2;
        prev_position = position;
        while(text[position] != ',')
            ++position;
        market[mark].price = log10(stol(text.substr(prev_position, position - prev_position)));
        
        prev_position = position;
        while(text[position] != '_')
            ++position;
        ++position;
        if(text.substr(prev_position, position - prev_position) != ",\"close_") break; 
    }
    
    for(int mark=0; mark < market_size; ++mark) {
        while(text[position] != ':') {
            ++position;
        }

        prev_position = position + 1;
        while(text[position] != ',' && text[position] != '}')
            ++position;
        if(text[position] == '}') {
            market[mark].volume = stol(text.substr(prev_position, position - prev_position));
            break;
        }
        market[mark].volume = stol(text.substr(prev_position, position - prev_position));
    }
    return;
}


// parametri: json string, polje strukture balance
// vraca: ispunjeno polje strukture balance
void parseBalance(string text, balance* wallet) {
    int prev_position = 0;
    int position = 0;
    for(int mark=0; mark < num_of_currencies; ++mark) {
        while(text[position] != '"') {
            ++position;
        }
        prev_position = ++position;
        while(text[position] != '"') {
            ++position;
        }
        wallet[mark].currency = text.substr(prev_position, position - prev_position);

        while(text[position] != ':')
            ++position;
        prev_position = position + 1;
        while(text[position] != ',' && text[position] != '}')
            ++position;
        if(text[position] == '}') {
            wallet[mark].amount = stol(text.substr(prev_position, position - prev_position));
            break;
        }
        wallet[mark].amount = stol(text.substr(prev_position, position - prev_position));
    }
    return;
}


void parseGetPairs(string text, price_pair* market, int num_of_pairs) {
    int position = start_position;
    int prev_position = 0;
    for(int mark=0; mark < num_of_pairs; ++mark) {
        prev_position = position;
        while(text[position] != ',') {
            ++position;
        }
        market[mark].name_sell = text.substr(prev_position, position - prev_position);
        ++position;
        prev_position = position;

        while(text[position] != '"')
            ++position;
        market[mark].name_buy = text.substr(prev_position, position - prev_position);
        position += 2;
        prev_position = position;

        while(text[position] != ',')
            ++position;
        market[mark].price = stol(text.substr(prev_position, position - prev_position));

        while(text[position] != ':')
            ++position;
        prev_position = ++position;
        while(text[position] != ',' && text[position] != '}')
            ++position;
        if(text[position] == '}') {
            market[mark].volume = stol(text.substr(prev_position, position - prev_position));
            break;
        }
        market[mark].volume = stol(text.substr(prev_position, position - prev_position)); 
        position += 8;
    }
    return;
}



int main()
{
    // Open the file
    ifstream file("responses/getAllPairs.txt");

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        cout << "Failed to open file\n";
        return 1;
    }

    // Read the file line by line and print each line to the console
    string line;
    while (getline(file, line))
    {
        price_pair wallet[market_size];
        parseGetAllPairs(line, wallet);

        quicksort_volume(wallet, 0, market_size-1);
        int new_size = 500;
        quicksort_name_sell(wallet, 0, new_size-1);
        vector<vector<price_pair>> graf = create_graph(wallet, new_size);

        int brojac=0;
        for(int i=0; i < graf.size(); ++i) {
            cout << graf[i][0].name_sell << endl;
            for(int j=0; j < graf[i].size(); ++j) {
                cout << graf[i][j].name_buy << " " << graf[i][j].price << " " << graf[i][j].volume << endl;
                ++brojac;
            }
           // cout << wallet[i].name_sell << " " << wallet[i].name_buy << " " << wallet[i].price << " " << wallet[i].volume << endl;
        }

    }

    // Close the file
    file.close();

    return 0;
}