#include "classes.hpp"

double apply_operations(OperationQueue<Operation>&_Queue, double input);
int main(){
    double result;
    Operation c1('*',pow(3,0.5));       // Create first object of operation class
    Operation c2('+',pow(6.75,0.5));    // Create second object of operation class
    Operation c3('^',2);                // Create third object of operation class
    Operation c4('-',1.75);             // Create fourth object of operation class

    OperationQueue<Operation> Q;        //Create an OperationQueue object

    Q.push_b(c1);                       //Push all the objects of the operation class into the queue
    Q.push_b(c2);
    Q.push_b(c3);
    Q.push_b(c4);

    result = apply_operations(Q,13.7); //Apply the function to solve the problem

    std::cout<< result << std::endl;    //Print the result

return 0;
}
