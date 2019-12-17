// AUTORS: Patricia Cabot, Yinlena Xu

#include <iostream>
#include <vector>
#include <ctime>        /* clock() */
#include <fstream>      /* ifstream(), getline() */
#include <limits>       /* numeric_limits<int>::max() */

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

/** Converts the chosen vector into an alignment */
vector<Player> chosen_to_alignment(const vector<int>& chosen) {
    int n = chosen.size();
    vector<Player> alignment;
    for(int i = 0; i < n; ++i) if(chosen[i]) alignment.push_back(CANDIDATES[i]);
    return alignment;
}

/** Generates all possible alignments on a vector<int>
    i: position in the vector
    cont_: keeps track of how many players of each kind */
void generate_alignment(vector<int>& chosen, int i, int cont0, int contpor, int contdef, int contmig, int contdav,
                        int price, int points, int& best_points, vector<Player>& best_alignment, char* argv) {

    int n = chosen.size();
    int cont1 = contpor+contdef+contmig+contdav;

    if(cont1 == 11 and points > best_points) {
            best_points = points;
            best_alignment = chosen_to_alignment(chosen);
            write_alignment(best_alignment, argv);
    }
    else if(i < n) {
        int price_i  = price+CANDIDATES[i].price;
        int points_i = points+CANDIDATES[i].points;
        int price_bound  = price_i+(10-cont1)*MIN_PRICE;
        int points_bound = points_i+(10-cont1)*MAX_POINTS;
        if(price_bound <= INPUT.T and points_bound > best_points) {
            chosen[i] = 1;
            if(CANDIDATES[i].pos == "por" and contpor < 1)
                generate_alignment(chosen, i+1, cont0, contpor+1, contdef, contmig, contdav, price_i, points_i, best_points, best_alignment, argv);
            if(CANDIDATES[i].pos == "def" and contdef < INPUT.N1)
                generate_alignment(chosen, i+1, cont0, contpor, contdef+1, contmig, contdav, price_i, points_i, best_points, best_alignment, argv);
            if(CANDIDATES[i].pos == "mig" and contmig < INPUT.N2)
                generate_alignment(chosen, i+1, cont0, contpor, contdef, contmig+1, contdav, price_i, points_i, best_points, best_alignment, argv);
            if(CANDIDATES[i].pos == "dav" and contdav < INPUT.N3)
                generate_alignment(chosen, i+1, cont0, contpor, contdef, contmig, contdav+1, price_i, points_i, best_points, best_alignment, argv);
        }
        if(cont0 < n-11) {
            chosen[i] = 0;
            generate_alignment(chosen, i+1, cont0+1, contpor, contdef, contmig, contdav, price, points, best_points, best_alignment, argv);
        }
    }
}

void get_alignment(char* argv) {
    int n = CANDIDATES.size();
    vector<int> chosen(n);          // From all players, the ones chosen for the alignment
    int best_points = 0;
    vector<Player> best_alignment;  // the best alignment found at the moment
    generate_alignment(chosen, 0, 0, 0, 0, 0, 0, 0, 0, best_points, best_alignment, argv);
}

// *******************************************************************************

/** Reads the input restrictions */
void read_input(char* argv) {
    ifstream in(argv);
    int N1, N2, N3, T, J;
    while(not in.eof()) {
        in >> N1 >> N2 >> N3 >> T >> J;
        INPUT = {N1, N2, N3, T ,J};
    } in.close();
}

/** Returns the data base of the players */
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
    get_alignment(argv[3]);
}
