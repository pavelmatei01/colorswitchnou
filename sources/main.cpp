#include <iostream>
#include <exception>
#include "joc.h"

int main() {
    std::cout << "Pornire joc Color Switch..." << std::endl;
    std::cout << "Vei juca 5 nivele cu dificultate progresiva." << std::endl;

    try {
        Joc joc;
        joc.run();
    } catch (const std::exception& e) {
        std::cerr << "\n!!! EROARE CRITICA !!!: " << e.what() << std::endl;
        std::cin.ignore();
        std::cin.get();
    } catch (...) {
        std::cerr << "\n!!! EROARE NECUNOSCUTA !!!" << std::endl;
        std::cin.ignore();
        std::cin.get();
    }
    return 0;
}