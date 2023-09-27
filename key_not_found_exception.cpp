#include "key_not_found_exception.h"

keyNotFoundException::keyNotFoundException(const std::string &message) 
	: std::runtime_error(message) {
		// il messaggio è forwardato alla classe std::runtime_error 
		// che a sua volta lo passerà alla classe std::exception
	}