// AUTORS: Patricia Cabot, Yinlena Xu

#include <iostream>
#include <vector>
#include <ctime>        /* clock() */
#include <fstream>      /* ifstream(), getline() */
#include <cstdlib>      /* rand() */
#include <algorithm>    /* find() */

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

/** Defining operator== for function find() */
bool operator==(const Player& p1, const Player& p2) {
    return p1.name == p2.name;
}

/** SIMULATED ANNEALING PARAMETERS */
double T = 1;                       // Temperature
const double Tmin = 0.00001;        // Temperature at which iteration terminates
const double alpha = 0.95;          // Decrease in temperature
const int numIterations = 100000;    // Number of iterations of annealing before decreasing temperature

/** GLOBAL VARIABLES */
time_t START;
Input INPUT;
vector<Player> POR, DEF, MIG, DAV;

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
        f << alignment[i].name << " " << alignment[i].pos << " " << alignment[i].price;
        points += alignment[i].points;
        price  += alignment[i].price;
    } f << endl;
    f << "Punts: " << points << endl;
    f << "Preu: "  << price  << endl;
    f.close();
}

// *******************************************************************************

/** Given a candidates vector of players, an alignment vector and the position to change,
    it choses a random player and updates the alignment under the given price condition */
void update_player(vector<Player>& CAND, vector<Player>& alignment, int p, int price) {
    int i;
    bool updated = false;
    while(not updated) {
        i = rand()%(CAND.size()+1);
        if(find(alignment.begin(), alignment.end(), CAND[i]) == alignment.end() and price+CAND[i].price <= INPUT.T) {
            alignment[p] = CAND[i];
            updated = true;
            return;
        }
    }
}

/** Generates the inicial alignment */
vector<Player> generate_alignment(int& alignment_points, int& alignment_price) {
    vector<Player> alignment(11);
    alignment[0] = POR[rand()%(POR.size()+1)];
    alignment_points = alignment[0].points;
    alignment_price = alignment[0].price;
    for(int i = 1; i < 11; ++i) {
        if(i <= INPUT.N1) update_player(DEF, alignment, i, alignment_price);
        else if(i <= INPUT.N1+INPUT.N2) update_player(MIG, alignment, i, alignment_price);
        else update_player(DAV, alignment, i, alignment_price);
        alignment_points += alignment[i].points;
        alignment_price += alignment[i].price;
    }
    return alignment;
}

/** Generates a neighbour alignment
    A neighbourhood is made of all alignments that differ in only one player
    from the initial alignment */
void random_update(vector<Player>& neighbour_alignment, int& neighbour_points, int& neighbour_price, int p) {
    if(neighbour_alignment[p].pos == "por") update_player(POR, neighbour_alignment, p, neighbour_price);
    else if(neighbour_alignment[p].pos == "def") update_player(DEF, neighbour_alignment, p, neighbour_price);
    else if(neighbour_alignment[p].pos == "mig") update_player(MIG, neighbour_alignment, p, neighbour_price);
    else update_player(DAV, neighbour_alignment, p, neighbour_price);
    neighbour_points += neighbour_alignment[p].points;
    neighbour_price += neighbour_alignment[p].price;
}

/** Picks a random neighbour */
vector<Player> pick_at_random(const vector<Player>& alignment, int& neighbour_points, int& neighbour_price) {
    int p = rand()%(11);
    neighbour_points -= alignment[p].points;
    neighbour_price -= alignment[p].price;
    vector<Player> neighbour_alignment = alignment;
    random_update(neighbour_alignment, neighbour_points, neighbour_price, p);
    return neighbour_alignment;
}

/** Updates the temperature following a geometric law such that
    T_(k+1) = α*T_k , with α ∈ [0, 1] */
void update(double& T) {
    T *= alpha;
}

/** Uses Simulated annealing metaheuristic algorithm to get the optim alignment */
void get_alignment(char* argv) {
    int alignment_points;
    int alignment_price;
    vector<Player> alignment = generate_alignment(alignment_points, alignment_price);
    while(T > Tmin) {
        for(int i = 0; i < numIterations; ++i) {
            int neighbour_points = alignment_points;
            int neighbour_price = alignment_price;
            vector<Player> neighbour_alignment = pick_at_random(alignment, neighbour_points, neighbour_price);
            if(neighbour_points > alignment_points or rand() < T) {
                alignment = neighbour_alignment;
                alignment_points = neighbour_points;
                alignment_price = neighbour_price;
                write_alignment(alignment, argv);
            }
        }
        update(T);
    }
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
                 if(pos == "por") POR.push_back({name, pos, price, club, points});
            else if(pos == "def") DEF.push_back({name, pos, price, club, points});
            else if(pos == "mig") MIG.push_back({name, pos, price, club, points});
            else DAV.push_back({name, pos, price, club, points});
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
