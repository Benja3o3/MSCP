#include "MSCP_EXHAUSTIVO.h"
#include <time.h>
using namespace std;

//Constructor de la clase
MSCP_EXHAUSTIVO::MSCP_EXHAUSTIVO(vector<string> universo, vector<vector<string>> familia){
	this->universo = universo;
	sort(this->universo.begin(),this->universo.end());
	this->familia = familia;
}
//Destructor de la clase
MSCP_EXHAUSTIVO::~MSCP_EXHAUSTIVO(){
	this->familia.clear();
	this->universo.clear();
}
//Algoritmo de busqueda exhaustiva
vector<vector<string>> MSCP_EXHAUSTIVO::busquedaExhaustiva(){
	unsigned t1 = clock();
	vector<vector<string>> mscp = familia;
	int min = familia.size();    
	int rows = pow(2,familia.size());      
	vector<vector<string>> v;                
	vector<bool> combinacion;
	for(unsigned i = 0; i < familia.size();i++){
		combinacion.push_back(false);
	}    
	for(int i = 0; i < rows; i++){
		for(unsigned j = 0; j < familia.size(); j++){
			if(combinacion[j] == true){
				v.push_back(familia[j]);
			}
		}
		if(unionVectores(v) == universo && int(v.size()) < min){
			mscp = v;
			min = v.size();
		}
		v.clear();
		increment(combinacion);
	}  
	unsigned t2 = clock();
	cout << "MSC Exhaustivo[" << mscp.size() << "]: " << double(t2 - t1)/CLOCKS_PER_SEC << " segundos" << endl;
	return mscp;
}
//Algoritmo de busqueda exhaustiva optimizada
vector<vector<string>> MSCP_EXHAUSTIVO::busquedaExhaustivaOptimizada(vector<int> pos_set_unicos){
	unsigned t1 = clock();
	vector<vector<string>> mscp = familia;
	int min = familia.size();    
	int rows = pow(2,familia.size());      
	vector<vector<string>> v;                
	vector<bool> combinacion;
	for(unsigned i = 0; i < familia.size();i++){
		combinacion.push_back(false);
	}
	for(int i = 0; i < rows; i++){
		for(unsigned j = 0; j < familia.size(); j++){
			//PREGUNTAR POR ELEMENTO UNICO
			if(combinacion[j]){
				v.push_back(familia[j]);
				if(int(v.size()) >= min) break;
			}
			else if(existeEnVector(pos_set_unicos, j)) break;
		}
		if(unionVectores(v) == universo && int(v.size()) < min){
			mscp = v;
			min = v.size();
		}
		v.clear();
		increment(combinacion);
	}  
	unsigned t2 = clock();
	cout << "MSC Exhaustivo Optimizado[" << mscp.size() << "]: " << double(t2 - t1)/CLOCKS_PER_SEC << " segundos" << endl;
	return mscp;
}

//Simula la union de conjuntos usando vectores
vector<string> MSCP_EXHAUSTIVO::unionVectores(vector<vector<string>> a){
	vector<string> v;
	for (vector<string> y : a){
		for (string x : y){
			if(v.empty()){
				v.push_back(x);
			}
			if(!(existeEnVector(v,x))){
				v.push_back(x);
			}
		}
	}
	sort(v.begin(),v.end());
	return v;
}
//Imprime un vector de string
void MSCP_EXHAUSTIVO::printVector(vector<string> v){
	for (string i : v){
		cout << i << "-";
	}
	cout << endl;
}
//Imprime una matriz
void MSCP_EXHAUSTIVO::printMatriz(vector<vector<string>> m){
	for (vector<string> i : m){
		for (string j : i){
			cout << j << " ";
		}
		cout << endl;
	}
}

//Incrementa en 1 la mat
void MSCP_EXHAUSTIVO::increment(vector<bool> &A){
	int i = A.size() - 1;
	while(i >= 0 && A[i] == true){
		A[i] = false;
		i--;
	}
	if(i >= 0){
		A[i] = true;
	}
}

//Funcion que verifica si existe un elemento en un vector
bool MSCP_EXHAUSTIVO::existeEnVector(vector<string> v, string busqueda) {
    return find(v.begin(), v.end(), busqueda) != v.end();
}
//Funcion que verifica si existe un elemento en un vector
bool MSCP_EXHAUSTIVO::existeEnVector(vector<int> v, int elemento){
	return find(v.begin(), v.end(), elemento) != v.end();
}