// AUTORS: Patricia Cabot, Yinlena Xu

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <limits>

using namespace std;

// *******************************************************************************

struct Input {
    int N1, N2, N3, T, J;
};

struct Player {
    string name;
    string pos;
    int price;
    string club;
    int points;
};

// GLOBAL VARIABLES
time_t START;
Input INPUT;
int MAX_POINTS;
int MIN_PRICE;
vector<Player> CANDIDATES;

// *******************************************************************************

// Creates a file with the output
void write_alignment(const vector<Player>& alignment, char* argv) {
    ofstream f(argv);
    f << float(clock() - START)/CLOCKS_PER_SEC << endl;
    int points = 0, price = 0;
    int n2 = INPUT.N1+1;
    int n3 = n2+INPUT.N2;
    bool first = true;
    for(int i = 0; i < 11; ++i) {
        if(i == 0) f << "POR: ";
        if(i ==  1) {f << endl << "DEF: ";  first = true;}
        if(i == n2) {f << endl << "MIG: ";  first = true;}
        if(i == n3) {f << endl << "DAV: ";  first = true;}
        if(not first) f << ";";
        first = false;
        f << alignment[i].name;
        points += alignment[i].points;
        price  += alignment[i].price;
    } f << endl;
    f << "Punts: " << points << endl;
    f << "Preu: "  << price  << endl;
    f.close();
}

// *******************************************************************************


// *******************************************************************************

// Reads the input restrictions
void read_input(char* argv) {
    ifstream in(argv);
    int N1, N2, N3, T, J;
    while(not in.eof()) {
        in >> N1 >> N2 >> N3 >> T >> J;
        INPUT = {N1, N2, N3, T ,J};
    } in.close();
}

// Returns the data base of the players
void read_data(char* argv) {
    MIN_PRICE  = numeric_limits<int>::max();
    MAX_POINTS = 0;

    ifstream in(argv);
    while(not in.eof()) {
        string name, pos, club;
        int points, price;
        getline(in,name,';');    if(name == "") break;
        getline(in,pos, ';');
        in >> price;
        char aux; in >> aux;
        getline(in,club,';');
        in >> points;
        string aux2;
        getline(in,aux2);
        if(price <= INPUT.J) {
            CANDIDATES.push_back({name, pos, price, club, points});
            if(price  < MIN_PRICE and price != 0) MIN_PRICE = price;
            if(points > MAX_POINTS) MAX_POINTS = points;
        }
    } in.close();
}

// *******************************************************************************

int main(int argc, char** argv) {
    START = clock();
    read_input(argv[2]);
    read_data(argv[1]);
    //get_alignment(argv[3]);
}
