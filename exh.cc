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
void write_alignment (const vector<Player>& candidates, const vector<int>& choosen, int n){
  for (int i=0; i<n; ++i) if (choosen[i] == 1) cout << candidates[i].nom << ", ";
  cout << endl;
}

void print_alignment (const vector<Player>& alignment, Input input){
  int sum = 0, preu = 0;
  cout << "POR: ";
  for (auto p : alignment) if (p.pos == "por") cout << p.nom << ";", sum += p.punts, preu += p.preu;
  cout << endl << "DEF: ";
  for (auto p : alignment) if (p.pos == "def") cout << p.nom << ";", sum += p.punts, preu += p.preu;
  cout << endl << "MIG: ";
  for (auto p : alignment) if (p.pos == "mig") cout << p.nom << ";", sum += p.punts, preu += p.preu;
  cout << endl << "DAV: ";
  for (auto p : alignment) if (p.pos == "dav") cout << p.nom << ";", sum += p.punts, preu += p.preu;
  cout << endl << "Punts: " << sum << endl;
  cout << "Preu: " << preu << endl;
}

/** */
void generate (const vector<Player>& candidates, vector<int>& choosen, int n, int i, int cont1, int cont0){
  if (i == n){ write_alignment(candidates, choosen, n); cout << "generate" << endl;}
  else {
    if (cont0 < n-11) generate(candidates, choosen, n, i+1, cont1, cont0+1);
    choosen[i] = 1;
    if (cont1 < 11) generate(candidates, choosen, n, i+1, cont1+1, cont0);
  }
}

/** Retorna el vector */
vector<Player> generate_alignment(const vector<Player>& candidates, Input input){
  int n = candidates.size();
  vector<int> choosen (n, 0);
  vector<Player> alignment;
  cout << "entra en generate" << endl;
  generate(candidates, choosen, n, 0, 0, 0);
  cout << "sale de generate" << endl;
  return alignment;
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
/** Input (pequeña): ./a.out data.txt Projecte/public_benchs/easy-1.txt */
int main(int argc, char** argv){
  vector<Player> players = read_data(argv[1]);
  Input input = read_input(argv[2]);
  const time_t starting_time = clock();
  vector<Player> alignment = get_alignment(players, input);
  cout << float(clock() - starting_time)/CLOCKS_PER_SEC << endl;
  print_alignment(alignment, input);
}
