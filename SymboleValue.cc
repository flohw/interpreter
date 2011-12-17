#include "SymboleValue.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
SymboleValue::SymboleValue(Symbole s) :
	Symbole(s.getChaine()) {
	if (s == "<ENTIER>") {
		valeur = atoi(s.getChaine().c_str()); // c_str convertit une string en char*
		defini = true;
	} else if (s == "<CHAINE>") {
		defini = true;
	} else {
		valeur = 0;
		defini = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
void SymboleValue::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Feuille - Symbole value : " << getChaine() << endl; // on affiche la chaine du symbole
}

////////////////////////////////////////////////////////////////////////////////
// Attention : cette fonction (operator) n'est pas membre de la classe SymboleValue
ostream & operator <<(ostream & cout, SymboleValue symb) {
	cout << (Symbole) symb << "\t\t - Valeur=";
	if (symb.defini)
	{
		if (symb == "<CHAINE>")
			cout << symb.getChaine();
		else
			cout << symb.valeur << " ";
	}
	else
		cout << "indefinie ";
	return cout;
}
