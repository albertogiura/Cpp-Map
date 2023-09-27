#ifndef KEY_ALREADY_DEFINED_EXCEPTION_H
#define KEY_ALREADY_DEFINED_EXCEPTION_H

#include <stdexcept>
#include <string>

/**
	Classe eccezione custom che deriva da std::runtime_error
*/
class keyAlreadyDefinedException : public std::runtime_error {
public:
	/**
		Costruttore che prende un messaggio d'errore
	*/
	keyAlreadyDefinedException(const std::string &message);
};

#endif