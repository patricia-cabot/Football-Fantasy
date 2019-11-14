#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <limits>
#include <algorithm>

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

bool comparator(const Player& p1, const Player& p2) {
    if(p1.points != p2.points) return p1.points > p2.points;
    return p1.price < p2.price;
}

// GLOBAL VARIABLES
time_t START;
Input INPUT;
int MAX_POINTS_POR;
int MAX_POINTS_DEF;
int MAX_POINTS_MIG;
int MAX_POINTS_DAV;
int MIN_PRICE_POR;
int MIN_PRICE_DEF;
int MIN_PRICE_MIG;
int MIN_PRICE_DAV;
vector<Player> CANDIDATES;

// *******************************************************************************

void print_alignment(const vector<Player>& alignment) {
    cout << float(clock() - START)/CLOCKS_PER_SEC << endl;
    int points = 0, price = 0;
    int n2 =  1+INPUT.N1;
    int n3 = n2+INPUT.N2;
    bool first = true;
    for(int i = 0; i < 11; ++i) {
        if(i ==  0)  cout << "POR: ";
        if(i ==  1) {cout << endl << "DEF: "; first = true;}
        if(i == n2) {cout << endl << "MIG: "; first = true;}
        if(i == n3) {cout << endl << "DAV: "; first = true;}
        if(not first) cout << ";";
        first = false;
        cout << alignment[i].name;
        points += alignment[i].points;
        price  += alignment[i].price;
    } cout << endl;
    cout << "Punts: " << points << endl;
    cout << "Preu: "  << price  << endl;
}

// Creates a file with the output
void write_alignment(const vector<Player>& alignment) {
    ofstream f("output.txt");
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

// Converts the chosen vector into an alignment
vector<Player> chosen_to_alignment(const vector<int>& chosen) {
    int n = chosen.size();
    vector<Player> alignment;
    for(int i = 0; i < n; ++i) if(chosen[i]) alignment.push_back(CANDIDATES[i]);
    return alignment;
}

// Generates possible alignments and choses the best one
void generate(vector<int>& chosen, int i, int cont0, int contpor, int contdef, int contmig, int contdav,
            int price, int points, int& best_points, vector<Player>& best_alignment) {

    int n = chosen.size();
    int cont1 = contpor+contdef+contmig+contdav;

    if(cont1 == 11 and points > best_points) {
            best_points = points;
            best_alignment = chosen_to_alignment(chosen);
            write_alignment(best_alignment);
    }
    else if(i < n) {
        int price_i  = price+CANDIDATES[i].price;
        int points_i = points+CANDIDATES[i].points;
        int price_bound  = price_i+MIN_PRICE_POR+(INPUT.N1-contdef)*MIN_PRICE_DEF+(INPUT.N2-contmig)*MIN_PRICE_MIG+(INPUT.N3-contdav)*MIN_PRICE_DAV;
        int points_bound = points_i+MAX_POINTS_POR+(INPUT.N1-contdef)*MAX_POINTS_DEF+(INPUT.N2-contmig)*MAX_POINTS_MIG+(INPUT.N3-contdav)*MAX_POINTS_DAV;
             if(CANDIDATES[i].pos == "por") {price_bound -= MIN_PRICE_POR ;     points_bound -= MAX_POINTS_POR;}
        else if(CANDIDATES[i].pos == "def") {price_bound -= MIN_PRICE_DEF ;     points_bound -= MAX_POINTS_DEF;}
        else if(CANDIDATES[i].pos == "mig") {price_bound -= MIN_PRICE_MIG ;     points_bound -= MAX_POINTS_MIG;}
        else if(CANDIDATES[i].pos == "dav") {price_bound -= MIN_PRICE_DAV ;     points_bound -= MAX_POINTS_DAV;}

        if(price_bound <= INPUT.T and points_bound > best_points) {
            chosen[i] = 1;
            if(CANDIDATES[i].pos == "por" and contpor < 1)
                generate(chosen, i+1, cont0, contpor+1, contdef, contmig, contdav, price_i, points_i, best_points, best_alignment);
            if(CANDIDATES[i].pos == "def" and contdef < INPUT.N1)
                generate(chosen, i+1, cont0, contpor, contdef+1, contmig, contdav, price_i, points_i, best_points, best_alignment);
            if(CANDIDATES[i].pos == "mig" and contmig < INPUT.N2)
                generate(chosen, i+1, cont0, contpor, contdef, contmig+1, contdav, price_i, points_i, best_points, best_alignment);
            if(CANDIDATES[i].pos == "dav" and contdav < INPUT.N3)
                generate(chosen, i+1, cont0, contpor, contdef, contmig, contdav+1, price_i, points_i, best_points, best_alignment);
        }
        if(cont0 < n-11) {
            chosen[i] = 0;
            generate(chosen, i+1, cont0+1, contpor, contdef, contmig, contdav, price, points, best_points, best_alignment);
        }
    }
}

void generate_alignment() {
    int n = CANDIDATES.size();
    vector<int> chosen(n); // From all players, the ones chosen for the alignment
    int best_points = 0;
    vector<Player> best_alignment; // the best alignment found at the moment
    generate(chosen, 0, 0, 0, 0, 0, 0, 0, 0, best_points, best_alignment);

    //----------------------------------------------------------------------------BORRAR ESTO LUEGO
    print_alignment(best_alignment);
}

// Gets the best alignment of players given input
void get_alignment(const vector<Player>& players) {
    MIN_PRICE_POR  = MIN_PRICE_DEF  = MIN_PRICE_MIG  = MIN_PRICE_DAV  = numeric_limits<int>::max();
    MAX_POINTS_POR = MAX_POINTS_DEF = MAX_POINTS_MIG = MAX_POINTS_DAV = 0;
    for(auto p : players) if(p.price <= INPUT.J) {
        CANDIDATES.push_back(p);
        if(p.price != 0) {
                 if(p.pos == "por" and p.price  < MIN_PRICE_POR) MIN_PRICE_POR = p.price;
            else if(p.pos == "def" and p.price  < MIN_PRICE_DEF) MIN_PRICE_DEF = p.price;
            else if(p.pos == "mig" and p.price  < MIN_PRICE_MIG) MIN_PRICE_MIG = p.price;
            else if(p.pos == "dav" and p.price  < MIN_PRICE_DAV) MIN_PRICE_DAV = p.price;
        }
             if(p.pos == "por" and p.points > MAX_POINTS_POR) MAX_POINTS_POR = p.points;
        else if(p.pos == "def" and p.points > MAX_POINTS_DEF) MAX_POINTS_DEF = p.points;
        else if(p.pos == "mig" and p.points > MAX_POINTS_MIG) MAX_POINTS_MIG = p.points;
        else if(p.pos == "dav" and p.points > MAX_POINTS_DAV) MAX_POINTS_DAV = p.points;
    }
    generate_alignment();
}

// *******************************************************************************

// Returns the data base of the players
vector<Player> read_data(char* argv) {
    vector<Player> players;
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
        players.push_back({name, pos, price, club, points});
    } in.close();
    return players;
}

// Reads the input restrictions
void read_input(char* argv) {
    ifstream in(argv);
    int N1, N2, N3, T, J;
    while(not in.eof()) {
        in >> N1 >> N2 >> N3 >> T >> J;
        INPUT = {N1, N2, N3, T ,J};
    } in.close();
}

// *******************************************************************************

/** Input: ./a.out data_base.txt Projecte/public_benchs/easy-1.txt
    Input (pequeña): ./a.out data.txt easy-0.txt */
int main(int argc, char** argv) {
    START = clock();
    vector<Player> players = read_data(argv[1]);
    read_input(argv[2]);
    get_alignment(players);
}
