#include <iostream>
#include <exception>

using namespace std;

class Erreur : public exception {
public:

	virtual const char* what() const throw () {
		return "Il y a une erreur inconnue dans votre programme !";
	}

};

class MauvaisSymbole : public exception {
private:
	string symbole;
public:

	MauvaisSymbole(string sym) {
		this->symbole = sym;
	};

	virtual ~MauvaisSymbole() throw () {
	};

	virtual const char* what() const throw () {
		return this->symbole.c_str();
	}

};
