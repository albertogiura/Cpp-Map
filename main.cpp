/*
	Progetto C++
	Nome: Alberto
	Cognome: Giura
*/
#include <iostream> // per std::ostream
#include <cassert>
#include <string> // per std::string
#include "map.h"

/**
  @brief Test metodi fondamentali struct pair
*/
void test_pair() {
	Pair<int, int> pair;

	Pair<int, int> pair2(35, 42);
	assert(pair2.key == 35);
	assert(pair2.value == 42);

	Pair<int, int> pair3(pair2);
	assert(pair3.key == 35);
	assert(pair3.value == 42);

	pair = pair2;
	assert(pair.key == 35);
	assert(pair.value == 42);
}

/**
  @brief Funtore di uguaglianza tra tipi interi

  Utilizzato nei confronti tra chiavi.
*/
struct int_equal {
  bool operator()(int a, int b) const {
    return a==b;
  }
};

/**
  @brief Funtore di uguaglianza tra tipi double

  Utilizzato nei confronti tra chiavi.
*/
struct double_equal {
  bool operator()(double a, double b) const {
    return a==b;
  }
};

/**
  @brief Funtore di uguaglianza tra stringhe

  Utilizzato nei confronti tra chiavi.
*/
struct str_equal {
  bool operator()(const std::string &a, const std::string &b) const {
    return a==b;
  }
};

// Typedef per velocizzare test di una mappa con chiavi e valori di tipo
// intero e relativo funtore per i confronti di uguaglianza
typedef Map<int, int, int_equal> mapint;

/**
  @brief Test dei metodi fondamentali della classe Map

  Test dei metodi fondamentali su una classe Map con chiavi e valori
  aventi tipo di dato primitivo.
*/
void test_metodi_fondamentali_primitivi() {
	std::cout << "----------- Inizio test sui metodi fondamentali con tipi primitivi -----------" << std::endl;
	std::cout << "----------- Mappa Map<int, int, int_equal>  -----------" << std::endl;
	// Test costruttore di default con chiave-valore-funtore int
	// utilizzando typedef
	mapint map1;

	map1.add(5, 12);
	map1.add(4, 7);
	map1.add(32, 84);

	std::cout << "Stampa di controllo inserimenti in map1:" << std::endl;
	// Test operatore<< implementato a fini di debug
	std::cout << map1 << std::endl;

	// Test copy constructor
	Map<int, int, int_equal> map2(map1);

	std::cout << "Stampa di controllo di map2 creato con copy constructor:" << std::endl;
	std::cout << map2 << std::endl;

	// Test assegnamento - operatore=
	mapint map3 = map1;
	std::cout << "Stampa di controllo di map3 dopo assegnamento:" << std::endl;
	std::cout << map3 << std::endl;

	std::cout << "----------- Mappa Map<double, int, double_equal>  -----------" << std::endl;
	Map<double, int, double_equal> map4;

	map4.add(8.7, 22);
	map4.add(9.3, 67);
	map4.add(42.0, 15);

	std::cout << "Stampa di controllo inserimenti in map4:" << std::endl;
	// Test operatore<< implementato a fini di debug
	std::cout << map4 << std::endl;

	// Test copy constructor
	Map<double, int, double_equal> map5(map4);

	std::cout << "Stampa di controllo di map5 creato con copy constructor:" << std::endl;
	std::cout << map5 << std::endl;

	// Test assegnamento - operatore=
	Map<double, int, double_equal> map6 = map4;
	std::cout << "Stampa di controllo di map6 dopo assegnamento:" << std::endl;
	std::cout << map6 << std::endl;

	std::cout << "----------- Fine test sui metodi fondamentali con tipi primitivi -----------" << std::endl;
}

/**
  @brief Test dei metodi dell'interfaccia pubblica della classe Map

  Test dei metodi pubblici su una classe Map con chiavi e valori
  aventi tipo di dato primitivo.
*/
void test_metodi_interfaccia_pubblica() {
	std::cout << "----------- Inizio test sui metodi dell'interfaccia pubblica -----------" << std::endl;
	mapint map1;

	map1.add(8, 10);
	map1.add(35, 42);
	map1.add(99, 82);
	map1.add(2, 3);
	map1.add(26, 9);

	std::cout << "Stampa di controllo inserimenti in map1:" << std::endl;
	// Test operatore<< implementato a fini di debug
	std::cout << map1 << std::endl;

	std::cout << "Numero di coppie presenti in map1:" << std::endl;
	std::cout << map1.size() << std::endl;
	// Utilizzo l'asserzione per verificare il test (post-condizione)
	assert(map1.size() == 5);

	std::cout << "Stampa delle chiavi presenti in map1:" << std::endl;
	std::vector<int> v = map1.keys();
	
	// Stampa le chiavi contenute nel vettore v
    for (int currkey : v) {
        std::cout << currkey << " ";
    }
    std::cout << std::endl;

	assert(map1.exists(35) == true);
	std::cout << "Il valore associato alla chiave avente numero 35 e': " << map1.value(35) << std::endl;
	assert(map1.value(35) == 42);

	std::cout << "Rimuovo la coppia avente per chiave 99 e ristampo le coppie:" << std::endl;
	map1.remove(99);
	std::cout << map1 << std::endl;

	std::cout << "Rimuovo le coppie aventi per chiavi 26 e 8." << std::endl;
	map1.remove(26);
	map1.remove(8);
	assert(map1.exists(26) == false);
	assert(map1.exists(8) == false);
	
	// Utilizzo dei const_iterator per accedere in lettura ai dati della mappa e stamparli
	std::cout << "Stampa delle coppie della mappa con iteratori:" << std::endl;
	mapint::const_iterator b, e;
	int count = 1;

	for (b = map1.begin(), e = map1.end(); b != e; ++b) {
		std::cout << count << ") Chiave: " << (*b).key << " Valore: " << (*b).value << std::endl;
		count++;
	}
	
	map1.clear();
	std::cout << "Dimensione della mappa aggiornata dopo la clear(): " << map1.size() << std::endl;
	assert(map1.size() == 0);
	std::cout << "----------- Fine test sui metodi dell'interfaccia pubblica -----------" << std::endl;
}

/**
  @brief Test dei metodi dell'interfaccia pubblica della classe Map

  Test dei metodi pubblici su una classe Map con chiavi aventi come tipo
  di dato le stringhe e i valori gli interi.
*/
void test_map_string() {
	std::cout << "----------- Inizio test su mappa Map<std::string, int, str_equal> -----------" << std::endl;
	Map<std::string, int, str_equal> mapstr;
	mapstr.add("prova", 88);
	mapstr.add("progetto", 94);
	mapstr.add("anno", 2023);

	std::cout << "Numero di coppie presenti in mapstr:" << std::endl;
	std::cout << mapstr.size() << std::endl;
	assert(mapstr.size() == 3);

	std::cout << "Stampa delle coppie presenti in mapstr:" << std::endl;
	// Test operatore<< implementato a fini di debug
	std::cout << mapstr << std::endl;

	std::cout << "Verifica della presenza della chiave 'progetto' (0-false, 1-true): " << mapstr.exists("progetto") << std::endl;
	assert(mapstr.exists("progetto") == true);

	mapstr.remove("prova");
	assert(mapstr.size() == 2);

	std::cout << "Stampa delle chiavi presenti in mapstr dopo rimozione di 'prova':" << std::endl;
	std::vector<std::string> v = mapstr.keys();
	
	// Stampa le chiavi contenute nel vettore v
    for (std::string currkey : v) {
        std::cout << currkey << " ";
    }
    std::cout << std::endl;

	mapstr.clear();
	assert(mapstr.size() == 0);

	std::cout << "----------- Fine test su mappa Map<std::string, int, str_equal> -----------" << std::endl;
}

/**
  @brief Struct che implementa un oggetto custom a scopo di test
*/
struct custom_obj {
	int first;
	int second;

	custom_obj() : first(0), second(0) {}

	custom_obj(int par1, int par2) : first(par1), second(par2) {}
};

/**
  Implementazione dell'operatore di stream per la struct custom_obj
  a scopo di debug
*/
std::ostream &operator<<(std::ostream &os, const custom_obj &ob) {
	//os << "Dato custom con first: " << ob.first << " e second: " << ob.second << std::endl;
	os << "(first: " << ob.first << " , second: " << ob.second << ")";
	return os;
}

/**
  @brief Funtore di uguaglianza tra tipi custom_obj

  Utilizzato nei confronti tra chiavi.
*/
struct custom_obj_equal {
  bool operator()(const custom_obj &ob1, const custom_obj &ob2) const {
    return (ob1.first == ob2.first) && (ob1.second == ob2.second);
  }
};

/**
  @brief Test dei metodi dell'interfaccia pubblica della classe Map

  Test dei metodi pubblici su una classe Map con chiavi aventi come tipo
  di dato custom_obj e i valori gli interi.
*/
void test_map_custom() {
	std::cout << "----------- Inizio test su mappa con chiave di tipo custom - Map<custom_obj, int, custom_obj_equal> -----------" << std::endl;
	Map<custom_obj, int, custom_obj_equal> cusmap;

	cusmap.add(custom_obj(4, 8), 55);
	cusmap.add(custom_obj(23, 5), 35);
	cusmap.add(custom_obj(1, 0), 4);
	cusmap.add(custom_obj(18, 2), 9);

	std::cout << "Numero di coppie presenti in cusmap:" << std::endl;
	std::cout << cusmap.size() << std::endl;
	assert(cusmap.size() == 4);

	std::cout << "Stampa delle coppie presenti in cusmap:" << std::endl;
	// Test operatore<< implementato a fini di debug
	std::cout << cusmap << std::endl;

	assert(cusmap.exists(custom_obj(23, 5)) == true);

	cusmap.remove(custom_obj(1, 0));
	assert(cusmap.size() == 3);

	std::cout << "Stampa delle chiavi presenti in cusmap dopo la rimozione della coppia avente come chiave (1,0):" << std::endl;
	std::vector<custom_obj> v = cusmap.keys();
	
	// Stampa le chiavi contenute nel vettore v
    for (custom_obj currkey : v) {
        std::cout << currkey << " ";
    }
    std::cout << std::endl;

	assert(cusmap.value(custom_obj(18, 2)) == 9);

	cusmap.clear();
	assert(cusmap.size() == 0);

	std::cout << "----------- Fine test su mappa con chiave di tipo custom -----------" << std::endl;
}

/**
  @brief Test metodi interfaccia Map con mappa non constante 
  passata come parametro

  @param maptest mappa usata per i test
*/
void test_mapint_parameter(mapint &maptest) {
	std::cout << "----------- Inizio test su mappa con coppie di interi passata come parametro non costante -----------" << std::endl;
	maptest.add(15, 17);
	maptest.add(8, 4);
	maptest.add(22, 56);
	maptest.add(99, 1);

	std::cout << "Stampa di controllo inserimenti in maptest:" << std::endl;
	std::cout << maptest << std::endl;

	maptest.remove(22);
	assert(maptest.exists(22) == false);

	std::cout << "Stampa delle coppie della mappa con iteratori:" << std::endl;
	mapint::const_iterator b, e;
	int count = 1;

	for (b = maptest.begin(), e = maptest.end(); b != e; ++b) {
		std::cout << count << ") Chiave: " << (*b).key << " Valore: " << (*b).value << std::endl;
		count++;
	}
	
	maptest.clear();
	std::cout << "Dimensione della mappa aggiornata dopo la clear(): " << maptest.size() << std::endl;
	assert(maptest.size() == 0);
	std::cout << "----------- Fine test su mappa con coppie di interi passata come parametro non costante -----------" << std::endl;
}

/**
  @brief Test metodi interfaccia Map con mappa constante 
  passata come parametro

  @param maptest mappa usata per i test
*/
void test_mapint_parameter_const(const mapint &maptest) {
	// Non è possibile chiamare operazioni che alterano lo stato della classe su oggetti const
	// Quindi non è possibile chiamare metodi come add, remove, clear

	std::cout << "----------- Inizio test su mappa con coppie di interi passata come parametro costante -----------" << std::endl;

	std::cout << "Stampa di controllo di maptest (mappa deve essere vuota):" << std::endl;
	std::cout << maptest << std::endl;

	std::cout << "Stampa delle coppie della mappa con iteratori (non devono esserci coppie):" << std::endl;
	mapint::const_iterator b, e;
	int count = 1;

	for (b = maptest.begin(), e = maptest.end(); b != e; ++b) {
		std::cout << count << ") Chiave: " << (*b).key << " Valore: " << (*b).value << std::endl;
		count++;
	}
	
	std::cout << "Numero di coppie presenti nella mappa: " << maptest.size() << std::endl;
	std::cout << "Il risultato atteso qui è non avere coppie, dal momento che non era possibile aggiungere pair alla mappa dal principio." << std::endl;
	std::cout << "----------- Fine test su mappa con coppie di interi passata come parametro costante -----------" << std::endl;
}

/**
  @brief Test tentativo di aggiunta chiave già presente nella mappa

  @throw keyAlreadyDefinedException
*/
void test_eccezione_chiave_presente() {
	mapint map1;	

	try {
			map1.add(5, 12);
			map1.add(87, 93);
			map1.add(5, 15);
	} catch(keyAlreadyDefinedException &e) {
			//std::cout << e.what() << std::endl;
			throw;
	} catch(...) {
			throw;
	}
}

/**
  @brief Test tentativo di rimozione chiave non presente nella mappa

  @throw keyNotFoundException
*/
void test_eccezione_rimozione_chiave_non_presente() {
	mapint map1;

	try {
			map1.add(5, 12);
			map1.add(87, 93);
			map1.remove(2);
	} catch(keyNotFoundException &e) {
			//std::cout << e.what() << std::endl;
			throw;
	} catch(...) {
			throw;
	}
}

int main() {

	test_metodi_fondamentali_primitivi();

	test_map_string();

	test_map_custom();

	test_metodi_interfaccia_pubblica();

	mapint maptest;

	test_mapint_parameter(maptest);

	test_mapint_parameter_const(maptest);

	//test_eccezione_chiave_presente();

	//test_eccezione_rimozione_chiave_non_presente();

	//test_pair();

	return 0;
}