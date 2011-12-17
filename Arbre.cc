#include <stdlib.h>
#include <typeinfo>
#include <cstdio>
#include "Arbre.h"
#include "SymboleValue.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////
NoeudSeqInst::NoeudSeqInst() : tabInst() { }
int NoeudSeqInst::getValeur() {
  int valeur=0;
  for (unsigned int i=0; i<tabInst.size(); i++)
    valeur = tabInst[i]->getValeur();  // on evalue chaque instruction de la séquence
  return valeur; // par convention, resultat = valeur de la derniere instruction
}
void NoeudSeqInst::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Sequence de " << tabInst.size() << " instruction(s)" << endl;
  for (unsigned int i=0; i<tabInst.size(); i++)
    tabInst[i]->afficher(indentation+1); // on affiche les fils en augmentant l'indentation
}
void NoeudSeqInst::ajouteInstruction(Noeud* instruction) {
  tabInst.push_back(instruction);
}
void NoeudSeqInst::translateAda(ostream& s, unsigned short indentation) {
//	Noeud::translateAda(s, indentation);
	for (unsigned int i = 0; i < tabInst.size(); i++){
		tabInst[i]->translateAda(s, indentation+1);
		s << ";" << endl;
	}
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////
NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression) {
  this->variable=variable;
  this->expression=expression;
}
int NoeudAffectation::getValeur() {
  int valeur = expression->getValeur(); // on évalue l'expression
  ((SymboleValue*)variable)->setValeur(valeur); // on affecte la variable
  return valeur; // par convention, une affectation a pour valeur la valeur affectée
}
void NoeudAffectation::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Affectation" << endl;
  variable->afficher(indentation+1);   // on affiche variable et expression
  expression->afficher(indentation+1); // en augmentant l'indentation
}
void NoeudAffectation::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	s << ((SymboleValue*)variable)->getChaine() << " := ";
	if (typeid(SymboleValue) == typeid(*expression))
		s << ((SymboleValue*)expression)->getChaine();
	else
		expression->translateAda(s, 0);
}
Noeud* NoeudAffectation::getVariable() {
	return this->variable;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////
NoeudOperateurBinaire::NoeudOperateurBinaire ( Symbole operateur,
                                               Noeud* operandeGauche,
                                               Noeud* operandeDroit) {
  this->operateur=operateur;
  this->operandeGauche=operandeGauche;
  this->operandeDroit=operandeDroit;
}
int NoeudOperateurBinaire::getValeur() { 
  int valeur=0;
  int og=operandeGauche->getValeur();
  int od=operandeDroit->getValeur();
  if      (this->operateur=="+")	valeur = og+od; 
  else if (this->operateur=="-")	valeur = og-od;
  else if (this->operateur=="*")	valeur = og*od;
  else if (this->operateur=="==")	valeur = og==od; 
  else if (this->operateur=="!=")	valeur = og!=od;
  else if (this->operateur=="<")	valeur = og<od; 
  else if (this->operateur==">")	valeur = og>od;
  else if (this->operateur=="<=")	valeur = og<=od;
  else if (this->operateur==">=")	valeur = og>=od;
  else if (this->operateur=="et")	valeur = og and od;
  else if (this->operateur=="ou")	valeur = og or od;
  else if (od != 0)					valeur = og/od;
  else {cout << "Erreur pendant l'interpretation : division par zero" << endl;
	  	exit(0); // plus tard on levera une exception
  }
  return valeur;
}
void NoeudOperateurBinaire::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Operateur Binaire \"" << this->operateur.getChaine() << "\" applique a : " << endl;
  operandeGauche->afficher(indentation+1);  // on affiche fils gauche et fils droit
  operandeDroit->afficher(indentation+1);   // en augmentant l'indentation
}
void NoeudOperateurBinaire::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	if (typeid(SymboleValue) == typeid(*operandeGauche))
		s << ((SymboleValue*)operandeGauche)->getChaine();
	else
		operandeGauche->translateAda(s, 0);
	if (this->operateur=="et")		s << " and ";
	else if (this->operateur=="ou")	s << " or ";
	else if (this->operateur=="==")	s << " = ";
	else							s << " " << operateur.getChaine() << " ";
	if (typeid(SymboleValue) == typeid(*operandeDroit))
		s << ((SymboleValue*)operandeDroit)->getChaine();
	else
		operandeDroit->translateAda(s, 0);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudEcrire
////////////////////////////////////////////////////////////////////////////////
NoeudEcrire::NoeudEcrire(Noeud* val) : valeur(val) { }
void NoeudEcrire::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Ecrire" << endl;
		valeur->afficher(indentation+1); 
}
int NoeudEcrire::getValeur() {
	if (typeid(*valeur) == typeid(SymboleValue) and *(SymboleValue*)valeur == "<CHAINE>")
		cout << ((SymboleValue*)valeur)->getChaine().substr(1, ((SymboleValue*)valeur)->getChaine().size()-2);
	else
		cout << valeur->getValeur();
	return 0;
}
void NoeudEcrire::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	s << "ECRIRE (";
	if (typeid(*valeur) == typeid(SymboleValue))
		s << ((SymboleValue*)valeur)->getChaine();
	else
		valeur->translateAda(s, 0);
	s << ")";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudTantQue
////////////////////////////////////////////////////////////////////////////////
NoeudInstBoucle::NoeudInstBoucle(Noeud* exp, Noeud* seq, string t, Noeud* aff, Noeud* incr) :
	expression(exp),
	sequence(seq),
	type(t),
	affectation(aff),
	incrementation(incr) { }
int NoeudInstBoucle::getValeur() {
	if (type == "tq") {
		while (expression->getValeur())
			sequence->getValeur();
	} else if (type == "faire") {
		do
			sequence->getValeur();
		while (expression->getValeur());
	} else if (type == "pour") {
		for (affectation->getValeur(); expression->getValeur(); incrementation->getValeur())
			sequence->getValeur(); 
	}
	
	return 0;
}
void NoeudInstBoucle::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - ";
	if (type == "tq")
	{
		cout << "Tant que" << endl;
			expression->afficher(indentation+1);
			sequence->afficher(indentation+1);
	} else if (type == "faire") {
		cout << "Faire jusqu'a" << endl;
			expression->afficher(indentation+1);
			sequence->afficher(indentation+1);
	} else if (type == "pour") {
		cout << "Pour" << endl;
			affectation->afficher(indentation+1);
			expression->afficher(indentation+1);
			incrementation->afficher(indentation+1);
			sequence->afficher(indentation+1);
	}
}
void NoeudInstBoucle::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	if (type == "tq")
	{
		s << "WHILE "; expression->translateAda(s, 0); s << " LOOP" << endl;
			sequence->translateAda(s, indentation);
		Noeud::translateAda(s, indentation);
		s << "END LOOP";
	} else if (type == "faire") {
		s << "LOOP " << endl;
			sequence->translateAda(s, indentation);
			Noeud::translateAda(s, indentation+1);
			s << "EXIT WHEN ";
			expression->translateAda(s, 0);
		s << ";" << endl;
		Noeud::translateAda(s, indentation);
		s << "END LOOP";
	} else if (type == "pour") {
		s << "FOR (";
			affectation->translateAda(s, 0);
			s << "; ";
			expression->translateAda(s, 0);
			s << "; ";
			incrementation->translateAda(s, 0);
			s << ") LOOP" << endl;
			sequence->translateAda(s, indentation);
			Noeud::translateAda(s, indentation);
		s << "END LOOP";
	}
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////
NoeudInstSi::NoeudInstSi(vector<Noeud*> exp, vector<Noeud*> seq) : expression(exp), sequence(seq) { }
int NoeudInstSi::getValeur() {
	bool passe = false;
	unsigned int i = 0;
	while (i < sequence.size() and !passe)
	{
		if (i < expression.size() and expression[i]->getValeur())
		{
			sequence[i]->getValeur();
			passe = true;
		}
		else if(i >= expression.size())
			sequence[i]->getValeur();
		i++;
	}
	return 0;
}
void NoeudInstSi::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);		
	cout << "Noeud - Si" << endl;
	unsigned int i = 0;
	while (i < sequence.size())
	{
		if (i < expression.size())
		{
			expression[i]->afficher(indentation+1);
			sequence[i]->afficher(indentation+1);
		}
		else if(i >= expression.size())
			sequence[i]->afficher(indentation+1);
		i++;
	}
}
void NoeudInstSi::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	s << "IF ";
	unsigned int i(0);
	while (i < sequence.size())
	{
		if (i < expression.size())
		{
			if (i != 0) {
				Noeud::translateAda(s, indentation);
				s << "ELSIF ";
			}
			expression[i]->translateAda(s, 0);
			s << " THEN" << endl;
			sequence[i]->translateAda(s, indentation);
		}
		else if(i >= expression.size())
		{
			Noeud::translateAda(s, indentation);
			s << "ELSE" << endl;
			sequence[i]->translateAda(s, indentation);
		}
		i++;
	}
	Noeud::translateAda(s, indentation);
	s << "END IF";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudLire
////////////////////////////////////////////////////////////////////////////////
NoeudLire::NoeudLire(Noeud* var) : variable(var) { }
int NoeudLire::getValeur() {
	int val;
	cin >> val;
	((SymboleValue*)variable)->setValeur(val);
	return 0;
}
void NoeudLire::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Lire" << endl;
		variable->afficher(indentation+1);
}
void NoeudLire::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	s << "LIRE (";
	s << ((SymboleValue*)variable)->getChaine();
	s << ")";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudSwitch
////////////////////////////////////////////////////////////////////////////////
NoeudSwitch::NoeudSwitch(vector<Noeud*> ent, vector<Noeud*> seq, Noeud* var) :
	entiers(ent),
	sequence(seq),
	variable(var){ }

int NoeudSwitch::getValeur() {
	bool passe = false;
	unsigned int i = 0;
	cout << sequence.size() << endl;
	fflush(stdout);
	cout << entiers.size() << endl;
	fflush(stdout);
	
	while (i < sequence.size() and !passe)
	{
		if (i < entiers.size() and entiers[i]->getValeur() == variable->getValeur())
		{
			sequence[i]->getValeur();
			passe = true;
		}
		else if(sequence.size() > entiers.size() and i >= entiers.size())
			sequence[i]->getValeur();
		i++;
	}

	return 0;
}

void NoeudSwitch::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Selon" << endl;
	unsigned int i = 0;
	while (i < entiers.size())
	{
		entiers[i]->afficher(indentation+1);
		sequence[i]->afficher(indentation+2);
		i++;
	}
	if (sequence.size() > entiers.size()) {
		Noeud::afficher(indentation+1);
		cout << "Feuille - Cas Defaut" << endl;
		sequence[i]->afficher(indentation+2);
	}

	variable->afficher(indentation+1);
}
void NoeudSwitch::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	unsigned int i(0);
	s << "CASE " << ((SymboleValue*)variable)->getChaine() << " IS" << endl;
	while (i < entiers.size()) {
		Noeud::translateAda(s, indentation+1);
		s << "WHEN ";
		s << ((SymboleValue*)entiers[i])->getValeur();
		s << " =>" << endl;
		sequence[i]->translateAda(s, indentation+1);
		i++;
	}
	
	if (sequence.size() > entiers.size()) {
		Noeud::translateAda(s, indentation+1);
		s << "WHEN OTHERS =>" << endl;
		sequence[i]->translateAda(s, indentation+1);
	}
	Noeud::translateAda(s, indentation);
	s << "END CASE";
}

///////////////////////////////////////////////////////////////////////////////
// NoeudIncrementation
////////////////////////////////////////////////////////////////////////////////
NoeudIncrementation::NoeudIncrementation(Noeud* var, Symbole incre, Noeud* exp) :
	variable(var),
	incrementation(incre),
	expression(exp) { }
int NoeudIncrementation::getValeur() {
	if (incrementation.getChaine() == "++")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() + 1);
	else if (incrementation.getChaine() == "--")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() - 1);
	else if (incrementation.getChaine() == "-=")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() - (expression->getValeur()));
	else if (incrementation.getChaine() == "+=")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() + (expression->getValeur()));
	else if (incrementation.getChaine() == "*=")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() * (expression->getValeur()));
	else if (incrementation.getChaine() == "/=")
		((SymboleValue*)variable)->setValeur(((SymboleValue*)variable)->getValeur() / (expression->getValeur()));
	return ((SymboleValue*)variable)->getValeur();
}
void NoeudIncrementation::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Incrementation ";
  cout << ((SymboleValue*)variable)->getChaine();
  cout << incrementation.getChaine() << endl;
  if (expression != NULL)
	  expression->afficher(indentation+1);
}
void NoeudIncrementation::translateAda(ostream& s, unsigned short indentation) {
	Noeud::translateAda(s, indentation);
	SymboleValue* var = ((SymboleValue*)variable);
	if (incrementation.getChaine() == "++") {
		s << var->getChaine() << " := " << var->getChaine() << " + 1";
	} else if (incrementation.getChaine() == "--") {
		s << var->getChaine() << " := " << var->getChaine() << " - 1";
	} else if (incrementation.getChaine() == "+=") {
		s << var->getChaine() << " := " << var->getChaine() << " +";
	} else if (incrementation.getChaine() == "-=") {
		s << var->getChaine() << " := " << var->getChaine() << " -";
	} else if (incrementation.getChaine() == "*=") {
		s << var->getChaine() << " := " << var->getChaine() << " *";
	} else if (incrementation.getChaine() == "/=") {
		s << var->getChaine() << " := " << var->getChaine() << " /";
	}
	if (expression != NULL and typeid(*expression) == typeid(SymboleValue))
		s << " " << ((SymboleValue*)expression)->getChaine();
	else if (expression != NULL)
		expression->translateAda(s);
}

///////////////////////////////////////////////////////////////////////////////
// NoeudSautLigne
////////////////////////////////////////////////////////////////////////////////
NoeudSautLigne::NoeudSautLigne(Noeud* ent) :
	entier(ent) { }

int NoeudSautLigne::getValeur() {
	for (int i = 0; i < entier->getValeur(); ++i)
	{
		cout << endl;
	}
	return entier->getValeur();
}

void NoeudSautLigne::afficher(unsigned short indentation){
	Noeud::afficher(indentation);
 	cout << "Noeud - Saut de Ligne " << endl;
	entier->afficher(indentation+1);
}

void NoeudSautLigne::translateAda(ostream& s, unsigned short indentation){
	Noeud::translateAda(s, indentation);
	s << "A_LA_LIGNE (" << ((SymboleValue*)entier)->getChaine() << ")";
}

///////////////////////////////////////////////////////////////////////////////
// NoeudEcrLigne
////////////////////////////////////////////////////////////////////////////////
NoeudEcrLigne::NoeudEcrLigne(Noeud* var, Noeud* ent) :
	valeur(var), entier(ent) { }

int NoeudEcrLigne::getValeur() {
	if (typeid(*valeur) == typeid(SymboleValue) and *(SymboleValue*)valeur == "<CHAINE>")
		cout << ((SymboleValue*)valeur)->getChaine().substr(1, ((SymboleValue*)valeur)->getChaine().size()-2);
	else
		cout << valeur->getValeur();
	
	for (int i = 0; i < entier->getValeur(); i++)
		cout << endl;
	return 0;
}

void NoeudEcrLigne::afficher(unsigned short indentation){
	Noeud::afficher(indentation);
	cout << "Noeud - Ecrire Ligne" << endl;
		valeur->afficher(indentation+1); 
		entier->afficher(indentation+1);
 	
}

void NoeudEcrLigne::translateAda(ostream& s, unsigned short indentation){
	Noeud::translateAda(s, indentation);
	s << "ECRIRE_LIGNE (" << ((SymboleValue*)valeur)->getChaine() << ")";
}



////////////////////////////////////////////////////////////////////////////////
// NoeudFonction
////////////////////////////////////////////////////////////////////////////////
//NoeudFonction::NoeudFonction(Noeud* seq) : seqInst(seq) { }
//int NoeudFonction::getValeur() {
//	seqInst->getValeur();
//	return 0;
//}
//void NoeudFonction::afficher(unsigned short indentation) {
//	Noeud::afficher(indentation);
//	cout << "Noeud - Fonction" << endl;
//		seqInst->afficher(indentation+1);
//}

