#include <ctype.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "Symbole.h"

////////////////////////////////////////////////////////////////////////////////

Symbole::Symbole(string s) {
	this->chaine = s;
	// attention : l'ordre des tests ci-dessous n'est pas innocent !
	if (s == "") this->categorie = FINDEFICHIER;
	else if (isdigit(s[0])) this->categorie = ENTIER;
	else if (isMotCle(s)) this->categorie = MOTCLE;
	else if (isalpha(s[0])) this->categorie = VARIABLE;
	else if (s[0] == '"') this->categorie = CHAINE;
	else this->categorie = INDEFINI;
}

////////////////////////////////////////////////////////////////////////////////

bool Symbole::operator ==(string ch) {
	return this->chaine == ch ||
			(this->categorie == VARIABLE && ch == "<VARIABLE>") ||
			(this->categorie == ENTIER && ch == "<ENTIER>") ||
			(this->categorie == INDEFINI && ch == "<INDEFINI>") ||
			(this->categorie == MOTCLE && ch == "<MOTCLE>") ||
			(this->categorie == CHAINE && ch == "<CHAINE>") ||
			(this->categorie == FINDEFICHIER && ch == "<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////

bool Symbole::isMotCle(string s) {
	static vector <string> motsCles; // vecteur pour stocker les mots clés du langage
	// si on n'a pas encore chargé les mots clés dans le vecteur, on le fait !
	if (!motsCles.size()) {
		ifstream fichier(FICHIER_MOTS_CLE, ifstream::in);
		while (!fichier.eof()) {
			string mot;
			getline(fichier, mot);
			if (mot != "") { // insertion triée de s dans le vecteur des mots clés
				vector<string>::iterator it = motsCles.begin();
				while (it < motsCles.end() && *it < mot) it++;
				if (it == motsCles.end() || *it != mot) // si pas trouvé...
					motsCles.insert(it, mot);
			}
		}
		fichier.close();
	}
	// on recherche  s dans le vecteur des mots clés triés
	unsigned int i;
	for (i = 0; i < motsCles.size() && motsCles[i] < s; i++);
	return (i < motsCles.size() && motsCles[i] == s);
}

////////////////////////////////////////////////////////////////////////////////
// Attention : cette fonction (operator << ) n'est pas membre de la classe Symbole

ostream & operator <<(ostream & cout, Symbole symb) {
	cout << "Symbole de type ";
	if (symb.categorie == Symbole::MOTCLE) cout << "<MOTCLE>      ";
	else if (symb.categorie == Symbole::VARIABLE) cout << "<VARIABLE>    ";
	else if (symb.categorie == Symbole::ENTIER) cout << "<ENTIER>      ";
	else if (symb.categorie == Symbole::INDEFINI) cout << "<INDEFINI>    ";
	else if (symb.categorie == Symbole::CHAINE) cout << "<CHAINE>      ";
	else if (symb.categorie == Symbole::FINDEFICHIER) cout << "<FINDEFICHIER>";
	cout << " : \"" << symb.chaine << "\"";
	return cout;
}