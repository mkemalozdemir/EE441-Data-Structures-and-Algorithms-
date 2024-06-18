#include <iostream>
#include <string>

enum TokenType{Operator, Number};
                                        //Enumerate Operator,Number,and operators
enum OperatorType{Add,Sub,Mult,Div};

///TOKEN LIST & LEXER
///----------------------------------------------------------------------------------------------------------------------
class Token{  // Token class declaration, objects of this class will be used as nodes of the linked list

    protected:
        TokenType typeT;    //Private variable to indicate the token type as number or operator

    public:
        Token* next;        //Next pointer of the node is declared to connect nodes in linked list
        Token(TokenType type):typeT(type), next(nullptr){}  //constructor of the class
        TokenType tokenTypeGetter() const { //Returns the private variable
            return typeT;
        }

        ~Token(){}  //Destructor of the class
};

class OperatorToken:public Token{   //Inherited class of Token class to hold operators

    private:
        OperatorType tOp;           //Private variable to hold operator
    public:
        OperatorToken(OperatorType type):Token(TokenType::Operator),tOp(type){} //constructor of the class
        OperatorType operatorTypeGetter() const{    //Returns private variable
            return tOp;
        }
};
class NumberToken:public Token{ //Inherited class of Token class to hold double number

    private:
        double number;  //Private variable to hold number
    public:
        NumberToken(double num):Token(TokenType::Number),number(num){}  //Constructor of the class
        double numberGetter()const{     //Returns private variable
            return number;
        }
};
class TokenList {   //TokenList class declaration to create linked list, it has necessary variables and functions

    private:
        Token* head;    //Private pointer head indicates the start of the linked list
    public:
        TokenList():head(nullptr){} //Constructor of the class
        ~TokenList(){               //Destructor of the class/linked list
            DeleteList();
        }
    void InsertToken(Token* p){     //Function inserts node to the end of the list
        if(!head){                  //If list is empty, insert the token as first node
           head = p;
        }
        else{                       //If it is not empty, search and find the last node and inserts the input token at the end of the list
            Token* current = head;
            while (current->next) { //Loop to search the last node
                current = current->next;
            }
            current->next = p;
        }
    }
    void DeleteList(){  //Function deletes all the linked list
        while(head){
            Token* temp;
            temp = head;
            head = head->next;
            delete temp;
        }
    }
    Token* DeleteToken(Token* a){   //Function deletes the input token from the linked list
        Token* temp = a;
        a = a->next;
        return temp;
    }
    Token* headGetter() const{      //Returns the private variable
        return head;
    }
};

TokenList lexer(const std::string& str) { //Function takes string input to create Token linked list
    TokenList _list;    //An object of TokenList is declared
    std::string holder;
    int i = 0;
    int j = 0;

    for (i; i < str.length();) {    //Loop inserts the number or operator in holder at a cycle
        for (i + j; str[i + j] != ' ' && str[i + j] != '\0';) {
            holder[j] = str[i+j];
            j++;
        }
        holder[j] = '\0'; //Closes the string
        i = i+j + 1;
        if ((holder[0] == '+' || holder[0] == '-' || holder[0] == '*' || holder[0] == '/')) {
            if(holder[0] == '+'){   //Addition operator is inserted in the list
                _list.InsertToken(new OperatorToken(OperatorType::Add));
            }
            else if(holder[0] == '-'){
                if(j>2){                //Subtraction operator is inserted in the list if the number is not negative,if negative than inserts the negative number into the list
                   double num = std::stod(holder);
                    _list.InsertToken(new NumberToken(num));
                }
                else{
                  _list.InsertToken(new OperatorToken(OperatorType::Sub));
                }
            }
            else if(holder[0] == '*'){  //Multiplication operator is inserted in the list
                _list.InsertToken(new OperatorToken(OperatorType::Mult));
            }
            else {  //Division operator is inserted in the list
                _list.InsertToken(new OperatorToken(OperatorType::Div));
            }

        } else { //Number is inserted in the list
            double num = std::stod(holder);
                _list.InsertToken(new NumberToken(num));
        }
        j = 0;
    }
    return _list; //Function returns as a linked list
}
///SYNTAXTREE & PARSER
///----------------------------------------------------------------------------------------------------------------------

class Node {    //Node class is declared because its objects will be used in tree as nodes
public:
    virtual ~Node() {} //Destructor of an object
    virtual double getValue() const = 0; //Function do mathematical operation
    virtual void print() const = 0; //Function does printing
};

class OperatorNode : public Node {
private:
    OperatorType tOP; //OperatorNode objects holds the type of operation as TypeOperator
    Node* Left; // Left and Right pointers of the nodes in the tree, this pointers are declared in OperatorNode class
    Node* Right;// because if a node in the tree holds a number, this node is leave

public:
    //Constructor of OperatorNode objects, also it forces the Node base class to behave as OperatorNode
    OperatorNode(OperatorType op, Node* left, Node* right) : tOP(op), Left(left), Right(right) {}

    ~OperatorNode() { //Destructor of OperatorNode objects
        delete Left;
        delete Right;
    }

    double getValue() const override { //Function does math operations for the OperatorNode objects
        double leftValue = Left->getValue();    //Recursive function call is needed to start calculate from bottom in the tree
        double rightValue = Right->getValue();

        if(tOP == OperatorType::Add)   //According to the type of operator, calculations are done
                return leftValue + rightValue;
        else if(tOP == OperatorType::Sub)
                return leftValue - rightValue;
        else if(tOP == OperatorType::Mult)
                return leftValue * rightValue;
        else if(tOP == OperatorType::Div)
                return leftValue / rightValue;
    }

    void print() const override { //Function for printing on the terminal
        std::cout << "(";
        Left->print();  //Recursive function call is needed to start from the bottom, InOrder search

        if(tOP == OperatorType::Add)
            std::cout << " + ";
        else if(tOP == OperatorType::Sub)
            std::cout << " - ";
        else if(tOP == OperatorType::Mult)
            std::cout << " * ";
        else if(tOP == OperatorType::Div)
            std::cout << " / ";

        Right->print(); //Recursive function call is needed to start from the bottom, InOrder search
        std::cout << ")";
    }
};

class NumberNode:public Node{ //NumberNode class holds double number

    private:
        double number;
    public:
        NumberNode(double x):number(x){}
        double getValue() const override {  //Function does not do any calculation because nodes holding numbers have left and right pointers as nullptr
            return number;
        }
        void print() const override {   //Function prints only the number in the node because nodes holding numbers have left and right pointers as nullptr
            std::cout << number;
        }
};

class SyntaxTree { // Class declaration to create tree
private:
    Node* root; //Objects of the class holds a Node pointer to indicate the root of the tree

public:
    SyntaxTree(Node* r) : root(r) {} //Constructor of the objects of the class

    ~SyntaxTree() { //Constructor of the objects of the class
        delete root;
    }

    double calc() const {   //Function evaluates the result of the Polish Notation which is inserted in a tree
        if (root) {
            return root->getValue();
        }
    }

    void print() const {        //Function prints the Polish Notation which is inserted in a tree
        if (root) {
            root->print();
        }
    }
    Node* rootGetter()const{       //Returns the private variable however it is unused
        return root;
    }
};

Node* helpParser(Token*& T);
SyntaxTree parse(TokenList& tokens);
void calculate(const std::string& expression);


// Helper function to parse expressions
Node* helpParser(Token*& T) { //Function takes a pointer that points the
                                        //This function is needed because parse function takes TokenList input
                                        //However I use Tokens to do parsing
    if (!T) {
        return nullptr;
    }
    switch(T->tokenTypeGetter()){
        case TokenType::Operator:{ //If token type is operator, then reach to the operator to insert it into tree
            OperatorType opType = static_cast<OperatorToken*>(T)->operatorTypeGetter();
            T = T->next; // Move to the next token

            Node* leftOperand = helpParser(T);  //Recursive function call is needed to do insertion properly
            Node* rightOperand = helpParser(T);
            return new OperatorNode(opType, leftOperand, rightOperand); //Creation of a node to hold operator
        break;}

        case TokenType::Number:{ //If token type is number, then reach the to the number to insert it into tree
            double number = static_cast<NumberToken*>(T)->numberGetter();
            T = T->next; // Move to the next token
            return new NumberNode(number); //Creation of a node to hold number*/
        break;}

    }
    return nullptr;
}

// Step 8: Write a parse function
SyntaxTree parse(TokenList& tokens) {   //Parse function declaration
    Token* currentToken = tokens.headGetter();
    return SyntaxTree(helpParser(currentToken));    // By using the helpParser a tree is created from a linked list
}

void calculate(const std::string& str) { //Implement the calculate function by using the functions that I implemented before
    TokenList tokens = lexer(str);
    SyntaxTree syntaxTree = parse(tokens);
    std::cout << "Infix Expression: ";
    syntaxTree.print();
    std::cout << " = " << syntaxTree.calc() << std::endl;
}
