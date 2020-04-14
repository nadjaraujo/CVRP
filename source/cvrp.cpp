#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>
#include <algorithm>
#include<time.h>
#include <stdlib.h>
#include "vector"
#include "cvrpClass.hpp"


#define INFINITO 2147483647; //valor maximo para uma variavel do tipo int

#define DISTRIBUIDOR 0


//heuristica do vizinho mais proximo
void CVRP::HVM()
{ 
  int noAtual = 0;

  // capacidade total do caminhão
  capacidadeAtual = capacidadeTotal;
  int resultadoAtual = 0;
  vector<int> rotaAtual;

  //enquanto houver demanda
  while (1)
  {

    rotaAtual.push_back(noAtual);

    //cout << "Capacidade atual do caminhao: " << capacidadeAtual << endl;
    int proxNo = procurarProximoNoAleatorio(noAtual,5);

    entregaProduto(proxNo);
    //custo total incrementado pela distancia dos dois nos.
    custoTotal += matrizCusto[proxNo][noAtual];

    noAtual = proxNo;
    //se o próximo nó for = 0
    if (proxNo == 0)
    {
      rotaAtual.push_back(proxNo);
      rotas.push_back(rotaAtual); //adiciona a rota feita no vetor de rotas
      rotaAtual.clear();          //limpa rota atual pra começar outra rota
    }

    if (!checarDemanda())
    {
      // voltando manualmente para o distribuidor, trabalho finalizado
      rotaAtual.push_back(proxNo);
      rotaAtual.push_back(DISTRIBUIDOR);
      rotas.push_back(rotaAtual);
      rotaAtual.clear();

      custoTotal += matrizCusto[DISTRIBUIDOR][noAtual];

      break;
    }
  }

  // quando não houver mais demandas.
  // cout << "O trabalho foi realizado! " //prints
  //     << "\n"
  //     << "Distancia total Percorrida de: " << custoTotal << " km" << endl;
  //cout << "Rotas :" << endl;

 vector<vector<int>> bestSolution_VND = VND(rotas);

}

//funçao pra entregar o produto
void CVRP::entregaProduto(int no)
{
  // se nao for um distruibuidor
  if (no != DISTRIBUIDOR)
  {
    capacidadeAtual -= demanda[no];
    demanda[no] = 0;

   // cout << "Cliente " << no << " abastecido" << endl;
  }
  else
  {
    capacidadeAtual = capacidadeTotal;

   // cout << "Caminhou voltou para o distribuidor" << endl;
  }
}

// função que verifica se há demanda
bool CVRP::checarDemanda()
{
  for (int i = 0; i < demanda.size() + 1; i++)
    if (demanda[i] > 0)
      return true;

  return false;
}

// verifica se a demanda do no/vizinho cabe na capacidade do caminhao
bool CVRP::checarCapacidadeComDemanda(int no)
{
  auto v = demanda.at(no);
  return v > 0 && v <= capacidadeAtual;
}

// funcao pra procurar o proximo menor vizinho
/*int CVRP::procurarProximoNo(int noATual)
{
  int prox = DISTRIBUIDOR;
  //variavel que armazena a menor distancia
  int distanciaProx = numeric_limits<int>::max();

  for (int i = 1; i < this->dimensao; i++)
  {
    if (this->matrizCusto[noATual][i] < distanciaProx && checarCapacidadeComDemanda(i)) //checa a distancia do no atual pros vizinhos
    {
      prox = i;
      distanciaProx = this->matrizCusto[noATual][i];
    }
  }

  return prox;
}*/

void CVRP::inserirVizinhoAleatorio(vector<int>& vizinhosAleatorios, int no, int noAtual, int maxAleatorios){

  if(vizinhosAleatorios.size() < maxAleatorios){
    vizinhosAleatorios.push_back(no);
  }
  else{
    int custoMax = 0;
    int iMax = 0;
    // Descobre o no com maior custo
    for(int i = 0; i < vizinhosAleatorios.size();i++){
      if(this->matrizCusto[noAtual][vizinhosAleatorios[i]] > custoMax){
        custoMax = this->matrizCusto[noAtual][vizinhosAleatorios[i]];
        iMax = i;
      }
    }

    if(custoMax > this->matrizCusto[noAtual][no]){
      vizinhosAleatorios[iMax] = no;
    }
  }
}


// funcao pra procurar o proximo menor vizinho
int CVRP::procurarProximoNoAleatorio(int noATual, int maxAleatorios)
{
  vector<int> vizinhosAleatorios;

  for (int i = 1; i < this->dimensao; i++)
  {
    if (checarCapacidadeComDemanda(i)) //checa se o vizinho atual tem demanda que possa ser atendida
    {
      inserirVizinhoAleatorio(vizinhosAleatorios, i, noATual, maxAleatorios);
    }
  }

  if(vizinhosAleatorios.size() > 0){
    int randomIndex = rand() % vizinhosAleatorios.size();
    return vizinhosAleatorios[randomIndex];
  }
  else{
    return DISTRIBUIDOR;
  }

}

//função que calcula a soma das demandas entregues por rota r
int CVRP::somaEntregas(vector<int> r)
{
  int soma = 0;
  for (int i = 0; i < r.size(); i++)
  {
    soma = soma + this->demandaTotal[r[i]];
  }
  return soma;
}

//função que calcula a menor demanda pra cada rota
int CVRP::entregaMin(vector<int> r)
{
  int menor = INFINITO;

  for (int i = 0; i < r.size(); i++)
  {
    if (demandaTotal[r[i]] < menor)
    {

      menor = demandaTotal[r[i]];
    }
  }
  return menor;
}

//função que calcula a maior demanda pra cada rota
int CVRP::entregaMax(vector<int> r)
{
  int maior = 0;

  for (int i = 0; i < r.size(); i++)
  {
    if (demandaTotal[r[i]] > maior)
    {

      maior = demandaTotal[r[i]];
    }
  }
  return maior;
}

//função que calcula o custo pra cada rota
int CVRP::CustoPorRota(vector<int> r)
{

  int soma = 0;

  for (int i = 1; i < r.size(); i++)
  {
    int v1 = r[i - 1];
    int v2 = r[i];

    soma = soma + matrizCusto[v1][v2];
  }

  return soma;
}

//função que calcula o custo da solução total
int CVRP::CustoSolucao(vector<vector<int>> s)
{
  int custo = 0;
  for (int i = 0; i < s.size(); i++)
  {
    custo = custo + CustoPorRota(s[i]);
  }

  return custo;
}

//Movimento de troca de clientes entre a mesma rota
vector<vector<int>> CVRP::swapInterRoute(vector<vector<int>> rota)
{
    vector<int> custoRota;
    vector<int>indicesPrint; // armazena o valor das rotas que foram alteradas
    int custoSwap = 0;
    vector<vector<int>> rotasFinal = rotas;   //decidi deixar um vetor separado pra armazenar as rotas alteradas pra não confundir
    vector<int> rotaAtual;
    vector<int> melhorRota;


    //Armazena os custos das rotas iniciais (rotas sem alterações) num vetor
    int aux = 0;
    for (int j = 0; j < rota.size(); j++){
            aux = CustoPorRota(rota[j]);
            custoRota.push_back(aux);
    }

    vector<int> custoRotaInicial = custoRota; // só armazena os custos iniciais das rotas pra printar depois

    for (int i = 0; i < rota.size(); i++)
    {
        vector<vector<int>>sortRotas;

    //faz parte da função de permutação
    sort(rota[i].begin() + 1, rota[i].end() - 1);

    //Esse next permutation gera todas as combinações possíveis de clientes dentro de cada rota, ou seja, as soluções possíveis
    while (next_permutation(rota[i].begin() + 1, rota[i].end() - 1))
    {

      rotaAtual = rota[i];                    //armazena a rota que está sendo trabalhada atualmente
      custoSwap = CustoPorRota(rota[i]);      // calcula o custo que a rota teria se a troca fosse efetuada

      // O custo que a rota teria caso a troca de clientes fosse efetuada é menor que o custo anterior da rota?
      if (custoSwap <= custoRota[i])
      {
        //se for, o custo passa a ser o menor e sua rota é armazenada
        custoRota[i] = custoSwap;
        melhorRota = rotaAtual;
        sortRotas.push_back(melhorRota);

        //Se não houve mais de 1 movimento que seja de menor custo
        if(sortRotas.size() < 1)
        rotasFinal[i] = melhorRota;

        //Se houve mais de 1 movimento que seja de menor custo, escolhe aleatoriamente algum deles
        if(sortRotas.size() > 1){
        int sizeRotas = sortRotas.size();

        int v1 = rand() % sizeRotas;
        rotasFinal[i] = sortRotas[v1];

        }

         indicesPrint.push_back(i); // armazena o indice da rota que foi alterada pra printar depois
        if(custoRotaInicial[i] == custoRota[i]){ //tira o excesso de indices do vetor
            indicesPrint.pop_back();
        }

       }
      }
    }

    return rotasFinal;
}

// movimento de troca entre um cliente de uma rota para um cliente de outra rota.
vector<vector<int>> CVRP::swap_1_1(vector<vector<int>> s)
{
  vector<vector<int>> rotaInicial = s;

  for (int i = 0; i < s.size(); i++)
  {
    vector<int> r1 = s[i]; //rota r1

    for (int j = i + 1; j < s.size(); j++)
    {
      vector<int> r2 = s[j]; //rota r2
      //otimização!! verifica se é viável fazer o swap
      if (entregaMin(r1) - entregaMax(r2) + somaEntregas(r2) <= capacidadeTotal)
      {
        for (int idxClienteR1 = 0; idxClienteR1 < r1.size(); idxClienteR1++)
        {
          int clienteR1 = r1[idxClienteR1]; //pega o cliente de r1

          if (clienteR1 == 0) // se for 0, é distribuidor (do nothing)
          {
            continue;
          }
          //variavel que vai guardar o melhor swap
          int clienteMelhorSwap = -1;
          int idxClienteMelhorSwap = -1;
          int custoMelhorSwap = INFINITO;

          if (demanda[clienteR1] + somaEntregas(r2) + entregaMax(r2) <= capacidadeTotal)
          {

            for (int idxClienteR2 = 0; idxClienteR2 < r2.size(); idxClienteR2++)
            {

              int clienteR2 = r2[idxClienteR2];
              if (clienteR2 == 0)
              {
                continue;
              }
              //armazena o custo da solução atual pra ser comparada com o custo da solução do swap.
              int custoSolucaoAtual = CustoPorRota(r1) + CustoPorRota(r2);

              vector<int> r1Swapado = r1;
              vector<int> r2Swapado = r2;

              // faz o swap
              r1Swapado[idxClienteR1] = clienteR2;
              r2Swapado[idxClienteR2] = clienteR1;

              //armazena o custo da solução do swap
              int custoSolucaoSwapada = CustoPorRota(r1Swapado) + CustoPorRota(r2Swapado);
              // custo s menor                                  //custo s menor                                           //caber na capacidade p/ r1 e r2;
              if ((custoSolucaoAtual > custoSolucaoSwapada) && (custoSolucaoSwapada < custoMelhorSwap) && (somaEntregas(r1Swapado) <= capacidadeTotal) && (somaEntregas(r2Swapado) <= capacidadeTotal))
              {
                // att solução
                idxClienteMelhorSwap = idxClienteR2;
                clienteMelhorSwap = clienteR2;
                custoMelhorSwap = custoSolucaoSwapada;
              }
            }
            if (clienteMelhorSwap != -1) //se houver
            {

              cout << "swap de " << clienteR1 << " com " << clienteMelhorSwap << endl;
              r2[idxClienteMelhorSwap] = clienteR1;
              r1[idxClienteR1] = clienteMelhorSwap;
              // atualiza rota
              s[i] = r1;
              s[j] = r2;
            }
          }
        }
      }
    }
  }

  vector<vector<int>> rotaFinal = s; // solução atualizada

  return rotaFinal;
}

//remove um cliente de uma rota e insere em outra posição.
vector<vector<int>> CVRP::opt_1(vector<vector<int>> s)
{
  vector<vector<int>> rotaInicial = s;

  for (int i = 0; i < s.size(); i++)
  {
    vector<int> r1 = s[i];

    for (int j = 0; j < r1.size(); j++)
    {
      int vizinhoJ = r1[j];

      if (vizinhoJ == DISTRIBUIDOR)
      {
        continue;
      }

      for (int k = j + 1; k < r1.size(); k++)
      {
        if (r1[k] == DISTRIBUIDOR)
        {
          continue;
        }

        int custoSolucaoAtual = CustoPorRota(r1);                  // salva custo atual
        vector<int> r1_reinserido = r1;                            //copia rota de r1
        r1_reinserido.insert(r1_reinserido.begin() + k, vizinhoJ); // insere vizinho j na posição k
        r1_reinserido.erase(r1_reinserido.begin() + j);            // remove j
        int custoNovaSolucao = CustoPorRota(r1_reinserido);
        if (custoSolucaoAtual > custoNovaSolucao)
        {
          r1 = r1_reinserido;
          s[i] = r1; // att solução
        }
      }
    }
  }
  vector<vector<int>> rotaFinal = s;


  return rotaFinal;
}

//Algortmo variable nighborhood Descent
vector<vector<int>> CVRP::VND(vector<vector<int>> s)
{

vector<vector<int>> rotas = s;

cout << "----------------------------VND-------------------------------" << endl;
  cout << "\n"
       << endl;
  cout << "---------------Rotas Inicias-------------" << endl;
  for (const auto &rot : rotas)
  {
    for (const auto &no : rot) //muitas prints
    {
      cout << no << " ";
    }
    cout << endl;
  }
  int b = CustoSolucao(rotas);
  cout << "Custo Inicial: " << b << " km" << endl;
  cout << "\n"
       << endl;


vector<vector<int>> resultadoAtual;

int custo = CustoSolucao(rotas); 
int custoAtual;
int k = 1;   // tipo de estrutura de vizinhança 

while (k <= 3)
{
  if ( k == 1)
  {
    resultadoAtual = swapInterRoute(rotas);
    custoAtual = CustoSolucao(resultadoAtual); 

  }
  else if ( k == 2)
  {
    resultadoAtual = swap_1_1(rotas);
    custoAtual = CustoSolucao(resultadoAtual); 

  }
  else if ( k == 3)
  {
    resultadoAtual = opt_1(rotas);
    custoAtual = CustoSolucao(resultadoAtual); 
  }

  if (custoAtual < custo)
{

 custo = custoAtual;
 rotas = resultadoAtual;
 k = 1 ; 

}

else 
{
 k = k + 1 ;
}
}

 cout << "\n"
       << endl;
  cout << "---------------Rota Final-------------" << endl;
  for (const auto &rot : resultadoAtual)
  {
    for (const auto &no : rot)
    {
      cout << no << " ";
    }
    cout << endl;
  }
  //prints
  int g = CustoSolucao(resultadoAtual);

  cout << "\n"
       << endl;

  cout << "Custo final apos VND: " << g << " km" << endl;
  cout << "Economia de " << b - g << " km" << endl;

  
return rotas;
}


CVRP::CVRP(const std::string arquivo)
{
  ifstream input(arquivo.c_str());
  int countLine = 1; //contador de linhas da string
  for (string line; getline(input, line);)
  {

    //função para dividir a string
    istringstream iss(line);
    vector<string>
        results(istream_iterator<string>{iss},
                istream_iterator<string>());

    if (countLine <= 3)
    {
      // tratar name, dimension,capacity

      if (countLine == 1) // tratamento do nome
      {
        this->nome = results[1];
      }
      else if (countLine == 2) // tratamento da dimensao
      {
        this->dimensao = std::stoi(results[1]);
        this->matrizCusto = new int *[this->dimensao];
        for (int i = 0; i < this->dimensao; ++i)
          this->matrizCusto[i] = new int[this->dimensao]; //a matriz custo recebe o tamanho da dimensao
      }
      else if (countLine == 3) // tratamento da capacidade
      {
        this->capacidadeTotal = std::stoi(results[1]);
      }
    }
    else
    {

      if (countLine > 4 && countLine < (5 + this->dimensao))
      {
        this->demandaTotal.push_back(atoi(results[1].c_str()));
        // demanda
      }
      else if (countLine > (6 + this->dimensao))
      {
        int offset = 7 + this->dimensao; // fixa a primeira linha da matrix

        for (int i = 0; i < results.size(); i++)
        {
          this->matrizCusto[countLine - offset][i] = std::stoi(results[i]);
        }
        // matriz custo
      }
    }
    countLine++;
  }

  demanda = this->demandaTotal;
}

