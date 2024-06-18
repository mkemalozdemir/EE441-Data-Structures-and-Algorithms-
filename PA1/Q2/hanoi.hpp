#include <iostream>

class Diameter{                 //Declaration of the Diameter class for the discs' diameters

    private:
        int _diameter;          //Private variable declaration to hold diameter of the disc

    public:

        Diameter(int _dia);     // Constructor of the Diameter class
        int getDiameter()const; // Provides access the private variable

};

class Hanoi{                    // Hanoi class declaration

    private:

        int rod0[20];
        int rod1[20];           //Private class variables
        int rod2[20];           //Rods are for store the discs, numDisc is for the number of discs
        int numDisc;

    public:

        Hanoi(int number);      //Hanoi class constructor
        void _move(int from, int to); //Provides transportation from one rod to another
        int discIndicator(int rod); // Returns to the disc which is on the top in a rod
        int* rodChooser(int rd);// Returns the desired rod
        int counter(int* arr); //Counts how many disc there is in the determined rod
        int GetNumDisc() const;//Returns the total number of discs
        int source =0;         //Public variable to use rod0 as source
        int aux=1;             //Public variable to use rod1 as auxillary
        int dest=2;            //Public variable to use rod2 as destination




};
void hanoisolver(Hanoi& obj, int dnum, int s, int a, int d);
void solve_hanoi(Hanoi& game);
