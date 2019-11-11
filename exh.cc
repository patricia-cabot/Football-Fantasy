#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;


/** */
struct Input {
    int n1, n2, n3, t, j;
};


/** */
struct Player {
    string name;
    string pos;
    int price;
    string club;
    int points;
};


/** */
void all_alignments(const vector<Player>& candidates, const vector<int>& chosen, vector<vector<Player>>& all) {
    int n = chosen.size();
    vector<Player> alignment;
    for (int i = 0; i < n; ++i){
        if (chosen[i]) alignment.push_back(candidates[i]);
    }
    all.push_back(alignment);
}


/** */
void print_alignment(const vector<Player>& alignment, Input input) {
    int points = 0, price = 0;
    int n2 = input.n1+1;
    int n3 = n2+input.n2;
    bool first = true;
    for(int i = 0; i < 11; ++i) {
        if(i == 0) cout << "POR: ";
        if(i ==  1) {cout << endl << "DEF: "; first = true;}
        if(i == n2) {cout << endl << "MIG: "; first = true;}
        if(i == n3) {cout << endl << "DAV: "; first = true;}
        if (not first) cout << ";";
        first = false;
        cout << alignment[i].name;
        points += alignment[i].points;
        price  += alignment[i].price;
    } cout << endl;
    cout << "Punts: " << points << endl;
    cout << "Preu: "  << price  << endl;
}


/** */
void generate(const vector<Player>& candidates, vector<int>& chosen, vector<vector<Player>>& all, Input input,
            int i, int cont1, int cont0, int contpor, int contdef, int contmig, int contdav, int price) {
    int n = chosen.size();
    if (i == n) all_alignments(candidates, chosen, all);
    else {
        chosen[i] = 0;
        if(cont0 < n-11) generate (candidates, chosen, all, input, i+1, cont1, cont0+1, contpor, contdef, contmig, contdav, price);
        if(cont1 < 11) {
            if(candidates[i].pos == "por" and contpor < 1 and candidates[i].price+price <= input.t){
                chosen[i] = 1;
                generate (candidates, chosen, all, input, i+1, cont1+1, cont0, contpor+1, contdef, contmig, contdav, candidates[i].price+price);
            }
            if(candidates[i].pos == "def" and contdef < input.n1 and candidates[i].price+price <= input.t){
                chosen[i] = 1;
                generate (candidates, chosen, all, input, i+1, cont1+1, cont0, contpor, contdef+1, contmig, contdav, candidates[i].price+price);
            }
            if(candidates[i].pos == "mig" and contmig < input.n2 and candidates[i].price+price <= input.t){
                chosen[i] = 1;
                generate (candidates, chosen, all, input, i+1, cont1+1, cont0, contpor, contdef, contmig+1, contdav, candidates[i].price+price);
            }
            if(candidates[i].pos == "dav" and contdav < input.n3 and candidates[i].price+price <= input.t){
                chosen[i] = 1;
                generate (candidates, chosen, all, input, i+1, cont1+1, cont0, contpor, contdef, contmig, contdav+1, candidates[i].price+price);
            }
        }
    }
}


/** From all possible alignments get the best one. */
vector<Player> get_best_alignment(const vector<vector<Player>>& all) {
    int n = all.size();
    int max_points = 0, max_i = 0;
    for(int i = 0; i < n; ++i ) {
        int points = 0;
        for(auto p : all[i]) points += p.points;
        if (max_points < points) max_points = points, max_i = i;
    }
    return all[max_i];
}


/**  */
vector<Player> generate_alignment(const vector<Player>& candidates, Input input) {
    int n = candidates.size();
    vector<int> chosen(n); // From all players, the ones chosen for the alignment
    vector<Player> alignment; // 11 players that does fit the constrictions
    vector<vector<Player>> all; // All possible alignments of 11 players
    generate(candidates, chosen, all, input, 0, 0, 0, 0, 0, 0, 0, 0);

    cout << "---------------" << endl;
    for(auto& a : all) print_alignment(a, input);
    cout << "---------------FIN" << endl;

    return get_best_alignment(all);
}


/** */
vector<Player> get_alignment(const vector<Player>& players, Input input) {
    vector<Player> candidates; // Players that don't exceed player's price
    for(auto p : players) if(p.price <= input.j) candidates.push_back(p);
    return generate_alignment(candidates, input);
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
Input read_input (char* argv){
    Input input;
    ifstream in(argv);
    int n1, n2, n3, t, j;
    while (not in.eof()){
        in >> n1 >> n2 >> n3 >> t >> j;
        input = {n1, n2, n3, t ,j};
    } in.close();
    return input;
}


/** Input: ./a.out data_base.txt Projecte/public_benchs/easy-1.txt
    Input (pequeña): ./a.out data.txt easy-0.txt */
int main(int argc, char** argv){
    vector<Player> players = read_data(argv[1]);
    Input input = read_input(argv[2]);
    const time_t starting_time = clock();
    vector<Player> alignment = get_alignment(players, input);
    cout << float(clock() - starting_time)/CLOCKS_PER_SEC << endl;
    print_alignment(alignment, input);
}
