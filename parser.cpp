#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define start_position 8 // idemo do prvog imena neke valute
#define market_size 4000
#define num_of_currencies 500

struct price_pair {
    string name_sell;
    string name_buy;
    long price;
    long volume;
};

struct balance {
    string currency;
    long amount;
};


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
        market[mark].price = stol(text.substr(prev_position, position - prev_position));
        
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

/*
int main()
{
    // Open the file
    ifstream file("responses/getPairs.txt");

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
        price_pair wallet[6];
        parseGetPairs(line, wallet, 6);

        for(int i=0; i < 6; ++i) {
            cout << wallet[i].name_buy << " " << wallet[i].name_sell 
            << " " << wallet[i].price << " " << wallet[i].volume << endl;
        }
    }

    // Close the file
    file.close();

    return 0;
}
*/