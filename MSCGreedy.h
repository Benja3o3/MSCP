#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>
#include <time.h>
using namespace std;

class MSCGreedy {
    struct conjunto {
        vector<string> s;
        bool aceptado;
    };

    private:
        vector<string> U; //universo
        vector<conjunto *> F; //familia de conjuntos    
        
    public:
        MSCGreedy(vector<string> U, vector<vector<string>> F);
        ~MSCGreedy();       
        void greedySC();
        vector<string> buscaConjuntoMax(vector<conjunto *> F, vector<string> U);
        vector<vector<string>> getMSC();
};