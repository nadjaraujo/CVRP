#include <iostream>
#include <fstream>
#include <vector>
#include "cvrp.cpp"

#include<time.h>
int main()

{
    
    srand(time(NULL));
    //string que contem o caminnho pra as instancias
    //const std::string name("../instancias_teste/P-n16-k8.txt");
    //const std::string name("../instancias_teste/P-n19-k2.txt");
  //  const std::string name("../instancias_teste/P-n20-k2.txt");
   //const std::string name("../instancias_teste/P-n23-k8.txt");
   const std::string name("../instancias_teste/P-n45-k5.txt");
   //const std::string name("../instancias_teste/P-n50-k10.txt");
   //const std::string name("../instancias_teste/P-n51-k10.txt");
   // const std::string name("../instancias_teste/P-n55-k7.txt");

    for (int i = 0; i < 10; i++){
        std::cout << "------------------------------------------EXECUCAO " << i << "------------------------------------------"<< std::endl;
         CVRP roteamento(name);
          //todos os metodos foram criados dentro da classe cvrp.

        roteamento.HVM();
        //chamada da heuristica
    }


       
}
