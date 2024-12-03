#ifndef CONTROLEUR_HPP
#define CONTROLEUR_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>

class Controleur {
public:
    Controleur() : trains_on_track(0), direction(0) {}

    // Fonction appelée avant qu'un train entre de A vers B
    bool controlinEnA(int numero) {
        if (trains_on_track == 0) { 
            direction = 1; // Direction 1 = A->B
        }
        if (direction == 1 && trains_on_track < max_trains_on_track) {
            trains_on_track++;
            std::cout << "Train n " << numero << " est autorise a entrer sur le troncon (A->B)." << std::endl;
            return true;
        }
        return false;
    }

    // Fonction appelée avant qu'un train entre de B vers A
    bool controlinEnB(int numero) {
        if (trains_on_track == 0) {
            direction = -1; // Direction -1 = B->A
        }
        if (direction == -1 && trains_on_track < max_trains_on_track) {
            trains_on_track++;
            std::cout << "Train n " << numero << " est autorise a entrer sur le troncon (B->A)." << std::endl;
            return true;
        }
        return false;
    }

    // Fonction appelée quand un train quitte de A vers B
    void controloutEnB(int numero) {
        trains_on_track--;
        std::cout << "Train n " << numero << " quitte le troncon (A->B)." << std::endl;
        if (trains_on_track == 0) {
            direction = 0; // Libérer la direction
        }
    }

    // Fonction appelée quand un train quitte de B vers A
    void controloutEnA(int numero) {
        trains_on_track--;
        std::cout << "Train n " << numero << " quitte le troncon (B->A)." << std::endl;
        if (trains_on_track == 0) {
            direction = 0; // Libérer la direction
        }
    }

private:
    int trains_on_track;           // Nombre de trains sur le tronçon
    int direction;                 // 1 = A->B, -1 = B->A, 0 = libre
    const int max_trains_on_track = 1; // Nombre maximum de trains autorisés
};

#endif // CONTROLEUR_HPP
