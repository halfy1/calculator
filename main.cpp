#include <iostream>
#include <string>
#include "Calculator.h"


using namespace std;
int main(){
    string expression;
    getline(cin, expression);

    Calculator calc(expression);

    calc.evaluate(expression);

    cout << "Результат " << calc.get_result() << endl;


    return 0;
}