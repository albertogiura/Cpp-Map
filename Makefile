main.exe: main.o key_not_found_exception.o key_already_defined_exception.o
	g++ main.o key_not_found_exception.o key_already_defined_exception.o -o main.exe

main.o: main.cpp map.h
	g++ -c main.cpp -o main.o

key_not_found_exception.o: key_not_found_exception.cpp key_not_found_exception.h
	g++ -c key_not_found_exception.cpp -o key_not_found_exception.o

key_already_defined_exception.o: key_already_defined_exception.cpp key_already_defined_exception.h
	g++ -c key_already_defined_exception.cpp -o key_already_defined_exception.o

.PHONY: clean
clean: 
	rm -r *.o *.exe