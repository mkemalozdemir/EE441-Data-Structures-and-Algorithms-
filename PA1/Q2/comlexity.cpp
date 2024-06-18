#include "complexity.hpp"
void print_backwards(char const * _string){ //Function to print the input string backwards

    if(_string[1] == '\0'){         //Check wheteher the next character is '\0' or not
        std::cout <<_string[0];     //If '\0', print the current character.
        return;                     //exit the function
    }
    else{
        print_backwards(_string+1);//Utilize recursion to search '\0'
        std::cout << _string[0]; //Print the current values
        return;
    }
}

int gcd(int n, int m){ // Greatest common divisor function decleration
    int result;
    n = abs(n);         // Takes the absolute values of the inputs
    m = abs(m);

    if(n == 0 || m == 0){   // Check whether the inputs are 0 or not
        return 0;
    }
    else{
        while (m != 0){     //Until the m becomes 0 stay in loop
            result = n%m;   // Set remainder
            n=m;            //Change the numbers
            m=result;       //Make m equals to remainder
        }
        return n;
    }
}

int lcm(int n, int m){      //Declaration of the least common multiply function
    int result;
        n = abs(n);         // Takes the absolute values of the inputs
        m = abs(m);

        if(n == 0 || m == 0){ // Check whether the inputs are 0 or not
            return 0;
        }
        else{
            result = (n*m)/gcd(n,m); //multiply the inputs and divide it in to gcd to obtain lcm
            return result;
        }
}
