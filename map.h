/*
	Progetto C++
	Nome: Alberto
	Cognome: Giura
*/
#ifndef MAP_H
#define MAP_H

#include <algorithm> // per std::swap
#include <vector> // per std::vector
#include <ostream> // per std::ostream
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t
#include <cassert> // assert
#include "key_not_found_exception.h" // eccezione custom per remove e value
#include "key_already_defined_exception.h" // eccezione custom per add

/**
	@brief Struct Pair

	Struttura dati esterna alla mappa che memorizza coppie della forma
	<chiave, valore> templata sui tipi generici C e V.
*/
template <typename C, typename V>
struct Pair {
	C key; // chiave
	V value; // valore

	/**
    	Costruttore di default
    */
	Pair() {}

	/**
		Costruttore secondario

		@param key coppia da copiare
		@param val puntatore al prossimo nodo

		@post key = key
		@post value == val
    */
	Pair(C key, V val) : key(key), value(val) {}

	/**
		Copy constructor

		@param other pair da copiare
		@post key = other.key
		@post value = other.value
  	*/
	Pair(const Pair<C, V> &other) : key(other.key), value(other.value) {}

	/**
		Operatore di assegnamento 

		@param other pair da copiare
		@return reference al pair this
    */
	Pair& operator=(const Pair<C, V> &other) {
		key = other.key;
		value = other.value;
		return *this;
	}

	/**
		Distruttore 
	*/
	~Pair() {}

}; // struct pair

/**
  @brief Classe Map

  La classe implementa una struttura dati associativa Map
  memorizzando una coppia di dati <chiave, valore> di tipo generico
  rispettivamente C e V.
  La classe si aspetta di ricevere dall'utente anche un funtore
  per poter definire come viene controllata l'uguaglianza di due chiavi,
  non essendo noto a priori come è fatto un dato di tipo C.

*/
template <typename C, typename V, typename Eq>
class Map {

	/**
		@brief Struct Node

		Struttura dati interna alla mappa che modella il concetto di
		nodo, utile per la creazione di una lista da utilizzare per 
		salvare le coppie.
  	*/
	struct Node {
		Pair<C, V> item; // Coppia <chiave, valore> da memorizzare
		Node *next; // Puntatore al nodo successivo della lista

		/**
      		Costruttore di default
      		@post next == nullptr
    	*/
		Node() : next(nullptr) {};

		/**
			Costruttore secondario

			@param p coppia da copiare
			@param n puntatore al prossimo nodo

			@post item == p
			@post next == n
    	*/
		Node(const Pair<C, V> &p, Node *n) : item(p), next(n) {}

		/**
			Costruttore secondario

			@param p coppia da copiare

			@post item == p
			@post next == nullptr
    	*/
		explicit Node(const Pair<C, V> &p) : item(p), next(nullptr) {}

		// Non inserisco il costruttore che prende solo next come parametro

		/**
      		Copy constructor

      		@param other nodo da copiare
    	*/
    	Node(const Node &other) : item(other.item), next(other.next) {}

		/**
			Operatore di assegnamento 

			@param other nodo da copiare
			@return reference al nodo this
    	*/
		Node& operator=(const Node &other) {
			item = other.item;
			next = other.next;
			return *this;
    	}

		/**
			Distruttore 
		*/
		~Node() {}

	}; // struct node

	Node *_head; // Puntatore al primo nodo della lista interna
	unsigned int _size; // Numero di nodi della lista e, quindi, di coppie
	Eq _fequal; // Funtore per l'uguaglianza tra chiavi di tipo generico C

public:

	/**
    	Costruttore di default

		@post _head == nullptr
		@post _size == 0
  	*/
	Map() : _head(nullptr), _size(0) {}

	/**
		Copy constructor

		@param other mappa da copiare
		@throw se l'allocazione delle risorse fallisce lancia un'eccezione
		@post _size = other._size
  	*/
	Map(const Map &other) : _head(nullptr), _size(0) {
		Node *current = other._head;

		// La mappa viene riempita ciclando sui nodi di other,
		// aggiungendoli uno alla volta. Il blocco di codice
		// viene racchiuso all'interno di un try-catch in quanto
		// la add potrebbe generare un'eccezione se
		// fallisce l'allocazione delle risorse
		try {
			while (current != nullptr) {
				add(current->item.key, current->item.value);
				current = current->next;
			}
		} catch(...) {
			clear(); // recovery degli errori
			throw; // rilancio l'eccezione
		}
	}

	/**
		Operatore di assegnamento

		@param other mappa da copiare

		@return reference alla mappa this

		@post _size = other._size
  	*/
	Map& operator=(const Map &other) {
		if (this != &other) {
			Map temp(other);
			std::swap(this->_head, temp._head);
			std::swap(this->_size, temp._size);
		}
		return *this;
	}

	/**
		Distruttore
		
		All'uscita di scope chiama una funzione dedicata della classe
		per la deallocazione delle risorse dinamicamente allocate in 
		precedenza.

		@post _head == nullptr
		@post _size == 0
  	*/
	~Map() { clear(); }

	/**
		@brief Metodo "getter" del numero di elementi nella mappa.

		Funzione per restituire la dimensione della lista di nodi,
		ovvero il numero di coppie memorizzate nella mappa.

		@return numero di coppie presenti nella mappa
  	*/
	unsigned int size() const {
    	return _size;
  	}

	/**
		@brief Funzione per svuotare la struttura dati.

		@post _head == nullptr
		@post _size == 0
  	*/
	void clear() {
		Node *current = _head;

		// Cicla su tutti i nodi a partire dal primo e ne
		// chiama la delete, invocando di conseguenza il distruttore
		// dei nodi e deallocando le risorse allocate in precedenza con
		// l'aggiunta delle coppie nella mappa
		while (current != nullptr) {
			Node *cnext = current->next;
			delete current;
			current = cnext;
		}

		_head = nullptr;
		_size = 0;
	}

	/**
		@brief Funzione che aggiunge una coppia alla mappa.

		La funzione crea una coppia con i parametri passati ed un nodo
		in cui memorizzarla. Il nodo viene poi inserito nella lista.
		Poiché non è previsto un ordinamento particolare per le coppie,
		ad esempio secondo la loro chiave, gli inserimenti avvengono
		sempre in testa.

		@param k chiave della coppia
		@param v valore della coppia

		@post _size = _size + 1

		@throw se l'allocazione delle risorse fallisce o l'utente
		tenta di aggiungere alla mappa una coppia avente per chiave
		chiave già definita in precedenza lancia un'eccezione (nella
		seconda eventualità viene lanciata l'eccezione custom 
		keyAlreadyDefinedException)
  	*/
	void add(const C &k, const V &v) {
		// Non racchiudo in un blocco try-catch perché
		// se l'allocazione di risorse fallisce non c'è
		// possibilità di gestire diversamente l'errore
		Node *temp = new Node(Pair<C, V>(k, v));

		// Controllo caso lista vuota
		// Se la lista è vuota, la chiave non è già presente
		// e il nodo può essere aggiunto in testa
		if (_size == 0) {
			_head = temp;
			_size = 1;
			return;
		}
		
		// Se la lista non è vuota, controllo se la chiave
		// è già presente. Se non è presente, inserisco in testa
		if (exists(k)){
			throw keyAlreadyDefinedException("Chiave già presente nella mappa.");
		} else {
			Node *current = _head;
			_head = temp;
			temp->next = current;
			_size++;
			return;
		}
		
	}

	/**
		@brief Verifica l'esistenza di una coppia nella mappa.
		
		Funzione per verificare se una coppia con una certa chiave
		è presente nella mappa.

		@param key chiave della coppia
		@return true se la coppia è presente, false altrimenti
  	*/
	bool exists(const C &key) const {
		Node *current = _head;

		// Ciclo su tutti i nodi della lista per verificare,
		// con l'ausilio del funtore di uguaglianza tra tipi C,
		// se è presente nella mappa una coppia con la chiave 
		// passata come parametro
		while (current != nullptr) {
			if (_fequal(key, current->item.key))
				return true;
			current = current->next;
		}
		return false;
	}

	/**
		@brief Funzione che rimuove una coppia dalla mappa.

		La funzione ricerca la coppia di cui si chiede la rimozione
		sulla base della chiave che riceve come parametro. Esegue un
		controllo preventivo sul fatto che la mappa non sia vuota ed 
		esista una coppia con la chiave passata.

		@param key chiave della coppia
		@throw se la mappa non ha coppie al suo interno o non esiste
		una coppia con la chiave passata come parametro lancia 
		un'eccezione custom keyNotFoundException.
  	*/
	void remove(const C &key) {
		if (_size == 0 || !exists(key)) { 
			throw keyNotFoundException("Chiave non trovata nella mappa."); 
		}
		
		Node *current = _head;
		Node *previous = _head;

		while(current != nullptr) {
			if (_fequal(key, current->item.key)) {
				
				// Caso di rimozione in testa
				if (current == _head) {
					_head = current->next;
				}

				// Passi di rimozione comuni
				previous->next = current->next;
				delete current;
				current = nullptr;
				_size--;

			} else {
				previous = current;
				current = current->next;
			}
		}
	}

	/**
		@brief Restituisce il valore associato ad una chiave

		Funzione che restituisce il valore associato ad 
		una certa coppia avente come chiave quella passata come 
		parametro.

		@param key chiave della coppia
		@return il valore di tipo V associato alla relativa chiave
		@throw lancia un'eccezione keyNotFoundException se la chiave
		passata non è presente nella mappa
  	*/
	const V& value(const C &key) const {

		// Lancia una eccezione nel caso non vi sia la chiave 
		// e quindi la coppia dalla quale recuperare il valore
		if (!exists(key)) { 
			throw keyNotFoundException("Chiave non trovata nella mappa."); 
		}

		Node *current = _head;

		while (current != nullptr) {
			if (_fequal(key, current->item.key))
				return current->item.value;
			current = current->next;
		}
	}
	
	/**
		Funzione globale che implementa l'operatore di stream
		(non richiesto esplicitamente, implementato per debug).
		Essendo all'interno della classe Map è anch'essa templata
		sugli stessi tipi generici. Inoltre, la keyword friend 
		si rende necessaria per consentire alla funzione l'accesso
		ai dati privati di Map.

		@param os stream di output
		@param map Map da spedire sullo stream

		@return lo stream di output
  	*/
	friend std::ostream &operator<<(std::ostream &os, const Map &map) {
		Node *current = map._head;
		int count = 1;

		while(current != nullptr) {
			os << count << ")" << std::endl;
			os << "Chiave: " << current->item.key << std::endl;
			os << "Valore: " << current->item.value << std::endl;
			current = current->next;
			count++;
		}
		
		return os;
  	}
	
	/**
		@brief Restituisce un vettore con le chiavi della mappa.

		Funzione che restituisce un vettore contenente le chiavi
		presenti all'interno della mappa.

		@return std::vector<C>
  	*/
	std::vector<C> keys() const {
		// La funzione sfrutta gli iteratori per scorrere la 
		// struttura dati
		const_iterator b, e;
		std::vector<C> v;

		for (b = begin(), e = end(); b != e; ++b) {
			v.push_back((*b).key);
		}
		
		return v;
	}

	/**
		La classe Map deve supportare l'accesso ai dati tramite 
		iteratori forward costanti. Gli iteratori iterano sulle 
		coppie, quindi concretamente sugli oggetti Pair<C, V> 
		che vengono inseriti nella mappa e non sui nodi, che 
		sono utilizzati solo internamente alla mappa per 
		realizzare un modello di lista linkata singola.
  	*/
	class const_iterator {
		//	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef Pair<C, V>                value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const Pair<C, V>*         pointer;
		typedef const Pair<C, V>&         reference;

	
		const_iterator() : nptr(nullptr) {}
		
		const_iterator(const const_iterator &other) : nptr(other.nptr) {}

		const_iterator& operator=(const const_iterator &other) {
			nptr = other.nptr;
			return *this;
		}

		~const_iterator() {}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return nptr->item;
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(nptr->item);
		}
		
		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator temp(*this);
			nptr = nptr->next;
			return temp;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			nptr = nptr->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (nptr == other.nptr);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (nptr != other.nptr);
		}

	private:
		//Dati membro

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class Map;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(const Node *n) : nptr(n) { }

		const Node *nptr;
		
	}; // classe const_iterator
	
	// Ritorna l'iteratore all'inizio della sequenza dati
	const_iterator begin() const {
		return const_iterator(_head);
	}
	
	// Ritorna l'iteratore alla fine della sequenza dati
	const_iterator end() const {
		return const_iterator(nullptr);
	}

}; // classe map

#endif