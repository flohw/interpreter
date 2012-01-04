#include "LecteurSymbole.h"
#include <ctype.h>
#include <string.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////

LecteurSymbole::LecteurSymbole(string nomFich) :
lc(nomFich), symCour("") {
	suivant(); // pour lire le premier symbole
}

////////////////////////////////////////////////////////////////////////////////

void LecteurSymbole::suivant() {
	sauterSeparateurs();
	while (lc.getCarCour() == '#')
		sauterCommentaires();
	// on est maintenant positionne sur le premier caractère d'un symbole
	ligne = lc.getLigne();
	colonne = lc.getColonne();
	symCour = Symbole(motSuivant()); // on reconstruit symCour avec le nouveau mot lu
}

////////////////////////////////////////////////////////////////////////////////

void LecteurSymbole::sauterSeparateurs() {
	while (lc.getCarCour() == ' ' ||
			lc.getCarCour() == '\t' ||
			lc.getCarCour() == '\r' ||
			lc.getCarCour() == '\n')
		lc.suivant();
}

void LecteurSymbole::sauterCommentaires() {
	if (lc.getCarCour() == '#') {
		while (lc.getCarCour() != '\n' and lc.getCarCour() != '\r') {
			lc.suivant();
		}
	}
	sauterSeparateurs();
}

////////////////////////////////////////////////////////////////////////////////

string LecteurSymbole::motSuivant() {
	static string s;
	s = "";
	if (isdigit(lc.getCarCour())) {
		// c'est le début d'un entier
		do {
			s = s + lc.getCarCour();
			lc.suivant();
		} while (isdigit(lc.getCarCour()));
	} else if (lc.getCarCour() == '"') {
		do {
			s += lc.getCarCour();
			lc.suivant();
		} while (lc.getCarCour() != '"');
		s += lc.getCarCour();
		lc.suivant();
	} else if (lc.getCarCour() == '=' || lc.getCarCour() == '!' || lc.getCarCour() == '<' || lc.getCarCour() == '>') {
		do {
			s += lc.getCarCour();
			lc.suivant();
		} while (lc.getCarCour() == '=');
	} else if (isalpha(lc.getCarCour())) {
		// c'est le début d'un mot
		do {
			s = s + lc.getCarCour();
			lc.suivant();
		} while (isalpha(lc.getCarCour()) ||
				isdigit(lc.getCarCour()) ||
				lc.getCarCour() == '_');
	} else if (lc.getCarCour() == '+' || lc.getCarCour() == '-' || lc.getCarCour() == '*' || lc.getCarCour() == '/') {
		do {
			s += lc.getCarCour();
			lc.suivant();
		} while (lc.getCarCour() == '+' || lc.getCarCour() == '-' || lc.getCarCour() == '=');
	} else if (lc.getCarCour() != EOF)
		// c'est un caractere special
	{
		s = lc.getCarCour();
		lc.suivant();
	}
	return s;
}
