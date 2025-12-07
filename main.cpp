#include <iostream>
#include <exception>
#include <limits>
#include "joc.h"
#include "obiectjoc.h"

void afiseazaMeniu() {
    int optiune = 0;
    std::cout << "Selectati dificultatea:" << std::endl;
    std::cout << "1. Usor" << std::endl;
    std::cout << "2. Mediu" << std::endl;
    std::cout << "3. Greu " << std::endl;
    std::cout << "4. Insane" << std::endl;

    while (true) {
        std::cout << "\nIntroduceti optiunea (1-4): ";


        if (!(std::cin >> optiune)) {
            std::cout << "EROARE: Te rog introdu un numar, nu litere!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            continue;
        }

        if (optiune >= 1 && optiune <= 4) {
            break;
        } else {
            std::cout <<  "EROARE: Numarul trebuie sa fie intre 1 si 4!" << std::endl;
        }
    }

    float vitezaAleasa;
    switch (optiune) {
        case 1: vitezaAleasa = 1.0f; break;
        case 2: vitezaAleasa = 1.75f; break;
        case 3: vitezaAleasa = 2.5f; break;
        case 4: vitezaAleasa = 3.5f; break;
        default: vitezaAleasa = 1.0f; break;
    }

    ObiectJoc::setViteza(vitezaAleasa);
    std::cout << "\nDificultate setata! Pornire joc..." << std::endl;
}

int main() {

    afiseazaMeniu();

    try {
        Joc joc;
        joc.run();
    } catch (const std::exception& e) {
        std::cerr << "\n!!! EROARE CRITICA !!!: " << e.what() << std::endl;
        std::cout << "\nApasa ENTER pentru a inchide..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    } catch (...) {
        std::cerr << "\n!!! EROARE NECUNOSCUTA !!!" << std::endl;
        std::cin.ignore();
        std::cin.get();
    }

    return 0;
}