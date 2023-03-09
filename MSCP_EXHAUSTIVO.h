#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

class MSCP_EXHAUSTIVO {
    private:
        vector<string> universo; //universo
        vector<vector<string>> familia;//familia de conjuntos    
    public:
		MSCP_EXHAUSTIVO(vector<string> universo, vector<vector<string>> familia);
        ~MSCP_EXHAUSTIVO();

        vector<vector<string>> busquedaExhaustiva();
        vector<string> unionVectores(vector<vector<string>> a);
        void printVector(vector<string> v);
        void printMatriz(vector<vector<string>> m);
        void increment(vector<bool> &A);
        bool existeEnVector(vector<string> v, string busqueda);
		vector<vector<string>> busquedaExhaustivaOptimizada(vector<int> pos_set_unicos);
		bool existeEnVector(vector<int> v, int elemento);
};