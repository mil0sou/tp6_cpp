#ifndef CONTROLEUR_HPP
#define CONTROLEUR_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

class Controleur {
public:
    Controleur() : trains_on_track(0), direction(0) {}

    // Méthode appelée pour demander à entrer sur le tronçon en direction de B
    bool controlinEnA(int numero) {
        unique_lock<mutex> lck(mtx);
        // Attente tant que le tronçon est occupé par des trains dans l'autre direction
        while (trains_on_track > 0 && direction != 1) {
            cv.wait(lck);
        }
        // Autoriser l'entrée
        direction = 1;  // Direction A->B
        trains_on_track++;
        cout << "Train n°" << numero << " est autorisé à entrer sur le tronçon (A->B)." << endl;
        return true;
    }

    // Méthode appelée pour demander à entrer sur le tronçon en direction de A
    bool controlinEnB(int numero) {
        unique_lock<mutex> lck(mtx);
        // Attente tant que le tronçon est occupé par des trains dans l'autre direction
        while (trains_on_track > 0 && direction != -1) {
            cv.wait(lck);
        }
        // Autoriser l'entrée
        direction = -1;  // Direction B->A
        trains_on_track++;
        cout << "Train n°" << numero << " est autorisé à entrer sur le tronçon (B->A)." << endl;
        return true;
    }

    // Méthode appelée pour signaler la sortie du tronçon en direction de B
    bool controloutEnB(int numero) {
        unique_lock<mutex> lck(mtx);
        trains_on_track--;
        cout << "Train n°" << numero << " a quitté le tronçon (A->B)." << endl;
        // Libérer le tronçon si aucun train n'y est présent
        if (trains_on_track == 0) {
            direction = 0;  // Libérer la voie
            cv.notify_all();
        }
        return true;
    }

    // Méthode appelée pour signaler la sortie du tronçon en direction de A
    bool controloutEnA(int numero) {
        unique_lock<mutex> lck(mtx);
        trains_on_track--;
        cout << "Train n°" << numero << " a quitté le tronçon (B->A)." << endl;
        // Libérer le tronçon si aucun train n'y est présent
        if (trains_on_track == 0) {
            direction = 0;  // Libérer la voie
            cv.notify_all();
        }
        return true;
    }

private:
    int trains_on_track;           // Nombre de trains actuellement sur le tronçon
    int direction;                 // Direction actuelle : 1 pour A->B, -1 pour B->A, 0 pour libre
    mutex mtx;                     // Mutex pour protéger les données partagées
    condition_variable cv;         // Condition variable pour la synchronisation
};

#endif // CONTROLEUR_HPP
