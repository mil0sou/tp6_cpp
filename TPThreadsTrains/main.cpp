#include <iostream>           
#include <thread>             
#include <mutex>              
#include <condition_variable> 
#include "Controleur.hpp" 

std::mutex mtx;
std::condition_variable cv;
Controleur ctrl(0);   // LE PROCESSUS CONTROLEUR

// TrainAB 
void circuleAB(int numTrain){
  std::cout << "Train n° " << numTrain << " arrive en A vers B " << endl;
  this_thread::sleep_for(chrono::milliseconds(rand() % 1000) );
  std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck,[numTrain]{return ctrl.controlinEnA(numTrain);}); // ATTENTE D'AUTORISATION DE CIRCULER
    lck.unlock();
  // DEBUT DU PARCOURS A->B
  std::cout << "Train n° "<< numTrain << " circule de A vers B  >>>>>> " << endl;
  this_thread::sleep_for(chrono::milliseconds(rand() % 100) );

  // FIN DU PARCOURS A->B
  std::cout << "Train n° " << numTrain << " quitte le tronçon de voie unique " << endl;
    lck.lock();
  ctrl.controloutEnB(numTrain);  // SIGNAL DE SORTIE AU CONTROLEUR
    lck.unlock();
  cv.notify_all();
}

// TrainBA 
void circuleBA(int numTrain){
  std::cout << "Train n° " << numTrain << " arrive en B vers A " << endl;
  this_thread::sleep_for(chrono::milliseconds(rand() % 1000) );
  std::unique_lock<std::mutex> lck(mtx);
  cv.wait(lck,[numTrain]{return ctrl.controlinEnB(numTrain);}); // ATTENTE D'AUTORISATION DE CIRCULER
  lck.unlock();
  // DEBUT DU PARCOURSs B->A
  std::cout << "Train n° "<< numTrain << " circule de B vers A  <<<<<<<<" << endl;
  this_thread::sleep_for(chrono::milliseconds(rand() % 100) );
  
  // FIN DU PARCOURS B->A
  std::cout << "Train n° " << numTrain << " quitte le tronçon de voie unique " << endl;
   lck.lock();
  ctrl.controloutEnA(numTrain);   // SIGNAL DE SORTIE AU CONTROLEUR
   lck.unlock();
  cv.notify_all();
}

int main(){
  const int nbr = 9;
  std::srand(std::time(nullptr));
  std::thread trainsAB[nbr];
  std::thread trainsBA[nbr];

  // INITALISE TRAINS ET CONTROLEUR
  for (int i = 1; i < nbr; i++){
    trainsAB[i] = std::thread(circuleAB, i);
    trainsBA[i] = std::thread(circuleBA, -i);
  }
// JOIN DES THREADS 
  for (int i = 1; i < nbr; i++) {
    if (trainsAB[i].joinable()) trainsAB[i].join(); 
    if (trainsBA[i].joinable()) trainsBA[i].join();   
  }
  return 0;
}

