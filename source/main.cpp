#include <iostream>
#include <fstream>
#include <vector>
#include "cvrp.hpp"
#include "movimentos.hpp"

int main()
{
    const std::string name("../instancias_teste/P-n16-k8.txt");
    //const std::string name("./instancias_teste/P-n55-k7.txt");
    CVRP roteamento(name);

    roteamento.HVM();
}