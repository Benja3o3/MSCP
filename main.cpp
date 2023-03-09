#include "MSCGreedy.h"
#include "MSCGreedyOpt.h"
#include "MSCP_EXHAUSTIVO.h"
using namespace std;

void print(vector<int> V);
void printSeparador();
vector<string> recuperaNombresArchivos(string filename);
string transformaDecimal(string numero);
map<string,vector<int>> creaEjemploSinArchivo(int m_sets, int max_set, int min_set,double mean, double sdv, vector<vector<string>> &family, vector<string> &universe);
vector<int> conjuntosConElementosUnicos(map<string, vector<int>> conjuntos_claves);
bool existeEnVector(vector<string> Universo, string x);
bool existeEnVector(vector<int> Universo, int x);
void agregaAlMap(map<string, vector<int>> &conjuntos_claves, vector<string> conjunto, int pos);
vector<string> setOfLine(string linea, vector<string> &U, map<string, vector<int>> &conjuntos_claves, int pos);
map<string,vector<int>> recuperaArchivo(string filename, vector<vector<string>> &F, vector<string> &U);
void creaEjemplo(int m_conjuntos, int max_conjunto, int min_conjunto,double mean, double sd);
void creaArchivo(string texto, string direccion);
string experimentacion(int k,int min_size_set, int max_size_set,int min_size_conj, int max_size_conj,double mean, double sd);
string experimentacionDatosReales(vector<string> filename);
string tiempoEjecucion(unsigned t1, unsigned t2);

int main(int argc, char **argv){
    if(argc != 9){
        cout << "Ejecuta ./main k min_size_set max_size_set min_size_conj max_size_conj mean sd filename" << endl;
        exit(EXIT_FAILURE);
    }
	unsigned start = clock();
	//Se recuperan las variables que se utilizaran
	int k = atoi(argv[1]), max_size_set = atoi(argv[3]) ,min_size_set = atoi(argv[2]);
	double mean = atof(argv[6]), sd = atof(argv[7]);
	string filename = argv[8];
	int min_size_conj = atoi(argv[4]);
	int max_size_conj = atoi(argv[5]);
	//Se realizan las experimentaciones con datos generados y datos reales
	
	string experimento = experimentacion(k,min_size_set,max_size_set,min_size_conj,max_size_conj,mean,sd);
	string experimento2 = experimentacionDatosReales(recuperaNombresArchivos(filename));

	//Se recuperan los datos de tiempo
	creaArchivo(experimento,"data/tiemposDatosAleatorios.txt");
	creaArchivo(experimento2,"data/tiemposDatosReales.txt");
	
	unsigned end = clock();
	printSeparador();
	cout << "TIEMPO TOTAL DEL PROGRAMA: " << tiempoEjecucion(start, end) << endl;
    
    return 0;
}

//Funcion que realiza la experimentacion con datos generados aleatoriamente
string experimentacion(int k,int min_size_set, int max_size_set,int min_size_conj, int max_size_conj,double mean, double sd){
	printSeparador();
	cout << "\nEXPERIMENTACION CON DATOS ALEATORIOS" << endl;
	string datos = "F.size()-MSCExhaustivo-MSCExhaustivoOp-MSCGreedy-MSCGreedyOp\n";
	for (int i = 0; i < k; i++){
		unsigned t1, t2;
		int numero_conjuntos = min_size_set+ rand()%(max_size_set-min_size_set);
        vector<string> U;
        vector<vector<string>> F;
		vector<int> posiciones = conjuntosConElementosUnicos(creaEjemploSinArchivo(numero_conjuntos, max_size_conj, min_size_conj, mean, sd, F, U));
		datos += to_string(F.size()) + "-";
		printSeparador();
		cout << "Iteracion: " << i+1 << endl;
		cout << "Tamaño F: " << F.size() << endl;
		cout << "Tamaño U: " << U.size() << endl << endl;
		
		//Solucion 1
		MSCP_EXHAUSTIVO msc_exhaustivo(U,F);
		if (F.size() < 24){
			t1 = clock();
			vector<vector<string>> solucion1 = msc_exhaustivo.busquedaExhaustiva();
			t2 = clock();
			datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		}
		else datos += "0-";
		
		//Solucion 2
		if (F.size() < 24){
			t1 = clock();
	    	vector<vector<string>> solucion2 = msc_exhaustivo.busquedaExhaustivaOptimizada(posiciones);
			t2 = clock();
			datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		}
		else datos += "0-";
		
		//Solucion 3
        t1 = clock();
		MSCGreedy msc_greedy(U,F);
        vector<vector<string>> solucion3 = msc_greedy.getMSC();
		t2 = clock();
		datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		
		//Solucion 4
		cout << endl;
		vector<vector<string>> aux;
		
		for(int i: posiciones){ 
			aux.push_back(F[i]);
    	}
		vector<string> aux2 = msc_exhaustivo.unionVectores(aux);
		int limite = log(min(U.size()-aux2.size(), (F.size() - posiciones.size())));
		if (limite < 1) limite = 1;
		for(int i = 1; i <= limite; i++){
			t1 = clock();
			cout << "Greedy optimizado con K = " << i << endl;
			MSCGreedyOpt msc_greedy_optimizado(U,F,i,posiciones);
			vector<vector<string>> solucion4 = msc_greedy_optimizado.getMSCOpt();
			t2 = clock();
			if (i != limite) datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
			else datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "\n";
		}
		printSeparador();
    }
	return datos;
}

//Funcion que realiza la experimentacion con datos extraidos de los archivos .txt de la carpeta data
string experimentacionDatosReales(vector<string> nombres_archivos){
	printSeparador();
	cout << "\nEXPERIMENTACION CON DATOS REALES" << endl;
	string datos = "NombreArchivo-F.size()-MSCExhaustivo-MSCExhaustivoOp-MSCGreedy-MSCGreedyOp\n";
	unsigned t1, t2;
	for (int i = 0; i < int(nombres_archivos.size()); i++){
		datos += nombres_archivos[i] + "-";
        vector<string> U;
        vector<vector<string>> F;
	    vector<int> posiciones = conjuntosConElementosUnicos(recuperaArchivo(nombres_archivos[i],F,U)); 
		
		datos += to_string(F.size()) + "-";
		
		printSeparador();
		cout << "Nombre archivo: " << nombres_archivos[i] << endl; 
		cout << "Tamaño F: " << F.size() << endl;
		cout << "Tamaño U: " << U.size() << endl << endl;
		
        //Solucion 1
		MSCP_EXHAUSTIVO msc_exhaustivo(U,F);
		if (F.size() < 20){
			t1 = clock();
			vector<vector<string>> solucion1 = msc_exhaustivo.busquedaExhaustiva();
			t2 = clock();
			datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		}
		else datos += "0-";
		
		//Solucion 2
		if (F.size() < 30){
			t1 = clock();
	    	vector<vector<string>> solucion2 = msc_exhaustivo.busquedaExhaustivaOptimizada(posiciones);
			t2 = clock();
			datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		}
		else datos += "0-";
		
		//Solucion 3
        t1 = clock();
		MSCGreedy msc_greedy(U, F);
        vector<vector<string>> solucion3 = msc_greedy.getMSC();
		t2 = clock();
		datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
		
		//Solucion 4
		cout << endl;
		vector<vector<string>> aux;
		for(int i: posiciones){ 
			aux.push_back(F[i]);
    	}
		vector<string> aux2 = msc_exhaustivo.unionVectores(aux);
		int limite = log(min(U.size()-aux2.size(), (F.size() - posiciones.size())));
		if (limite < 1) limite = 1;
		
		for(int i = 1; i <= limite; i++){
			t1 = clock();
			cout << "Greedy optimizado con K = " << i << endl;
			MSCGreedyOpt msc_greedy_optimizado(U,F,i,posiciones);
			vector<vector<string>> solucion4 = msc_greedy_optimizado.getMSCOpt();
			t2 = clock();
			if (i != limite) datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "-";
			else datos += transformaDecimal(to_string(double(t2-t1)/CLOCKS_PER_SEC)) + "\n";
		}
		
    }
	return datos;
}



//Recorre el archivo .txt formando la Familia y el Universo
map<string,vector<int>> recuperaArchivo(string filename, vector<vector<string>> &F, vector<string> &U){
    ifstream archivo(filename.c_str());
    string linea;
    int cont = 0;
    map<string,vector<int>> conjuntos_claves;
	while (getline(archivo, linea)){
        F.push_back(setOfLine(linea, U, conjuntos_claves, cont));
        cont++;
	}
	return conjuntos_claves;
}

//Funcion que crea un conjunto desde un string, usando como separador un espacio
vector<string> setOfLine(string linea, vector<string> &U, map<string, vector<int>> &conjuntos_claves, int pos){
    string elemento = ""; //Un elemento del conjunto 
    vector<string> conjunto; //conjunto que contendra los elementos de la linea, no repetidos
    for(char i: linea){
        if(i != ' ') elemento.push_back(i);
        else if(i == ' ' && elemento != ""){ 
            if(!existeEnVector(U, elemento)) U.push_back(elemento);
            if(!existeEnVector(conjunto, elemento)) conjunto.push_back(elemento);
            elemento = "";
        }
    }
	if(!existeEnVector(U, elemento)) U.push_back(elemento);
    if(!existeEnVector(conjunto, elemento)) conjunto.push_back(elemento);
    agregaAlMap(conjuntos_claves, conjunto, pos);
    return conjunto;
}

//Agrega los conjuntos que tienen los elementos para revisar en que conjunto hay elementos unicos
void agregaAlMap(map<string, vector<int>> &conjuntos_claves, vector<string> conjunto, int pos){
    for(string s : conjunto){
        auto it = conjuntos_claves.find(s);
        if(it != conjuntos_claves.end()){ // si existe la clave
			if(conjuntos_claves[it->first].size() != 2)
            	conjuntos_claves[it->first].push_back(pos);
        }
		else{ // si no existe la clave en el mapa hay que crearla
			vector<int> v = {pos};
			conjuntos_claves.insert({s,v});
		}
    }
}
//Revisa el map y retorna las posiciones con elementos unicos pertenecientes a la familia
vector<int> conjuntosConElementosUnicos(map<string, vector<int>> conjuntos_claves){
    auto iter = conjuntos_claves.begin(); // iterador en el unicio del map
    vector<int> pos_set_unicos; //vector de posiciones de conjuntos con elementos unicos
	while (iter != conjuntos_claves.end()){ // la key
        if(iter->second.size() == 1){
			if(find(pos_set_unicos.begin(), pos_set_unicos.end(), iter->second[0]) == pos_set_unicos.end()){
				pos_set_unicos.push_back(iter->second[0]);
			}
        }
		iter++;
	}
	return pos_set_unicos;
}



//Genera un MSCP de ejemplo con datos generados aleatoriamente siguiendo una distribucion normal
//dis(mean,sd), ademas un 5% de los conjuntos tendran elementos unicos para que las optimizaciones
//de las soluciones 2 y 4 tengan sentido
map<string,vector<int>> creaEjemploSinArchivo(int m_sets, int max_set, int min_set,double mean, double sd, vector<vector<string>> &family, vector<string> &universe){
	map<string,vector<int>> set_with_unique;
    default_random_engine gen;
    normal_distribution<double> d(mean,sd);
	int actual,salto;
	int cantidad_conjuntos_unicos = family.size() * 0.05;
	if (cantidad_conjuntos_unicos > 0) salto = family.size()/cantidad_conjuntos_unicos;
	else salto = 0;
	if (cantidad_conjuntos_unicos > 0) actual = family.size()/cantidad_conjuntos_unicos;
	else actual = 1;
	vector<int> numeros_unicos;
	int c = 0;

	if (cantidad_conjuntos_unicos > 0){
		for (int i = 0; i < cantidad_conjuntos_unicos; i++){
			int numero_unico = d(gen);
			while (existeEnVector(numeros_unicos,numero_unico)) numero_unico = d(gen);
			numeros_unicos.push_back(numero_unico);
		}
	}
	else{
		int numero_unico = d(gen);
		while (existeEnVector(numeros_unicos,numero_unico)) numero_unico = d(gen);
		numeros_unicos.push_back(numero_unico);
	}
    string linea = "";
    for (int i = 0; i < m_sets; i++){
        int size_conjunto = min_set + rand()%(max_set - min_set);
		if (i == actual-1){
			linea += to_string(numeros_unicos[c]) + " ";
			c++;
			actual += salto;
		}
        for (int j = 0; j < size_conjunto; j++){
            int numero = d(gen);
			while(existeEnVector(numeros_unicos,numero)) numero = d(gen);
            if (j != size_conjunto - 1) linea += to_string(numero) + " ";
            else linea += to_string(numero);
			
        }
        if(i != m_sets - 1){
			family.push_back(setOfLine(linea, universe, set_with_unique,i));
			linea = "";
		}
    }
	return set_with_unique;
}

//Funcion que devuelve un string con el tiempo de ejecucion separado en horas, minutos y segundos
string tiempoEjecucion(unsigned t1, unsigned t2){
	string tiempo = "El programa demoro: ";
	int minutos = 0;
	double segundos = 0;
	int horas = 0;
	if((double(t2-t1)/CLOCKS_PER_SEC) > 3600){
		horas = (double(t2-t1)/CLOCKS_PER_SEC)/3600;
		segundos = ((double(t2-t1)/CLOCKS_PER_SEC) - 3600*horas);
		if (segundos > 60) minutos = (segundos)/60;
		segundos = segundos-60*minutos;
		tiempo += to_string(horas) + " horas " + to_string(minutos) + " minutos " + to_string(segundos) + " segundos\n";
	}
	else if ((double(t2-t1)/CLOCKS_PER_SEC) > 60){
		minutos = (double(t2-t1)/CLOCKS_PER_SEC)/60;
		segundos = double(t2-t1)/CLOCKS_PER_SEC-60*minutos;
		tiempo += to_string(minutos) + " minutos " + to_string(segundos) + " segundos\n";
	}
	else {
		segundos = double(t2-t1)/CLOCKS_PER_SEC;
		tiempo += to_string(segundos) + " segundos\n";
	}
	return tiempo;
}

//Recupera los nombres de los archivos de ejemplo del archivo nombrearchivos.txt
vector<string> recuperaNombresArchivos(string filename){
	ifstream archivo(filename.c_str());
    string linea;
	vector<string> nombres;
    map<string,vector<int>> conjuntos_claves;
	while (getline(archivo, linea)){
        nombres.push_back(linea);
	}
	return nombres;
}

//Transforma un decimal de la forma 0.0123 a 0,0123
//(Para efectos de resultados)
string transformaDecimal(string numero){
    string nuevo;
    for(char i : numero){
        if(i == '.') nuevo += ',';            
        else nuevo += i;
    }
    return nuevo;
}

//Funcion que verifica la existencia de un elemento en un vector(string)
bool existeEnVector(vector<string> vect, string x){
    for(string i: vect){
        if( x == i) return true;
    }
    return false;
}
//Funcion que verifica la existencia de un elemento en un vector(int)
bool existeEnVector(vector<int> vect, int x){
    for(int i: vect){
        if( x == i) return true;
    }
    return false;
}
//Funcion que genera un archivo .txt
void creaArchivo(string texto, string direccion){
	std::fstream file;
	file.open(direccion, std::ios::out);
	if(file.fail()){
		cout << "No se pudo crear el archivo..." << endl;
		return;
	}
	file << texto;
}

//Funcion que imprime un separador
void printSeparador(){
	cout << "-------------------------------------------------------" << endl;
}

void print(vector<int> V){
	for(int i : V) cout << i << " ";
	cout << endl;
}
