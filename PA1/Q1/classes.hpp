#include <iostream>
#include <cmath>
#define MaxQSize 10
class Operation{                        //Class declaration of the class that does the operation

    private:

        char _operator;                //The variable holds operators which are +,-,*,/,^
        double _number;                //Holds the number we used as operand

    public:

        Operation(char a, double b);    //Constructor to determine private variables
        Operation();                    //initializer
        double calculator(double num);  //Calculates the result
};

template<class T>
class OperationQueue{                   //Queue class declaration

private:
        int _front,_rear,_count;        //Private variables to make queue work
        T Qlist[MaxQSize];              //An array for holds something in queue
    public:

        OperationQueue();               //Constructor of the class
        void push_b(const T& item);     //Push function to insert object of T in queue
        T pop_f();                      //Pop function to take the object away from queue
        bool Qempty () const;           //Checks whether the queue is empty or not
};



