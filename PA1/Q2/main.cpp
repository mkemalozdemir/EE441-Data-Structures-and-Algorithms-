#include "hanoi.hpp"

int main(){

    int dia[] = {10,9,8,7,6,5,4,3,2,1};
    int* pDiameter;
    pDiameter = dia;

    Diameter Disc(pDiameter);

    Hanoi Game(sizeof(dia)/sizeof(pDiameter[0]));

    Game.initRod(pDiameter);



    solve_hanoi(Game);



    for(int i=19; i>=0; i--){
        std::cout << Game.rodChooser(2)[i] << std::endl;

    }
return 0;
}
