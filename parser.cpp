#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define start_position 8 // idemo do prvog imena neke valute
#define market_size 2800

struct price_pair {
    string name_sell;
    string name_buy;
    long price;
    long volume;
    int tick;
};

void parseGetAllPairs(string text, price_pair* market) {
    int position = start_position;
    int prev_position = 0;
    for(int mark=0; mark < market_size; ++mark) {
        prev_position = position;
        while(text[position] != ',') {
            ++position;
        }
        market[mark].name_sell = text.substr(prev_position, position - prev_position);
        //cout << text.substr(prev_position, position - prev_position) << " ";
        ++position;
        prev_position = position;
        while(text[position] != '"')
            ++position;
        market[mark].name_buy = text.substr(prev_position, position - prev_position);
        //cout << text.substr(prev_position, position - prev_position) << " ";
        position += 2;
        prev_position = position;
        while(text[position] != ',')
            ++position;
        market[mark].price = stol(text.substr(prev_position, position - prev_position));
        //cout << text.substr(prev_position, position - prev_position) << "\n";
        market[mark].tick = -1;
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
        price_pair market[market_size];
        parseGetAllPairs(line, market);

        for(int i=0; i < market_size; ++i) {
            cout << market[i].name_sell << " " << market[i].name_buy << " " << market[i].price << " " << market[i].volume << endl;
        }
    }

    // Close the file
    file.close();

    return 0;
}