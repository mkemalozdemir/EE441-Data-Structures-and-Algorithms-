#include "hanoi.hpp"

void solve_hanoi(Hanoi& game) {                                         //Function declaration that solves the game
    hanoisolver(game,game.GetNumDisc(),game.source,game.aux,game.dest); //Calling another function that does the operation
}



Diameter::Diameter(int _dia):_diameter(_dia){}     //Constructor of diameter determines the diameter

int Diameter::getDiameter()const{                  //Returns the diameter in Diameter class
    return _diameter;
}

Hanoi::Hanoi(int number):numDisc(number){          //Constructor for Hanoi class,it determines the number of discs and
                                                   //initializes the rods.
    Diameter discdia(GetNumDisc());
    int j=0;

    for(int i=discdia.getDiameter(); i>0; i--){     //Insterts the discs in rod0

        rod0[j] = i;
        j++;
    }
    for(int i=discdia.getDiameter(); i<20; i++){    //Fills the spaces with 0

        rod0[i] = 0;
    }
    for(int i=0; i<20; i++){                        ////Fills the spaces with 0 in rod1 and rod2

        rod1[i] = 0;
        rod2[i] = 0;
    }

}


int Hanoi::discIndicator(int rod){  //Returns the disc on the top in the rod
    int i=0;
    if(rod == 0){
        i = counter(rod0);          //Count how many discs there are in the specific rod
        if(i != 0){
            return rod0[i-1];       //If there is one or more discs return to the disc on the top
        }
        else{
            return rod0[i];         //If there is not any disc return to 0
        }
    }
    if(rod == 1){
        i = counter(rod1);
        if(i != 0){
            return rod1[i-1];
        }
        else{
            return rod1[i];
        }
    }
    if(rod == 2){
        i = counter(rod2);
        if(i != 0){
            return rod2[i-1];
        }
        else{
            return rod2[i];
        }
    }

}

int* Hanoi::rodChooser(int rd){         // Choose the rod according to the number input
    if (rd == 0){
        return rod0;
    }
    if (rd == 1){
        return rod1;
    }
    if(rd == 2){
        return rod2;
    }
}

int Hanoi::counter(int* arr){          //Counts how many discs there are in a rod
    int counter = 0;
    for(int i=0; i<20; i++){
        if(arr[i] != 0){
            counter++;
        }
    }
    return counter;
}
// Move one disc from a rod to another
void Hanoi::_move(int from, int to){
    if((from == 0 || from== 1 || from == 2)&&(to == 0 || to == 1 || to == 2)&&(from!=to)){  // Check the inputs whether they are valid
        if((discIndicator(from) < discIndicator(to)|| discIndicator(to)== 0)&& discIndicator(from)!=0){ //Check  the rule of the game
            std::cout << "Disc "<< discIndicator(from) << " is moved from Rod "<< from << " to " << to << std::endl; //Print movement
            rodChooser(to)[counter(rodChooser(to))] = discIndicator(from);  //Move the disc to destination rod
            rodChooser(from)[counter(rodChooser(from))-1] = 0;  //Insert 0 instead of the disc that is moeved in the source rod
        }
        else{
            std::cout << "Illegal Move!" << std::endl; //Print warning message if the move is illegal
        }
    }
    else {
        std::cout << "Illegal Move!" << std::endl;  //Print warning message if the move is illegal
    }
}

int Hanoi::GetNumDisc() const{  // Gives how many rods there are in total
    return numDisc;
}

void hanoisolver(Hanoi& obj, int dnum, int s, int a, int d){    //Function takes 3 rods(source,auxillary,destination) to solve the game

    if  (dnum == 1){         //if the total number of discs is 1 move the disc from source to destination
        obj._move(s,d);
        return;
    }

    hanoisolver(obj,dnum-1,s,d,a);  //Utilize recursion the transport n-1 discs from source rod to auxillary rod by using destination rod

    obj._move(s,d);                 //Move the biggest disc to destination rod

    hanoisolver(obj, dnum-1, a,s,d);    //Utilize recursion to transport n-1 discs from auxillary rod to destination rod by using source rod

}
