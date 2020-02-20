#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool lerArquivo(const std:: string arquivo)
{
   
ifstream input( arquivo.c_str() );

for( string line; getline( input, line ); )
{
    cout<< line << endl;
}
 /*   std::string n;
    ifstream file (arquivo.c_str());
    if(file.is_open())
    {
         getline(file, n);
         return ! file.bad();
    }
        return false;

       cout << n << endl;*/
}