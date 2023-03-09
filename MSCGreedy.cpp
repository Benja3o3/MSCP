#include "MSCGreedy.h"

// Funcion que genera un "vector de nodos" a partir de la ruta del archivo
MSCGreedy::MSCGreedy(vector<string> U, vector<vector<string>> F){
    this->U = U;
    vector<conjunto *> vector_conjuntos;
    
    for(vector<string> v : F){
        conjunto *c1 = new conjunto();
        c1->s = v;
        c1->aceptado = false;
        vector_conjuntos.push_back(c1);
    }
    this->F = vector_conjuntos;

    greedySC(); //realiza SC con la familia de conjuntos y universo
}

MSCGreedy::~MSCGreedy(){
    U.clear();
    F.clear();
}

void MSCGreedy::greedySC(){
	unsigned t1 = clock();
    // U : [1,4,7,2,8,14,23]   X {0: [1,4,7], 1: [1,2,8,14,23]....}
    while(!U.empty()){
        vector<string> conjuntoMax = buscaConjuntoMax(F,U);
        for(unsigned i = 0; i < conjuntoMax.size(); i++){
            auto it = find(U.begin(), U.end(), conjuntoMax[i]);
            if(it != U.end()){ // si esta el conjunto 
                U.erase(it);
            }
        }   
    }
	unsigned t2 = clock();
	cout << "MSC Greedy[" << getMSC().size() << "]: " << double(t2 - t1)/CLOCKS_PER_SEC << " segundos" << endl;
}

vector<string> MSCGreedy::buscaConjuntoMax(vector<conjunto *> F, vector<string> U){
    int numActual = 0, pos = 0, suma, mayorSuma = 0;
    for(unsigned i = 0; i < F.size(); i++){
        if(F[i]->aceptado == false){ //si el conjunto no ha sido seleccionado
            suma = 0;
            for(unsigned j = 0; j < F[i]->s.size(); j++){
                if(find(U.begin(), U.end(), F[i]->s[j]) != U.end()){
                    suma++;
                }
            }
            if(suma > mayorSuma){ // encuentra el conjunto que contiene un mayor numero de apariciones nuevas en el universo
                mayorSuma = suma;
                pos = numActual;
            } 
        }
        numActual++;
    }
    F[pos]->aceptado = true;
    return F[pos]->s;
}

vector<vector<string>> MSCGreedy::getMSC(){
    //imprime los subconjuntos que forman el MSC
    vector<vector<string>> v;
    for(unsigned i = 0; i <F.size(); i++){
        if(F[i]->aceptado == true){
            v.push_back(F[i]->s);
        }
    }
    return v;
}