// AUTORS: Patricia Cabot, Yinlena Xu

#include <iostream>     /* cin, cout */
#include <vector>       /* find() */
#include <ctime>        /* clock() */
#include <fstream>      /* ifstream(), getline() */
#include <algorithm>    /* sort() */
#include <cstdlib>      /* rand() */

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

/** Defining operator== to use function find() */
bool operator==(const Player& p1, const Player& p2) {
    return p1.name == p2.name;
}

/** SIMULATED ANNEALING PARAMETERS */
double T = 1;                       // Temperature
const double Tmin = 0.00001;        // Temperature at which iteration terminates
const double alpha = 0.95;          // Decrease in temperature
const int numIterations = 10000;    // Number of iterations of annealing before decreasing temperature

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
        f << alignment[i].name << " " << alignment[i].pos;
        points += alignment[i].points;
        price  += alignment[i].price;
    } f << endl;
    f << "Punts: " << points << endl;
    f << "Preu: "  << price  << endl;
    f.close();
}

// *******************************************************************************

/** Generates the inicial alignment */
vector<Player> generate_alignment(int& alignment_points) {
    vector<Player> alignment(11);
    alignment[0] = POR[rand()%(POR.size()+1)];
    alignment_points = alignment[0].points;
    for(int i = 1; i < 11; ++i) {
             if(i <= INPUT.N1) alignment[i] = DEF[rand()%(DEF.size()+1)];
        else if(i <= INPUT.N1+INPUT.N2) alignment[i] = MIG[rand()%(MIG.size()+1)];
        else alignment[i] = DAV[rand()%(DAV.size()+1)];
        alignment_points += alignment[i].points;
    }
    return alignment;
}

/** Generates a neighbour alignment
    A neighbourhood is made of all alignments that differ in only one player
    from the initial alignment */
void random_update(vector<Player>& neighbour_alignment, int& neighbour_points, int p) {
    bool updated = false;
    int i;
    if(neighbour_alignment[p].pos == "por") {
        while(not updated) {
            i = rand()%(POR.size()+1);
            if(find(neighbour_alignment.begin(), neighbour_alignment.end(), POR[i]) == neighbour_alignment.end()) {
                neighbour_alignment[p] = POR[i];
                updated = true;
    }   }   }
    else if(neighbour_alignment[p].pos == "def") {
        while(not updated) {
            i = rand()%(DEF.size()+1);
            if(find(neighbour_alignment.begin(), neighbour_alignment.end(), DEF[i]) == neighbour_alignment.end()) {
                neighbour_alignment[p] = DEF[i];
                updated = true;
    }   }   }
    else if(neighbour_alignment[p].pos == "mig") {
        while(not updated) {
            i = rand()%(MIG.size()+1);
            if(find(neighbour_alignment.begin(), neighbour_alignment.end(), MIG[i]) == neighbour_alignment.end()) {
                neighbour_alignment[p] = MIG[i];
                updated = true;
    }   }   }
    else {
        while(not updated) {
            i = rand()%(DAV.size()+1);
            if(find(neighbour_alignment.begin(), neighbour_alignment.end(), DAV[i]) == neighbour_alignment.end()) {
                neighbour_alignment[p] = DAV[i];
                updated = true;
    }   }   }
    neighbour_points += neighbour_alignment[p].points;
}

/** Picks a random neighbour */
vector<Player> pick_at_random(const vector<Player>& alignment, int& neighbour_points) {
    int p = rand()%(11);
    neighbour_points -= alignment[p].points;
    vector<Player> neighbour_alignment = alignment;
    random_update(neighbour_alignment, neighbour_points, p);
    return neighbour_alignment;
}

/** Updates the temperature following a geometric law such that
    T_(k+1) = α*T_k , with α ∈ [0, 1] */
void update(double& T) {
    T *= alpha;
}

void get_alignment(char* argv) {
    int alignment_points;
    vector<Player> alignment = generate_alignment(alignment_points);
    while(T > Tmin) {
        for(int i = 0; i < numIterations; ++i) {
            int neighbour_points = alignment_points;
            vector<Player> neighbour_alignment = pick_at_random(alignment, neighbour_points);
            if(neighbour_points > alignment_points or rand() < T) {
                alignment = neighbour_alignment;
                alignment_points = neighbour_points;
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
