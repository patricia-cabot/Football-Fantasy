#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <limits>

using namespace std;

/** */
struct Input {
    int N1, N2, N3, T, J;
};


/** */
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
int MAX_POINT;
int MIN_PRICE;
vector<Player> candidates;

/** */
void print_alignment(const vector<Player>& alignment) {
    int points = 0, price = 0;
    int N2 = INPUT.N1+1;
    int N3 = N2+INPUT.N2;
    bool first = true;
    for(int i = 0; i < 11; ++i) {
        if(i == 0) cout << "POR: ";
        if(i ==  1) {cout << endl << "DEF: "; first = true;}
        if(i == N2) {cout << endl << "MIG: "; first = true;}
        if(i == N3) {cout << endl << "DAV: "; first = true;}
        if (not first) cout << ";";
        first = false;
        cout << alignment[i].name;
        points += alignment[i].points;
        price  += alignment[i].price;
    } cout << endl;
    cout << "Punts: " << points << endl;
    cout << "Preu: "  << price  << endl;
}

void write_alignment(const vector<Player>& alignment) {
    ofstream f("output.txt");
    f << float(clock() - START)/CLOCKS_PER_SEC << endl;
    int points = 0, price = 0;
    int N2 = INPUT.N1+1;
    int N3 = N2+INPUT.N2;
    bool first = true;
    for(int i = 0; i < 11; ++i) {
        if(i == 0) f << "POR: ";
        if(i ==  1) {f << endl << "DEF: "; first = true;}
        if(i == N2) {f << endl << "MIG: "; first = true;}
        if(i == N3) {f << endl << "DAV: "; first = true;}
        if (not first) f << ";";
        first = false;
        f << alignment[i].name;
        points += alignment[i].points;
        price  += alignment[i].price;
    } f << endl;
    f << "Punts: " << points << endl;
    f << "Preu: "  << price  << endl;
    f.close();
}

vector<Player> chosen_to_alignment(const vector<int>& chosen) {
    int n = chosen.size();
    vector<Player> alignment;
    //cerr << "--- fuera for" << endl;
    for(int i = 0; i < n; ++i) {
        //cerr << "--- dentro for" << endl;
        if(chosen[i]) alignment.push_back(candidates[i]);
    }
    //cerr << "-acaba chosen_to_alignment" << endl << endl;
    return alignment;
}
  
/** */
void generate(vector<int>& chosen, int i,
            int cont1, int cont0, int contpor, int contdef, int contmig, int contdav,
            int price, int points, int& max_points, vector<Player>& best_alignment) {

    int n = chosen.size();
    if (cont1 == 11) {
        //cerr << "--hay 11 chosen" << endl;
        //cerr << max_points << " " << points << endl;
        if(points > max_points) {
            //cerr << "en el IF" << endl;
            //cerr << max_points << " " << points << endl;
            max_points = points;
            best_alignment = chosen_to_alignment(chosen);
            //cerr << "--- despues de best_alignment" << endl;
            write_alignment(best_alignment);
            //print_alignment(best_alignment);
        } else {
            points = 0;
            price = 0;
        }
        //cerr << "holaaaa" << endl;
    }
    else if (i < n) {
        chosen[i] = 0;
        if(cont0 < n-11) {
            //cerr << "entra generate cont0"<< endl;
            generate (chosen, i+1, cont1, cont0+1, contpor, contdef, contmig, contdav, price, points, max_points, best_alignment);
        }
        if(cont1 < 11) {
            //cerr << "hola" << endl;
            int price_i = price+candidates[i].price;
            int points_i = points+candidates[i].points;
            if(candidates[i].pos == "por" and contpor < 1 and price_i+(10-cont1)*MIN_PRICE <= INPUT.T and points_i+(10-cont1)*MAX_POINT > max_points){
                //cerr << "----hola por" << endl;
                chosen[i] = 1;
                generate (chosen, i+1, cont1+1, cont0, contpor+1, contdef, contmig, contdav, price_i, points_i, max_points, best_alignment);
            }
            if(candidates[i].pos == "def" and contdef < INPUT.N1 and price_i+(10-cont1)*MIN_PRICE <= INPUT.T and points_i+(10-cont1)*MAX_POINT > max_points){
                //cerr << "----hola def" << endl;
                chosen[i] = 1;
                generate (chosen, i+1, cont1+1, cont0, contpor, contdef+1, contmig, contdav, price_i, points_i, max_points, best_alignment);
            }
            if(candidates[i].pos == "mig" and contmig < INPUT.N2 and price_i+(10-cont1)*MIN_PRICE <= INPUT.T and points_i+(10-cont1)*MAX_POINT > max_points){
                //cerr << "----hola mig" << endl;
                chosen[i] = 1;
                generate (chosen, i+1, cont1+1, cont0, contpor, contdef, contmig+1, contdav, price_i, points_i, max_points, best_alignment);
            }
            if(candidates[i].pos == "dav" and contdav < INPUT.N3 and price_i+(10-cont1)*MIN_PRICE <= INPUT.T and points_i+(10-cont1)*MAX_POINT > max_points){
                //cerr << "----hola dav" << endl;
                chosen[i] = 1;
                generate (chosen, i+1, cont1+1, cont0, contpor, contdef, contmig, contdav+1, price_i, points_i, max_points, best_alignment);
            }
        }
    }
}


/**  */
void generate_alignment() {
    int n = candidates.size();
    vector<int> chosen(n); // From all players, the ones chosen for the alignment
    vector<Player> alignment; // 11 players that does fit the constrictions
    vector<Player> best_alignment; // the best alignment found at the moment
    int max_points = 0;
    generate(chosen, 0, 0, 0, 0, 0, 0, 0, 0, 0, max_points, best_alignment);
}


/** */
void get_alignment(const vector<Player>& players) {
    MAX_POINT = 0;
    MIN_PRICE = numeric_limits<int>::max();
    for(auto p : players) if(p.price <= INPUT.J) {
        candidates.push_back(p);
        if(p.price  < MIN_PRICE) MIN_PRICE = p.price;
        if(p.points > MAX_POINT) MAX_POINT = p.points;
    }
    generate_alignment();
}


/** */
vector<Player> read_data(char* argv) {
    vector<Player> players;
    ifstream in(argv);
    while (not in.eof()) {
        string name, pos, club;
        int points, price;
        getline(in,name,';');    if (name == "") break;
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


/** */
void read_input (char* argv){
    ifstream in(argv);
    int N1, N2, N3, T, J;
    while (not in.eof()){
        in >> N1 >> N2 >> N3 >> T >> J;
        INPUT = {N1, N2, N3, T ,J};
    } in.close();
}

/** Input: ./a.out data_base.txt Projecte/public_benchs/easy-1.txt
    Input (pequeña): ./a.out data.txt easy-0.txt */
int main(int argc, char** argv){
    START = clock();
    vector<Player> players = read_data(argv[1]);
    read_input(argv[2]);
    get_alignment(players);
}
