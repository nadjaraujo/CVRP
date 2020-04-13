#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>
#include <algorithm>
#include<time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class CVRP
{
private:
  string nome;
  int dimensao;
  int capacidadeTotal;
  int capacidadeAtual;
  vector<int> demanda;
  vector<int> demandaTotal;
  int **matrizCusto;
  bool checarDemanda();
  bool checarCapacidadeComDemanda(int);
  void entregaProduto(int);
  int procurarProximoNo(int);
  int somaEntregas(vector<int>);
  int entregaMin(vector<int>);
  int entregaMax(vector<int>);
  int CustoPorRota(vector<int>);
  int CustoSolucao(vector<vector<int>>);
  vector<vector<int>> rotas; //vertor que vai armazenar as rotas
  int custoTotal = 0;
  vector<vector<int>> swapInterRoute(vector<vector<int>>);
  vector<vector<int>> swap_1_1(vector<vector<int>>);
  vector<vector<int>> opt_1(vector<vector<int>>);
  vector<vector<int>> VND(vector<vector<int>>);

public:
  CVRP() {}
  CVRP(const string);
  void HVM();
};