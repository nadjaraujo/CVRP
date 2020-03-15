#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>
#include <algorithm>
#include <bits/stdc++.h>

#define INFINITO 2147483647; //valor maximo para uma variavel do tipo int

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
    vector<int> demandaTotal;
    int **matrizCusto;
    bool checarDemanda();
    bool checarCapacidadeComDemanda(int);
    void entregaProduto(int);
    int procurarProximoNo(int);
    int CalcularCustoSwap(vector<vector<int>>, int);
    int CalcularCustoPrint(vector<int>);
    void CalcularCustoDasRotas();
    int somaEntregas(vector<int>);
    int entregaMin(vector<int>);
    int entregaMax(vector<int>);
    int CustoPorRota(vector<int>);
    int CustoSolucao(vector<vector<int>>);
    vector<int> custoRota;
    vector<vector<int>> swap_1_1(vector<vector<int>>);
    vector<vector<int>> rotas; //vertor que vai armazenar as rotas
    int custoTotal = 0;

public:
    CVRP() {}
    CVRP(const string);

    void HVM();
    void swapInterRoute();
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
    while (1)
    {

        rotaAtual.push_back(noAtual);

        cout << "Capacidade atual do caminhao: " << capacidadeAtual << endl;
        int proxNo = procurarProximoNo(noAtual);

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
            rotas.push_back(rotaAtual); //adiciona a rota feita no vetor de rotas
            rotaAtual.clear();          //limpa rota atual pra começar outra rota

            custoTotal += matrizCusto[DISTRIBUIDOR][noAtual];

            break;
        }
    }

    // quando não houver mais demandas.
    cout << "O trabalho foi realizado! "
         << "\n"
         << "Distancia total Percorrida de: " << custoTotal << " km" << endl;
    cout << "Rotas :" << endl;

    CalcularCustoDasRotas();

    for (const auto &rota : rotas)
    {
        for (const auto &no : rota)
        {
            cout << no << " ";
        }
        cout << endl;
    }

    swapInterRoute();

    swap_1_1(rotas);
}

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

//
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

int CVRP::CustoSolucao(vector<vector<int>> s)
{
    int custo = 0;
    for (int i = 0; i < s.size(); i++)
    {
        custo = custo + CustoPorRota(s[i]);
    }

    return custo;
}

// movimento de troca entre duas rotas (swap 1 por 1)

vector<vector<int>> CVRP::swap_1_1(vector<vector<int>> s)
{
    vector<vector<int>> rotaInicial = s;
    cout << "----------------------------SWAP_1_1-------------------------------" << endl;
    cout << "\n"
         << endl;
    cout << "---------------Rotas Inicias-------------" << endl;
    for (const auto &rot : rotaInicial)
    {
        for (const auto &no : rot)
        {
            cout << no << " ";
        }
        cout << endl;
    }

    cout << "\n"
         << endl;

    for (int i = 0; i < s.size(); i++)
    {
        vector<int> r1 = s[i];

        for (int j = i + 1; j < s.size(); j++)
        {
            vector<int> r2 = s[j];

            if (entregaMin(r1) - entregaMax(r2) + somaEntregas(r2) <= capacidadeTotal)
            {
                for (int idxClienteR1 = 0; idxClienteR1 < r1.size(); idxClienteR1++)
                {
                    int clienteR1 = r1[idxClienteR1];

                    if (clienteR1 == 0)
                    {
                        continue;
                    }

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

                            int custoSolucaoAtual = CustoPorRota(r1) + CustoPorRota(r2);

                            vector<int> r1Swapado = r1;
                            vector<int> r2Swapado = r2;

                            // swap
                            r1Swapado[idxClienteR1] = clienteR2;
                            r2Swapado[idxClienteR2] = clienteR1;

                            int custoSolucaoSwapada = CustoPorRota(r1Swapado) + CustoPorRota(r2Swapado);

                            if ((custoSolucaoAtual > custoSolucaoSwapada) && (custoSolucaoSwapada < custoMelhorSwap) && (somaEntregas(r1Swapado) <= capacidadeTotal) && (somaEntregas(r2Swapado) <= capacidadeTotal))
                            {
                                cout << somaEntregas(r1Swapado) << endl;
                                cout << somaEntregas(r2Swapado) << endl;
                                idxClienteMelhorSwap = idxClienteR2;
                                clienteMelhorSwap = clienteR2;
                                custoMelhorSwap = custoSolucaoSwapada;
                            }
                        }
                        if (clienteMelhorSwap != -1)
                        {

                            cout << "swap de " << clienteR1 << " com " << clienteMelhorSwap << endl;
                            r2[idxClienteMelhorSwap] = clienteR1;
                            r1[idxClienteR1] = clienteMelhorSwap;

                            s[i] = r1;
                            s[j] = r2;
                        }
                    }
                }
            }
        }
    }

    vector<vector<int>> rotaFinal = s;

    cout << "\n"
         << endl;
    cout << "---------------Rota Final-------------" << endl;
    for (const auto &rot : rotaFinal)
    {
        for (const auto &no : rot)
        {
            cout << no << " ";
        }
        cout << endl;
    }

    int b = CustoSolucao(rotaInicial);
    int g = CustoSolucao(rotaFinal);

    cout << "\n"
         << endl;
    cout << "Distancia Percorrida inicialmente: " << b << " km" << endl;
    cout << "Distancia Percorrida apos o swap: " << g << " km" << endl;
    cout << "Economia de " << b - g << " km" << endl;

    return rotaFinal;
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

//funçao que calcula os custos finais pra printar
int CVRP::CalcularCustoPrint(vector<int> r)
{
    int aux = 0;
    for (int i = 0; i < r.size(); i++)
    {

        aux += r[i];
    }

    return aux;
}

//Função que calcula os custos das rotas iniciais e coloca num array de custos
void CVRP::CalcularCustoDasRotas()
{

    vector<vector<int>> r = rotas;

    int c = 0;

    for (int i = 0; i < r.size(); i++)
    {
        c = 0;
        for (int j = 0; j < r[i].size(); j++)
        {

            int clienteInicial = r[i][j];
            int clienteTestado = r[i][j + 1];

            if (clienteTestado == 0)
            {

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
int CVRP::CalcularCustoSwap(vector<vector<int>> r, int indiceRota)
{

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
    for (int j = 0; j < r[indiceRota].size(); j++)
    {

        int clienteInicial = r[indiceRota][j];
        int clienteTestado = r[indiceRota][j + 1];

        if (clienteTestado == 0)
        {
            custo += matrizCusto[clienteTestado][clienteInicial];

            break;
        }
        else
            custo += matrizCusto[clienteTestado][clienteInicial];
    }

    return custo;
}
void CVRP::swapInterRoute()
{

    vector<int> indicesPrint;
    int custoSwap = 0;
    vector<vector<int>> ro = rotas;
    vector<int> rotaAtual;
    vector<int> melhorRota;
    vector<vector<int>> rotasFinal = rotas;   //decidi deixar um vetor separado pra armazenar as rotas alteradas pra não confundir
    vector<int> custoRotaInicial = custoRota; // só armazena os custos iniciais das rotas pra printar depois
    //for(int i = 0; i < custoRotaInicial.size(); i++){cout<< "Custo rota indice"<< i << " "<<custoRotaInicial[i]<< "\n";}

    //

    for (int i = 0; i < ro.size(); i++)
    {

        //faz parte da função de permutação
        sort(ro[i].begin() + 1, ro[i].end() - 1);

        //Esse next permutation gera todas as combinações possíveis de clientes dentro de cada rota, ou seja, as soluções possíveis
        while (next_permutation(ro[i].begin() + 1, ro[i].end() - 1))
        {

            rotaAtual = ro[i];                    //armazena a rota que está sendo trabalhada atualmente
            custoSwap = CalcularCustoSwap(ro, i); // calcula o custo que a rota teria se a troca fosse efetuada

            //cout<<"CustoSwap"<<custoSwap<<endl;
            //cout<<"CustoRota"<<custoRota[i]<<endl;

            if (custoSwap < custoRota[i])
            { // Se o custo que a rota teria caso a troca de clientes fosse efetuada é menor que o custo anterior da rota?

                indicesPrint.push_back(i);

                //se for, o custo passa a ser o menor e sua rota é armazenada
                custoRota[i] = custoSwap;
                melhorRota = rotaAtual;
                rotasFinal[i] = melhorRota;
            }
        }
    }

    cout << "----------------------------SWAP INTER ROUTE-------------------------------" << endl;
    cout << "\n"
         << endl;
    cout << "---------------Rotas Inicias-------------" << endl;
    for (const auto &rot : rotas)
    {
        for (const auto &no : rot)
        {
            cout << no << " ";
        }
        cout << endl;
    }

    cout << "\n"
         << "Alteracoes foram feitas nas rotas: " << endl;
    for (int kaka = 0; kaka < indicesPrint.size(); kaka++)
        cout << indicesPrint[kaka] << endl;

    cout << "\n"
         << endl;

    for (int kaka = 0; kaka < indicesPrint.size(); kaka++)
    {
        cout << "***** Rota " << indicesPrint[kaka] << ": *******" << endl;
        cout << "Custo anterior da rota: " << custoRotaInicial[indicesPrint[kaka]] << endl;
        cout << "Custo da rota apos a alteracao: " << custoRota[indicesPrint[kaka]] << endl;
    }

    cout << "\n"
         << endl;
    cout << "---------------Rota Final-------------" << endl;
    for (const auto &rot : rotasFinal)
    {
        for (const auto &no : rot)
        {
            cout << no << " ";
        }
        cout << endl;
    }

    int b = CalcularCustoPrint(custoRotaInicial);
    int g = CalcularCustoPrint(custoRota);

    cout << "\n"
         << endl;
    cout << "Distancia Percorrida inicialmente: " << b << " km" << endl;
    cout << "Distancia Percorrida apos o swap: " << g << " km" << endl;
    cout << "Economia de " << b - g << " km" << endl;
}
