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
  string nom;
  string pos;
  int preu;
  string equip;
  int punts;
};

/** */
void write_alignment (const vector<Player>& candidates, const vector<int>& choosen, vector<vector<Player>>& all){
  int n = choosen.size();
  vector<Player> alignment;
  for (int i=0; i<n; ++i){
    if (choosen[i] == 1) alignment.push_back(candidates[i]);
  } all.push_back(alignment);
}

/** */
void print_alignment (const vector<Player>& alignment, Input input){
  int sum = 0, preu = 0;
  cout << "POR: ";
  cout << alignment[0].nom << endl;
  sum += alignment[0].punts, preu += alignment[0].preu;
  cout << "DEF: ";
  bool primer = true;
  for (int i=1; i<=input.n1; ++i){
    if (primer) primer = false, cout << alignment[i].nom;
    cout << ";" << alignment[i].nom;
    sum += alignment[i].punts;
    preu += alignment[i].preu;
  } primer = true;
  cout << endl << "MIG: ";
  for (int i=input.n1+1; i<=input.n1+input.n2; ++i){
    if (primer) primer = false, cout << alignment[i].nom;
    cout << ";" << alignment[i].nom;
    sum += alignment[i].punts;
    preu += alignment[i].preu;
  } primer = true;
  cout << endl << "DAV: ";
  for (int i=input.n2+input.n1+1; i<11; ++i){
    if (primer) primer = false, cout << alignment[i].nom;
    cout << ";" << alignment[i].nom;
    sum += alignment[i].punts;
    preu += alignment[i].preu;
  }
  cout << endl << "Punts: " << sum << endl;
  cout << "Preu: " << preu << endl;
}

/** */
void generate (const vector<Player>& candidates, vector<int>& choosen, vector<vector<Player>>& all, Input input, int i, int cont1,
              int cont0, int contpor, int contdef, int contmig, int contdav, int preu){
  int n = choosen.size();
  if (i == n) write_alignment(candidates, choosen, all);
  else {
    choosen[i] = 0;
    if (cont0 < n-11) generate (candidates, choosen, all, input, i+1, cont1, cont0+1, contpor, contdef, contmig, contdav, preu);
    if (cont1 < 11){
      if (candidates[i].pos == "por" and contpor == 0 and candidates[i].preu+preu <= input.t){
        choosen[i] = 1;
        generate (candidates, choosen, all, input, i+1, cont1+1, cont0, contpor+1, contdef, contmig, contdav, candidates[i].preu+preu);
      }
      if (candidates[i].pos == "def" and contdef < input.n1 and candidates[i].preu+preu <= input.t){
        choosen[i] = 1;
        generate (candidates, choosen, all, input, i+1, cont1+1, cont0, contpor, contdef+1, contmig, contdav, candidates[i].preu+preu);
      }
      if (candidates[i].pos == "mig" and contmig < input.n2 and candidates[i].preu+preu <= input.t){
        choosen[i] = 1;
        generate (candidates, choosen, all, input, i+1, cont1+1, cont0, contpor, contdef, contmig+1, contdav, candidates[i].preu+preu);
      }
      if (candidates[i].pos == "dav" and contdav < input.n3 and candidates[i].preu+preu <= input.t){
        choosen[i] = 1;
        generate (candidates, choosen, all, input, i+1, cont1+1, cont0, contpor, contdef, contmig, contdav+1, candidates[i].preu+preu);
      }
    }
  }
}

vector<Player> get_best_alignment (const vector<vector<Player>>& all){
  int max = 0;
  int punts = 0;
  int n = all.size();
  int posicio = 0;
  for (int i=0; i<n; ++i ){
    for(auto p : all[i]) punts += p.punts;
    if (max < punts) max = punts, posicio = i;
  } return all[posicio];
}

/** Retorna el vector */
vector<Player> generate_alignment(const vector<Player>& candidates, Input input){
  int n = candidates.size();
  vector<int> choosen (n, 0);
  vector<Player> alignment;
  vector<vector<Player>> all;
  generate(candidates, choosen, all, input, 0, 0, 0, 0, 0, 0, 0, 0);
  return get_best_alignment(all);
}

/** */
vector<Player> get_alignment(const vector<Player>& players, Input input){
  vector<Player> candidates;
  for (auto p : players) if (p.preu <= input.j) candidates.push_back(p);
  return generate_alignment(candidates, input);
}

/** */
vector<Player> read_data(char* argv){
  vector<Player> players;
  ifstream in(argv);
  while (not in.eof()) {
    string nom, posicio, club;
    int punts, preu;
    getline(in,nom,';');    if (nom == "") break;
    getline(in,posicio,';');
    in >> preu;
    char aux; in >> aux;
    getline(in,club,';');
    in >> punts;
    string aux2;
    getline(in,aux2);
    players.push_back({nom, posicio, preu, club, punts});
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

/** Input: ./a.out data_base.txt Projecte/public_benchs/easy-1.txt */
/** Input (pequeña): ./a.out data.txt easy-0.txt */
int main(int argc, char** argv){
  vector<Player> players = read_data(argv[1]);
  Input input = read_input(argv[2]);
  const time_t starting_time = clock();
  vector<Player> alignment = get_alignment(players, input);
  cout << float(clock() - starting_time)/CLOCKS_PER_SEC << endl;
  print_alignment(alignment, input);
}
