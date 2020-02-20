#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>

using namespace std;

#define DISTRIBUIDOR 0

class CVRP
{
public:
    string nome;
    int dimensao;
    int capacidadeTotal;
    int capacidadeAtual;
    vector<int> demanda;
    int **matrizCusto; // matriz de custos

    CVRP() {}
    CVRP(const string);
    bool checarDemanda();
    bool checarCapacidadeComDemanda(int);
    int procurarProximoNo(int);
    void HVM();
};

void CVRP::HVM()
{
    int noAtual = 0;
    int custoTotal = 0;
    capacidadeAtual = capacidadeTotal;

    while (checarDemanda())
    {
        int proxNo = procurarProximoNo(noAtual);

        if (proxNo == DISTRIBUIDOR)
        {
            capacidadeAtual = capacidadeTotal;
        }

        custoTotal += matrizCusto[proxNo][noAtual];
    }
}

bool CVRP::checarDemanda()
{
    for (int i = 0; i < this->demanda.size(); i++)
    {
        if (this->demanda[i] > 0)
        {
            return true;
        }
    }
    return false;
}

bool CVRP::checarCapacidadeComDemanda(int no)
{
    if (this->demanda[no] > 0 && this->demanda[no] <= this->capacidadeAtual)
    {
        return true;
    }
    else
    {

        return false;
    }
}

int CVRP::procurarProximoNo(int noATual)
{
    int prox = DISTRIBUIDOR;
    int distanciaProx = numeric_limits<int>::max();

    for (int i = 1; i < this->dimensao; i++)
    {
        if (this->matrizCusto[noATual][i] < distanciaProx && checarCapacidadeComDemanda(i))
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
                this->dimensao = atoi(results[1].c_str());
                this->matrizCusto = new int *[this->dimensao];
                for (int i = 0; i < this->dimensao; ++i)
                    this->matrizCusto[i] = new int[this->dimensao]; //a matriz custo recebe o tamanho da dimensao
            }
            else if (countLine == 3) // tratamento da capacidade
            {
                this->capacidadeTotal = atoi(results[1].c_str());
            }
        }
        else
        {
            if (countLine > 4 && countLine < (5 + this->dimensao))
            {
                this->demanda.push_back(atoi(results[1].c_str()));
                // demanda
            }
            else if (countLine > (6 + this->dimensao))
            {
                int offset = 7 + this->dimensao; // fixa a primeira linha da matrix

                for (int i = 0; i < results.size(); i++)
                {
                    cout << atoi(results[i].c_str()) << endl;
                    this->matrizCusto[countLine - offset][i] = atoi(results[i].c_str());
                }
                // matriz custo
            }
            countLine++;
            // cout << line << endl;
        }
    }
}