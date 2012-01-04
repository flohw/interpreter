// Contient toutes les spécifications de classes nécessaires
// pour représenter l'arbre abstrait
#ifndef ARBRE_H_
#define ARBRE_H_

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"

////////////////////////////////////////////////////////////////////////////////

class Noeud {
	// classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
	// Remarque : la classe ne contient aucun constructeur
public:
	virtual int getValeur() = 0; // méthode pure (non implémentée) qui rend la classe abstraite

	virtual void afficher(unsigned short indentation = 0) {
		cout << setw(4 * indentation) << " ";
	}

	virtual void translateAda(ostream& s, unsigned short indentation = 0) {
		s << setw(4 * indentation) << "";
	}
	// pour afficher un noeud avec un décalage (indentation) proportionnel à son niveau dans l'arbre

	virtual void translatePhp(ostream &s, unsigned short indentation = 0) {
		s << setw(4 * indentation) << "";
	}

	virtual ~Noeud() {
	} // présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////

class NoeudSeqInst : public Noeud {
	// classe pour représenter un noeud "sequence d'instruction"
	// qui a autant de fils que d'instructions dans la séquence
public:
	NoeudSeqInst(); // construit une séquence d'instruction vide

	~NoeudSeqInst() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation = 0); // affiche la séquence d'instructions
	void ajouteInstruction(Noeud* instruction); // ajoute une instruction à la séquence
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	vector<Noeud *> tabInst; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////

class NoeudAffectation : public Noeud {
	// classe pour représenter un noeud "affectation"
	// composé de 2 fils : la variable et l'expression qu'on lui affecte
public:
	NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation

	~NoeudAffectation() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue l'expression et affecte sa valeur à la variable
	void afficher(unsigned short indentation = 0); // affiche l'affectation
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Noeud* variable;
	Noeud* expression;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudOperateurBinaire : public Noeud {
	// classe pour représenter un noeud "opération arithmétique" composé d'un opérateur (+-*/)
	// et de 2 fils : l'opérande gauche et l'opérande droit
public:
	NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
	// construit une opération binaire : operandeGauche operateur OperandeDroit

	~NoeudOperateurBinaire() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue l'operande gauche, l'operande droit et applique l'opérateur
	void afficher(unsigned short indentation = 0); // affiche l'opération
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Symbole operateur;
	Noeud* operandeGauche;
	Noeud* operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudEcrire : public Noeud {
public:
	NoeudEcrire(Noeud* val); // construit une chaine ou une expression

	~NoeudEcrire() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // retourne le symbole (chaine ou expression)
	void afficher(unsigned short indentation = 0); // affiche l'opération
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

protected:
	Noeud* valeur;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudInstBoucle : public Noeud {
public:
	NoeudInstBoucle(Noeud* exp, Noeud* seq, std::string t, Noeud* aff = NULL, Noeud* incr = NULL);

	~NoeudInstBoucle() {
	}
	int getValeur(); // retourne le symbole (chaine ou expression)
	void afficher(unsigned short indentation = 0); // affiche l'opération
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Noeud* expression;
	Noeud* sequence;
	std::string type;
	Noeud* affectation;
	Noeud* incrementation;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudInstSi : public Noeud {
public:
	NoeudInstSi(vector<Noeud*> exp, vector<Noeud*> seq);

	~NoeudInstSi() {
	}
	int getValeur(); // retourne le symbole (chaine ou expression)
	void afficher(unsigned short indentation = 0); // affiche l'opération
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	vector<Noeud*> expression;
	vector<Noeud*> sequence;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudLire : public Noeud {
public:
	NoeudLire(Noeud* var);

	~NoeudLire() {
	};
	int getValeur();
	void afficher(unsigned short indentation = 0);
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Noeud* variable;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudSwitch : public Noeud {
public:
	NoeudSwitch(vector<Noeud*> ent, vector<Noeud*> seq, Noeud* var);

	~NoeudSwitch() {
	}
	int getValeur();
	void afficher(unsigned short indentation = 0);
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	vector<Noeud*> entiers;
	vector<Noeud*> sequence;
	Noeud* variable;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudIncrementation : public Noeud {
public:
	NoeudIncrementation(Noeud* var, Symbole incre, Noeud* exp = NULL); // construit une séquence d'instruction vide

	~NoeudIncrementation() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation = 0); // affiche la séquence d'instructions
	void translateAda(ostream& s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Noeud* variable;
	Symbole incrementation;
	Noeud* expression;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudSautLigne : public Noeud {
public:
	NoeudSautLigne(Noeud* ent);

	~NoeudSautLigne() {
	}
	int getValeur();
	void afficher(unsigned short indentation = 0);
	void translateAda(ostream &s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

protected:
	Noeud* entier;
};

class NoeudEcrLigne : public Noeud {
public:
	NoeudEcrLigne(Noeud* var, Noeud* entier);

	~NoeudEcrLigne() {
	}
	int getValeur();
	void afficher(unsigned short indentation = 0);
	void translateAda(ostream &s, unsigned short indentation = 0);
	void translatePhp(ostream& s, unsigned short indentation = 0);

private:
	Noeud* valeur;
	Noeud* entier;
};


////////////////////////////////////////////////////////////////////////////////
//class NoeudFonction : public Noeud {
//public:
//	NoeudFonction(Noeud* seqInst);
//	~NoeudFonction() { };
//	int getValeur();
//	void afficher(unsigned short indentation = 0);
//private:
//	Noeud* seqInst;
//};

#endif /* ARBRE_H_ */
