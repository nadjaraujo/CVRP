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
        istringstream iss(line);
        vector<string>
            results(istream_iterator<string>{iss},
                    istream_iterator<string>());

        if (countLine <= 3)
        {

            // tratar name, dimension,capacity

            if (countLine == 1)
            {
                roteamento.nome = results[1];
            }
            else if (countLine == 2)
            {
                roteamento.dimensao = atoi(results[1].c_str());
                roteamento.matrizCusto = new int *[roteamento.dimensao];
                for (int i = 0; i < roteamento.dimensao; ++i)
                    roteamento.matrizCusto[i] = new int[roteamento.dimensao];
            }
            else if (countLine == 3)
            {
                roteamento.capacidade = atoi(results[1].c_str());
            }
        }

        else
        {
            if (countLine == 4 || countLine == (6 + roteamento.dimensao) || countLine == (5 + roteamento.dimensao))
            {
            }
            else if (countLine > 4 && countLine < (5 + roteamento.dimensao))
            {
                roteamento.demanda.push_back(atoi(results[1].c_str()));
                // demanda
            }
            else
            {
                int offset = 7 + roteamento.dimensao;

                for (int i = 0; i < results.size(); i++)
                {
                    cout << atoi(results[i].c_str()) << endl;
                    roteamento.matrizCusto[countLine - offset][i] = atoi(results[i].c_str());
                }
                // matriz peso
            }

        }
        countLine++;
        // cout << line << endl;
    }
}
