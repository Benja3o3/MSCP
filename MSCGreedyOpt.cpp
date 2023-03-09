#include "MSCGreedyOpt.h"

//Constructor de la clase
MSCGreedyOpt::MSCGreedyOpt(vector<string> U, vector<vector<string>> F, int k, vector<int> pos){
    this->U = U; //Universo
    this->k = k; //Familia de conjuntos
    for(int i: pos){ 
        for(unsigned j = 0; j < F[i].size(); j++){ //elimina los elementos del universo
            auto it = find(this->U.begin(), this->U.end(), F[i][j]);
            if(it != this->U.end()){ // si existe el conjunto 
                this->U.erase(it);
            }
        }
        this->MSCOpt.push_back(F[i]); // vector que tendra solamente los conjuntos del MSC
    }
    vector<vector<string>> aux;
    for(unsigned i = 0; i < F.size(); i++){
        if(find(pos.begin(),pos.end(),i) == pos.end()){
            aux.push_back(F[i]);
        }
    }
    this->F = aux;
    greedySCOpt(); //realiza SC con la familia de conjuntos y universo
}
//Destructor de la clase
MSCGreedyOpt::~MSCGreedyOpt(){
    this->U.clear();
    this->F.clear();
}
//Algoritmo greedyOptimizado usando funciones
void MSCGreedyOpt::greedySCOpt(){
	unsigned t1 = clock();
    while(!this->U.empty()){
        buscaKConjuntos();
    }
	unsigned t2 = clock();
	cout << "MSC GreedOpt[" << getMSCOpt().size() << "]: ";
	cout << double(t2 - t1)/CLOCKS_PER_SEC << " segundos" << endl;
}

//Busca los k conjuntos que unidos maximizan la la interseccion con el universo
void MSCGreedyOpt::buscaKConjuntos(){
	/* posiciones siempre es de tamaño 2: en la posicion 0 guarda las posiciones que maximizan la interseccion 
	con el universo y en 1 guarda las posiciones de los conjuntos que no aportan ningun elemento */
	vector<vector<int>> posiciones = makeCombi();
	vector<vector<string>> aux;
	for(int i: posiciones[0]){ 
		vector<string> interseccionV = interseccion(this->U,F[i]);
		//Pensar en caso cuando el size == 0
		if(interseccionV.size() > 0){
			for(unsigned j = 0; j < this->F[i].size(); j++){ //elimina los del universo
	            auto it = find(this->U.begin(), this->U.end(), this->F[i][j]);
	            if(it != this->U.end()){ // si existe el conjunto 
	                this->U.erase(it);
	            }
	        }
			this->MSCOpt.push_back(this->F[i]);
		}
    }
    aux.clear();
	/* Heuristica: guarda los conjuntos que no aportan ningun elemento nuevo y los elimina para reducir la 	
	cantidad de elementos para la siguiente combinatoria*/
	vector<int> pos_unidas = unionVectoresInt(posiciones);
    for(unsigned i = 0; i < this->F.size(); i++){
        if(find(pos_unidas.begin(),pos_unidas.end(),i) == pos_unidas.end()){
            aux.push_back(this->F[i]);
        }
    }
    this->F = aux;
	/* Heuristica*/
} 

//Funcion que crea todo lo necesario para hacer la combinatoria y resetea variables
vector<vector<int>> MSCGreedyOpt::makeCombi(){ // n es tamaño de la familia
    vector<vector<int>> V;
	vector<int> tmp; // vector de posiciones
	vector<int> mscp;
	this->max = 0;
	vector<int> posiciones2;
    makeCombiUtil(tmp, this->F.size(), 0, this->k, mscp, posiciones2);
    V.push_back(mscp);
	V.push_back(posiciones2);
	return V;
}
//Funcion que simula la interseccion usando vectores
vector<string> MSCGreedyOpt::interseccion(vector<string> v1, vector<string> v2){
	vector<string> inter;
	for(string s: v1){
		for(string a : v2){
			if(s == a) inter.push_back(s);
		}
	}
	return inter;
}

//Funcion que genera k-combinaciones de n elementos
void MSCGreedyOpt::makeCombiUtil(vector<int> &tmp, int n, int left, int k, vector<int> &mscp, vector<int> &posiciones2){
	//Si k == 0 significa que tiene los 3 elementos listos para comprobarlos
    if(k == 0) { 
		vector<vector<string>> aux;
		for(int i: tmp){//en aux se van guardando los vectores de cada combinacion
			aux.push_back(this->F[i]);
		}
		vector<string> v_i(U.size()); 
		vector<string> v2;
		if(aux.size() > 1) v2 = unionVectores(aux); 
		else v2 = aux[0];
		vector<string> inter = interseccion(this->U, v2);
		int suma = inter.size(); //suma almacena el tamaño de la interseccion
		if(suma == 0 && posiciones2.size() <= this->F.size()){
			for(int i: tmp){
				posiciones2.push_back(i);
			}
		}
		if(suma >= this->max){
			mscp = tmp; //mscp almacena el vector de conjuntos correcto
			this->max = suma;
		}
        return;
    } 								// 1 2 3
    for(int i = left; i < n; i++){ //(o(n)) ///
        tmp.push_back(i);
        makeCombiUtil(tmp, n, i + 1, k - 1, mscp, posiciones2); // o(n(n - 1))k+  // k*T(n-1)+h k*T(n-2)+h 
        tmp.pop_back(); // T(n) = n ((T[n-1] * k) + h)
    }
}
//Funcion que simula la union usando vectores
vector<string> MSCGreedyOpt::unionVectores(vector<vector<string>> a){
	vector<string> v;
	for(vector<string> y : a){
		for(string x : y){
			if(!(existeEnVector(v,x))){
				v.push_back(x);
			}
		}
	}
	return v;
}
//Funcion que simula la union usando vectores
vector<int> MSCGreedyOpt::unionVectoresInt(vector<vector<int>> a){
	vector<int> v;
	for(vector<int> y : a){
		for(int x : y){
			if(!(existeEnVector(v,x))){
				v.push_back(x);
			}
		}
	}
	return v;
}
//Funcion que verifica si existe un elemento en un vector
bool MSCGreedyOpt::existeEnVector(vector<int> v, int busqueda) {
    return (find(v.begin(), v.end(), busqueda) != v.end());
}
//Funcion que verifica si existe un elemento en un vector
bool MSCGreedyOpt::existeEnVector(vector<string> v, string busqueda) {
    return (find(v.begin(), v.end(), busqueda) != v.end());
}
//getter
vector<vector<string>> MSCGreedyOpt::getMSCOpt(){
    return this->MSCOpt;
}

void MSCGreedyOpt::print(vector<int> V){
	for(int i : V)cout << i << " ";
	cout << endl;
}

void MSCGreedyOpt::print(vector<string> V){
	for(string i : V) cout << i << " ";
	cout << endl;
}