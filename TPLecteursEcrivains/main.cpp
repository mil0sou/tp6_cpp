#include <iostream>           
#include <thread>             
#include "Semaphore.hpp"

int ressourcePartagee;

Semaphore lec(1);
Semaphore ecr(1);

void lecteur(int numLecteur){
  for (int i = 0; i < 4; i++){
    lec.P(ressourcePartagee);
    std::cout << "Lecteur no " << numLecteur << " en cours " << endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 20000) );
    std::cout << "        Valeur lue = " << ressourcePartagee << "  " << endl;
    lec.V(ressourcePartagee);
  }
}

void ecrivain(int numEcrivain){
  int x;
  for (int i = 0; i < 4; i++){
    ecr.P(ressourcePartagee);  
    std::cout << "Ecrivain no " << numEcrivain << " en cours " << endl;
    x = ressourcePartagee;
    this_thread::sleep_for(chrono::milliseconds(rand() % 20000) );
    std::cout << "valeur a incrementer de la ressourcePartagee = " << x << "  " << endl;
    ressourcePartagee = x+1 ;
    ecr.V(ressourcePartagee);
  }
}

int main() {
  const int nbr = 8;
  std::thread r[nbr];
  std::thread w[nbr];
  int ressourcePartagee = 0;

  for (int i = 1; i < nbr; i++){
    r[i] = std::thread(lecteur, -i);
    w[i] = std::thread(ecrivain, i);
  }
// Join des threads 
  for (int i = 1; i < nbr; i++) {
  r[i].join(); 
  w[i].join();   
  }
  return 0;
}
