#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>
#include <math.h> 
#include <stdio.h> 
using namespace std;

class MSCGreedyOpt{
    private:
        vector<string> U; //universo
        vector<vector<string>> F; //familia de conjuntos
        vector<vector<string>> MSCOpt;

        int k;    
		int max;
        
    public:
        MSCGreedyOpt(vector<string> U, vector<vector<string>> F, int k, vector<int> pos);
        ~MSCGreedyOpt();       
        void greedySCOpt();
        void buscaKConjuntos();
        vector<vector<string>> getMSCOpt();
		vector<int> unionVectoresInt(vector<vector<int>> a);

		vector<string> unionVectores(vector<vector<string>> a);//simula la union
		bool existeEnVector(vector<string> v, string busqueda);
		bool existeEnVector(vector<int> v, int busqueda);
		void makeCombiUtil(vector<int> &tmp, int n, int left, int k,vector<int> &mscp, vector<int> &posiciones2);
		vector<vector<int>> makeCombi();
		vector<string> interseccion(vector<string> v1, vector<string> v2);
		void print(vector<int> V);
		void print(vector<string> V);
};
