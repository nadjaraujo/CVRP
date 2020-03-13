#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>
#include <algorithm>
#include <bits/stdc++.h>
//#include "movimentos.hpp"

using namespace std;

#define DISTRIBUIDOR 0

class CVRP
{
private:
    string nome;
    int dimensao;
    int capacidadeTotal;
    int capacidadeAtual;
    vector<int> demanda;
    int **matrizCusto;
    bool checarDemanda();
    bool checarCapacidadeComDemanda(int);
    void entregaProduto(int);
    int procurarProximoNo(int);
    int CalcularCustoSwap(vector<vector <int>> , int);
    void CalcularCustoDasRotas();
    int CalcularCusto(vector<vector<int>>);
    vector<vector <int>> rotas;  //vertor que vai armazenar as rotas
    vector <int> custoRota;
    int custoTotal = 0;


public:

    void swapOnebyOne();


    CVRP() {}
    CVRP(const string);

    void HVM();

};

//heuristica do vizinho mais proximo
void CVRP::HVM()
{
    int noAtual = 0;

    // capacidade total do caminhão
    capacidadeAtual = capacidadeTotal;
    int custoAtual = 0;
    //vetor que vai armazenar as rotas atuais
    vector<int> rotaAtual;
    //enquanto houver demanda
    while (checarDemanda())
    {

        rotaAtual.push_back(noAtual);


        cout << "Capacidade atual do caminhao: " << capacidadeAtual << endl;
        int proxNo = procurarProximoNo(noAtual);

        entregaProduto(proxNo);

        //custo total incrementado pela distancia dos dois nos.

        custoTotal += matrizCusto[proxNo][noAtual];





        noAtual = proxNo;
        //se o próximo nó for = 0


        if(proxNo == 0){


            rotaAtual.push_back(proxNo);
            rotas.push_back(rotaAtual);    //adiciona a rota feita no vetor de rotas


            rotaAtual.clear();        //limpa rota atual pra começar outra rota

        }

    }
    // quando não houver mais demandas.
    cout << "O trabalho foi realizado! "
         << "\n"
         << "Distancia total Percorrida de: " << custoTotal << " km" << endl;


        CalcularCustoDasRotas();

        swapOnebyOne();

}




// int CVRP::swapOnebyOne(vector<vector<int>> r){
//     // int i = r[1][1];
//     // for(i = 1; i < r.size(); i++ )
// }


void CVRP::entregaProduto(int no)
{
    // se nao for um distruibuidor
    if (no != DISTRIBUIDOR)
    {
        capacidadeAtual -= demanda[no];
        demanda[no] = 0;

        cout << "Cliente " << no << " abastecido" << endl;
    }
    else
    {
        capacidadeAtual = capacidadeTotal;

        cout << "Caminhou voltou para o distribuidor" << endl;
    }
}
// verifica se há demanda
bool CVRP::checarDemanda()
{
    for (int i = 0; i < this->demanda.size(); i++)
        if (this->demanda[i] > 0)
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
int CVRP::procurarProximoNo(int noATual)
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
                this->demanda.push_back(std::stoi(results[1]));
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
}



//Função que calcula os custos das rotas iniciais e coloca num array de custos
void CVRP:: CalcularCustoDasRotas()
{

vector<vector <int>> r = rotas;

   int c = 0;

     for(int i = 0; i < r.size(); i++){
        c = 0;
           for(int j = 0; j < r[i].size(); j++){

            int clienteInicial = r[i][j];
            int clienteTestado = r[i][j+1];

            if(clienteTestado == 0){

                c += matrizCusto[clienteTestado][clienteInicial];

                break;
            }
            else
            c += matrizCusto[clienteTestado][clienteInicial];

           }
            custoRota.push_back(c);

           }

}

//Função que calcula o custo que a rota passaria a ter caso troque seus clientes de ordem
int CVRP:: CalcularCustoSwap(vector<vector <int>> r , int indiceRota){

int custo = 0;
/*
 cout <<"rotaTeste:"<< endl;
        for (const auto& rota : r){
            for(const auto& no :rota){
                cout<<no<< " ";
            }
            cout << endl;
    }
*/
       for(int j = 0; j < r[indiceRota].size(); j++){

        int clienteInicial = r[indiceRota][j];
        int clienteTestado = r[indiceRota][j+1];

        if(clienteTestado == 0){
            custo += matrizCusto[clienteTestado][clienteInicial];

            break;
        }
        else
        custo += matrizCusto[clienteTestado][clienteInicial];

       }

    return custo;

}

void CVRP::swapOnebyOne() {

    vector <int> indicesPrint;
    int custoSwap = 0;
    vector<vector<int>> ro = rotas;
    vector <int> rotaAtual;
    vector <int> melhorRota;
    vector<vector<int>> rotasFinal = rotas;  //decidi deixar um vetor separado pra armazenar as rotas alteradas pra não confundir
    vector <int> custoRotaInicial = custoRota; // só armazena os custos iniciais das rotas pra printar depois
    vector <int> somarCustos;

    for(int i = 0; i < custoRotaInicial.size(); i++)
        cout<< "Custo rota indice"<< i << " "<<custoRotaInicial[i]<< "\n";

    for(int i = 0; i < ro.size(); i++){

       //faz parte da função de permutação
       sort(ro[i].begin()+1, ro[i].end()-1);

        //Esse next permutation gera todas as combinações possíveis de clientes dentro de cada rota, ou seja, as soluções possíveis
        while (next_permutation(ro[i].begin()+1, ro[i].end()-1)) {

            rotaAtual = ro[i]; //armazena a rota que está sendo trabalhada atualmente
            custoSwap = CalcularCustoSwap(ro, i); // calcula o custo que a rota teria se a troca fosse efetuada

         //cout<<"CustoSwap"<<custoSwap<<endl;
         //cout<<"CustoRota"<<custoRota[i]<<endl;

            if (custoSwap < custoRota[i]){ // Se o custo que a rota teria caso a troca de clientes fosse efetuada é menor que o custo anterior da rota?
                indicesPrint.push_back(i);


                //se for, o custo passa a ser o menor e sua rota é armazenada
                custoRota[i] = custoSwap;
                melhorRota = rotaAtual;
                rotasFinal[i] = melhorRota;


            }

        }

    }


    cout<< "----------------------------SWAP INTER ROUTE-------------------------------"<<endl;
    cout<< "\n"<<endl;
     cout <<"---------------Rotas Inicias-------------"<< endl;
            for (const auto& rot : rotas){
                for(const auto& no :rot){
                    cout<<no<< " " ;
                }
                cout << endl;
            }


    cout<< "\n"<< "Alteracoes foram feitas nas rotas: "<<endl;
    for(int kaka = 0; kaka < indicesPrint.size(); kaka++)
        cout<< indicesPrint[kaka]<<endl;

    cout<< "\n"<<endl;

    for(int kaka = 0; kaka < indicesPrint.size(); kaka++){
    cout << "***** Rota " << indicesPrint[kaka] << ": *******"<< endl;
    cout<< "Custo anterior da rota: " << custoRotaInicial[indicesPrint[kaka]] << endl;
    cout<< "Custo da rota apos a alteracao: " <<  custoRota [indicesPrint[kaka]]<<endl;
    }

    cout<< "\n"<<endl;
    cout <<"---------------Rota Final-------------"<< endl;
            for (const auto& rot : rotasFinal){
                for(const auto& no :rot){
                    cout<<no<< " ";
                }
                cout << endl;

            }
}
