#include <iostream>
#include <cstdlib>  // for system()

int main() {
    std::cout << "This will be cleared.\n";
    system("pause");  // Optional: wait for user input
    system("cls");    // Clears the terminal screen
    std::cout << "Screen cleared!\n";
    return 0;
}