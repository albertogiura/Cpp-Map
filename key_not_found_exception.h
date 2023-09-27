#ifndef KEY_NOT_FOUND_EXCEPTION_H
#define KEY_NOT_FOUND_EXCEPTION_H

#include <stdexcept>
#include <string>

/**
	Classe eccezione custom che deriva da std::runtime_error
*/
class keyNotFoundException : public std::runtime_error {
public:
	/**
		Costruttore che prende un messaggio d'errore
	*/
	keyNotFoundException(const std::string &message);
};

#endif