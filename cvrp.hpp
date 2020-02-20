#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

using namespace std;

class CVRP
{
public:
    string nome;
    int dimensao;
    int capacidade;
    vector<int> demanda;
    int **matrizCusto; // matriz de custos
    CVRP() {}
};

bool lerArquivo(const std::string arquivo)
{
    CVRP roteamento;
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
                roteamento.nome = results[1];
            }
            else if (countLine == 2) // tratamento da dimensao
            {
                roteamento.dimensao = atoi(results[1].c_str());
                roteamento.matrizCusto = new int *[roteamento.dimensao];
                for (int i = 0; i < roteamento.dimensao; ++i)
                    roteamento.matrizCusto[i] = new int[roteamento.dimensao]; //a matriz custo recebe o tamanho da dimensao
            }
            else if (countLine == 3) // tratamento da capacidade
            {
                roteamento.capacidade = atoi(results[1].c_str());
            }
        }
        else
        {
            if (countLine > 4 && countLine < (5 + roteamento.dimensao))
            {
                roteamento.demanda.push_back(atoi(results[1].c_str()));
                // demanda
            }
            else if (countLine > (6 + roteamento.dimensao))
            {
                int offset = 7 + roteamento.dimensao; // fixa a primeira linha da matrix

                for (int i = 0; i < results.size(); i++)
                {
                    cout << atoi(results[i].c_str()) << endl;
                    roteamento.matrizCusto[countLine - offset][i] = atoi(results[i].c_str());
                }
                // matriz custo
            }
            countLine++;
            // cout << line << endl;
        }
    }
