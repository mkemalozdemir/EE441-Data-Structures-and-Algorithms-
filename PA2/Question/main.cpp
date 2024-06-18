#include "headerSon.hpp"

int main() {
    std::string str;

    std::cout<< "Enter the Polish Notation: "<<std::endl;
    std::getline(std::cin,str);

    calculate(str);

    return 0;
}

