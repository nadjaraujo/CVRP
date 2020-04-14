#include <iostream>
#include <fstream>
#include <vector>
#include "cvrp.cpp"

#include<time.h>
int main()

{
    
    srand(time(NULL));

    const std::string name("../instancias_teste/P-n16-k8.txt");
    //const std::string name("../instancias_teste/P-n19-k2.txt");
    //const std::string name("../instancias_teste/P-n20-k2.txt");
    //const std::string name("../instancias_teste/P-n23-k2.txt");
    //const std::string name("../instancias_teste/P-n45-k2.txt");
    //const std::string name("../instancias_teste/P-n50-k2.txt");
    //const std::string name("../instancias_teste/P-n51-k2.txt");
    //const std::string name("../instancias_teste/P-n55-k2.txt");

    for (int i = 0; i < 10; i++){
        std::cout << "------------------------------------------EXECUCAO " << i << "------------------------------------------"<< std::endl;
         CVRP roteamento(name);

        roteamento.HVM();
    }
       
}
