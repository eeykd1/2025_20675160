#include <iostream>  // Fixes 'cout' and 'cin' errors
#include <stdlib.h>  // Fixes 'system' error
#include "adder.h"   // Fixes 'add' error

int main() {
    int a, b;

    // Use std::cout to print to the screen
    std::cout << "Enter first number: ";
    std::cin >> a;

    std::cout << "Enter second number: ";
    std::cin >> b;

    // Call the add function from your library
    std::cout << "The sum is: " << add(a, b) << std::endl;

    // Keep the window open so you can see the result
    system("pause");

    return 0;
}
