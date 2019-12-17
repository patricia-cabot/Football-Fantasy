// AUTORS: Patricia Cabot, Yinlena Xu

#include <iostream>
#include <vector>
#include <ctime>     /* clock() */
#include <fstream>   /* ifstream(), getline() */
#include <algorithm> /* sort() */

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
vector<Player> CANDIDATES;

// *******************************************************************************

/** Creates a file with the output */
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

/** Sorts Players by their(points)^3/price ratio */
bool comp1(const Player& p1, const Player& p2) {
    if(p1.price == 0) return false;
    if(p2.price == 0) return true;
    if(double(p1.points)*p1.points*p1.points/p1.price != double(p2.points)*p2.points*p2.points/p2.price)
        return double(p1.points)*p1.points*p1.points/p1.price > double(p2.points)*p2.points*p2.points/p2.price;
    return p1.points > p2.points;
}

/** Sorts Players by positions: por > def > mig > dav */
bool comp2(const Player& p1, const Player& p2) {
    if(p1.pos != p2.pos) {
        if(p1.pos == "por") return true;
        if(p2.pos == "por") return false;
        if(p1.pos == "dav") return false;
        if(p2.pos == "dav") return true;
        if(p1.pos == "def") return true;
        if(p2.pos == "def") return false;
    } return true;
}

/** Inserts a new player to alignment and updates the player's counter and the current expenses */
void update_alignment(vector<Player>& alignment, int& cont, int& expense, int i) {
    alignment.push_back(CANDIDATES[i]);
    cont++;
    expense += CANDIDATES[i].price;
}

/** Algorithm:
        1) Sort the players based on comp1
        2) Choose best legal players */
void greedy_alignment(char* argv) {
    sort(CANDIDATES.begin(), CANDIDATES.end(), comp1);
    vector<Player> alignment;
    int cont_por = 0, cont_def = 0, cont_mig = 0, cont_dav = 0, expense = 0;
    for(int i = 0; i < int(CANDIDATES.size()); i++) {
        if(alignment.size() == 11) break;
        else if(expense + CANDIDATES[i].price <= INPUT.T) {
            if(CANDIDATES[i].pos == "por" and cont_por < 1)
                update_alignment(alignment, cont_por, expense, i);
            else if(CANDIDATES[i].pos == "def" and cont_def < INPUT.N1)
                update_alignment(alignment, cont_def, expense, i);
            else if(CANDIDATES[i].pos == "mig" and cont_mig < INPUT.N2)
                update_alignment(alignment, cont_mig, expense, i);
            else if(CANDIDATES[i].pos == "dav" and cont_dav < INPUT.N3)
                update_alignment(alignment, cont_dav, expense, i);
        }
    }
    sort(alignment.begin(), alignment.end(), comp2);
    write_alignment(alignment, argv);
}

// *******************************************************************************

/* Reads the input restrictions */
void read_input(char* argv) {
    ifstream in(argv);
    int N1, N2, N3, T, J;
    while(not in.eof()) {
        in >> N1 >> N2 >> N3 >> T >> J;
        INPUT = {N1, N2, N3, T ,J};
    } in.close();
}

/* Returns the data base of the players */
void read_data(char* argv) {
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
        }
    } in.close();
}

// *******************************************************************************

int main(int argc, char** argv) {
    START = clock();
    read_input(argv[2]);
    read_data(argv[1]);
    greedy_alignment(argv[3]);
}
