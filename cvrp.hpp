#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class CVRP {
    public:
    string nome;
    int dimensao;
    int capacidade;
    vector<int> demanda;

}

bool lerArquivo(const std:: string arquivo)
{ 
ifstream input( arquivo.c_str() );

for( string line; getline( input, line ); )
{
    cout<< line << endl;
}
}

