#include "classes.hpp"

Operation::Operation(char a, double b): _operator(a), _number(b){}  //Constructor that determines the private variables
Operation::Operation() : _operator('+'), _number(0.0) {}    //Initializer

double Operation::calculator(double num){   //Calculator function

    double result;

    if(_operator =='+'){
        result = num + _number;
    }
    else if(_operator =='-'){
        result = num - _number;
    }
    else if(_operator =='*'){              //According to the operator that is held in the object we created, does the operations
        result = num * _number;
    }
    else if(_operator =='/'){
        result = num / _number;
    }
    else if(_operator =='^'){
        result = pow(num,_number);
    }
    return result;
}

template<class T>
OperationQueue<T>::OperationQueue(): _front(0), _rear(0),_count(0){}    //Constructor and initializer for the queue

template<class T>
void OperationQueue<T>::push_b(const T& item){      //Pushes the item to the queue

    if(_count == MaxQSize){                         //If queue is full shifts the front
        _front = (_front+1)%MaxQSize;
    }
    else{
        _count++;                                   //If queue is not full, increment the count
    }
    Qlist[_rear] = item;                            //Stores the item in the place that is pointed by rear
    _rear = (_rear+1)%MaxQSize;                     //Shift the rear

}
template<class T>
T OperationQueue <T>::pop_f(){                      //Pop the item from queue
    T temp;

    if(_count){                                     //If there is any item in queue

        temp = Qlist[_front];
        _front = (_front+1)%MaxQSize;               //Pops it from the queue and updates the front and count
        _count--;
    }
    else{
        std::cout << "Queue is empty" << std::endl; //If there is no item in queue, prints the message
    }
    return temp;
}
template<class T>
bool OperationQueue<T>::Qempty()const{             //Checks whether the queue is empty
    if(_count == 0){
       return true;
    }
    else {
        return false;
    }
    ;
}

double apply_operations(OperationQueue<Operation>&_Queue, double input){   //Function to solve the problem
    double result;
    Operation op;
    op = _Queue.pop_f();                       //Pops the front object
    result = op.calculator(input);             //Does the operation with the object and stores it in result

    while(!_Queue.Qempty()){                   //Until the queue becomes empty repeat the step above
    op = _Queue.pop_f();
    result = op.calculator(result);
    }
    return result;                              //When queue becomes empty return the result
}

template class OperationQueue<Operation>;       //Template declaration for the OperationQueue class

