#ifndef TABLESYMBOLES_H_
#define TABLESYMBOLES_H_

#include "SymboleValue.h"
#include <vector>
#include <iostream>
using namespace std;

class TableSymboles {
public:
    TableSymboles(); // Construit une table vide de pointeurs sur des symboles valués
    SymboleValue * chercheAjoute (Symbole s);
      // si s est identique à un symbole valué déjà présent dans la table,
      // on renvoie un pointeur sur ce symbole valué
      // Sinon on insère un nouveau symbole valué correspondant à s
      // et on renvoie un pointeur sur le nouveau symbole valué inséré
	inline unsigned int size() { return this->table.size(); }
	unsigned int getFirstVariable();	// Retourne l'indice de table ou se trouve le premiere symbole de categorie VARIABLE
	unsigned int getNbVariables();		// retourne le nombre de symbole dont la categorie est VARIABLE
	string getNomVariable(unsigned int i); // Retourne la chaine du symbole en table[i]

    friend ostream & operator << (ostream & cout, TableSymboles ts); // affiche ts sur cout

private:
    vector<SymboleValue *> table; // la table des symboles valués, triée sur la chaine
};

#endif /* TABLESYMBOLES_H_ */
