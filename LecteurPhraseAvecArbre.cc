#include "LecteurPhraseAvecArbre.h"
#include "Exception.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecArbre::LecteurPhraseAvecArbre(string nomFich) :
	ls(nomFich), ts(), fichier() {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::analyse() {
	arbre = programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::programme() {
	// <programme> ::= debut <seq_inst> fin FIN_FICHIER

	sauterSymCour("debut");
	Noeud* si = seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
	return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::seqInst() {
	// <seqInst> ::= <inst> ; { <inst> ; }
	NoeudSeqInst* si = new NoeudSeqInst();
	do {
		si->ajouteInstruction(inst());
		sauterSymCour(";");
	} while (ls.getSymCour() == "si" || ls.getSymCour() == "ecrire" || ls.getSymCour() == "lire" ||
			ls.getSymCour() == "tq" || ls.getSymCour() == "faire" || ls.getSymCour() == "pour" || 
			ls.getSymCour() == "selon" || ls.getSymCour() == "alaligne" || ls.getSymCour() == "ecrire_ligne" ||
			ls.getSymCour() == "<VARIABLE>");
	// tant que le symbole courant est un debut possible d'instruction...
	return si;
}
//
//////////////////////////////////////////////////////////////////////////////////
//Noeud* LecteurPhraseAvecArbre::fonction() {
////	<fonction> ::= fonction <nomFonction> ({<param>}) <seqInst> { <seqInst> } [retour <facteur>] finfonc
//	sauterSymCour("fonction");
//	sauterSymCour("(");
//	
//	sauterSymCour(")");
//	
//}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::inst() {
// <inst> ::= <affectation> | <inst_condi>
	if (ls.getSymCour() == "ecrire")
		return instEcrire();
	else if (ls.getSymCour() == "alaligne")
		return instSautLigne();
	else if (ls.getSymCour() == "ecrire_ligne")
		return instEcrLigne();
	else if (ls.getSymCour() == "si")
		return instSi();
	else if (ls.getSymCour() == "tq")
		return instTq();
	else if (ls.getSymCour() == "selon")
		return instSwitch();
	else if (ls.getSymCour() == "faire")
		return instFaire();
	else if (ls.getSymCour() == "lire")
		return instLire();
	else if (ls.getSymCour() == "pour")
		return instPour();
	else if (ls.getSymCour() == "<VARIABLE>") // Si AFFECTATION
		return affectation();
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instEcrire() {
// <instEcrire> ::= ecrire(<chaine> | <expression>)
	sauterSymCour("ecrire");
	sauterSymCour("(");
	Noeud* var;
	if (ls.getSymCour() == "<CHAINE>") {
		var = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	}
	else
		var = expression();
	sauterSymCour(")");
	return new NoeudEcrire(var);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instEcrLigne() {
	sauterSymCour("ecrire_ligne");
	sauterSymCour("(");
	Noeud* var;
	if (ls.getSymCour() == "<CHAINE>") {
		var = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	}
	else
		var = expression();
	sauterSymCour(")");	
	return new NoeudEcrLigne(var, ts.chercheAjoute(Symbole("1")));

}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instLire() {
//	<instLire> ::= lire(<VARIABLE>)
	sauterSymCour("lire");
	sauterSymCour("(");
	
	testerSymCour("<VARIABLE>");
	Noeud* var = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour(")");
	return new NoeudLire(var);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instFaire() {
//	<instFaire> ::= faire <seqInst> jusqua ( <expBool> )
	sauterSymCour("faire");
	Noeud* seq = seqInst();
	sauterSymCour("jusqua");
	sauterSymCour("(");
	Noeud* exp = expBool();
	sauterSymCour(")");
	return new NoeudInstBoucle(exp, seq, "faire");
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instTq() {
//	<instTq> ::= tq ( <expBool> ) <seqInst> fintq
	Noeud *exp, *seq;
	sauterSymCour("tq");
	sauterSymCour("(");
	exp = expBool();
	sauterSymCour(")");
	seq = seqInst();
	sauterSymCour("fintq");
	
	return new NoeudInstBoucle(exp, seq, "tq");
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instPour() {
//	<instPour> ::= pour ( <affectation> ; <expBool> ; <affectation> ) <seqInst> finpour
	sauterSymCour("pour");
	sauterSymCour("(");
	Noeud* aff = affectation();
	sauterSymCour(";");
	Noeud* exp = expBool();
	sauterSymCour(";");
	Noeud* incr = affectation();
	sauterSymCour(")");
	Noeud* seq = seqInst();
	sauterSymCour("finpour");
	return new NoeudInstBoucle(exp, seq, "pour", aff, incr);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instSi() {
//	<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> } [ sinon <seqInst> ] finsi
	vector<Noeud*> exp;
	vector<Noeud*> seq;
	
	sauterSymCour("si");
	sauterSymCour("(");
	exp.push_back(expBool());
	sauterSymCour(")");
	seq.push_back(seqInst());
	while (ls.getSymCour() == "sinonsi") {
		ls.suivant();
		sauterSymCour("(");
		exp.push_back(expBool());
		sauterSymCour(")");
		seq.push_back(seqInst());
	}
	if (ls.getSymCour() == "sinon") {
		ls.suivant();
		seq.push_back(seqInst());
	}
	sauterSymCour("finsi");
	return new NoeudInstSi(exp, seq);
}

////////////////////////////////////////////////////////////////////////////////
Noeud*  LecteurPhraseAvecArbre::instSwitch() {
//<instSwitch> ::= switch (<VARIABLE>) { case <ENTIER>: <seqInst> break; } [default: <seqInst> break;] endswitch

	vector<Noeud*> ent;
	vector<Noeud*> seq;

	sauterSymCour("selon");
	sauterSymCour("(");
	testerSymCour("<VARIABLE>");
	Noeud* var = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour(")");
	
	while (ls.getSymCour() == "cas") {
		sauterSymCour("cas");
		testerSymCour("<ENTIER>");
		Noeud* entier = ts.chercheAjoute(ls.getSymCour());
		ent.push_back(entier);
		ls.suivant();
		sauterSymCour(":");
		seq.push_back(seqInst());
		sauterSymCour("stop");
		sauterSymCour(";");
	}
	
	if (ls.getSymCour() == "defaut") {
		ls.suivant();
		sauterSymCour(":");
		seq.push_back(seqInst());
		sauterSymCour("stop");
		sauterSymCour(";");
	}
	sauterSymCour("finselon");
	return new NoeudSwitch(ent, seq, var);
}

////////////////////////////////////////////////////////////////////////////////
Noeud*	LecteurPhraseAvecArbre::instSautLigne() {
//	va à la ligne NB fois avec NB qui suit alaligne
	Noeud* entier;
	sauterSymCour("alaligne");
	sauterSymCour("(");
		if (ls.getSymCour() == "<ENTIER>") {
			entier = ts.chercheAjoute(ls.getSymCour());
			ls.suivant();
		}
		else {
			entier = ts.chercheAjoute(Symbole("1"));
		}
	sauterSymCour(")");
	return new NoeudSautLigne(entier);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::affectation() {
// <affectation> ::= <variable> = <expression> || <variable> "++" | "--" || <variable> += <expression> || <variable> -= <expression> 
	testerSymCour("<VARIABLE>");
	Noeud* var = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	if (ls.getSymCour()=="--" || ls.getSymCour()=="++") {
		Symbole incre = ls.getSymCour();
		ls.suivant();
		return new NoeudIncrementation(var,incre);
	}
	else if (ls.getSymCour()=="+=" || ls.getSymCour()=="-=" || ls.getSymCour()=="*=" || ls.getSymCour()=="/=") {
		Symbole incre = ls.getSymCour();
		ls.suivant();
		Noeud* exp = expression();
		return new NoeudIncrementation(var,incre,exp);
	}
	else {
		sauterSymCour("=");
		Noeud* exp = expression();
		return new NoeudAffectation(var,exp);
	}
}


////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::terme() {
//  <terme> ::= <facteur> { <opMult> <facteur> }
	Noeud* fact = facteur();
	while (ls.getSymCour() == "*" || ls.getSymCour() == "/") {
		Symbole operateur = opMult(); // on stocke le symbole de l'opérateur
		Noeud* factDroit = facteur(); // lecture de l'operande droit
		fact = new NoeudOperateurBinaire(operateur,fact,factDroit); // const. du noeud
	}
	return fact;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::expression() {
// <expression> ::= <terme> { <opAd> <terme> }
	Noeud* termeGauche = terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		Symbole operateur = opAdd(); // on stocke le symbole de l'opérateur
		Noeud* termeDroit = terme(); // lecture de l'operande droit
		termeGauche = new NoeudOperateurBinaire(operateur,termeGauche,termeDroit); // const. du noeud
	}
	return termeGauche;
}


////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::relation() {
// <relation> ::= <expression> { <opRel> <expression> }
	Noeud* expGauche = expression();
	while (ls.getSymCour()=="==" || ls.getSymCour() == "!=" || ls.getSymCour() == "<" ||
			ls.getSymCour() == ">" || ls.getSymCour() == "<=" || ls.getSymCour() == ">=") {
		Symbole operateur = opRel(); // on stocke le symbole de l'opérateur
		Noeud* expDroit = expression(); // lecture de l'operande droit
		expGauche = new NoeudOperateurBinaire(operateur,expGauche,expDroit); // const. du noeud
	}
	return expGauche;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::termeBool() {
// <termeBool> ::= <relation> { <opEt> <relation> }
	Noeud* relationGauche = relation();
	while (ls.getSymCour()=="et") {
		Symbole operateur = opEt(); // on stocke le symbole de l'opérateur
		Noeud* relationDroit = relation(); // lecture de l'operande droit
		relationGauche = new NoeudOperateurBinaire(operateur,relationGauche,relationDroit); // const. du noeud
	}
	return relationGauche;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::expBool() {
// <expBool> ::= <termeBool> { <opOu> <termeBool> }
	Noeud* expGauche = termeBool();
	while (ls.getSymCour()=="ou") {
		Symbole operateur = opOu(); // on stocke le symbole de l'opérateur
		Noeud* expDroit = relation(); // lecture de l'operande droit
		expGauche = new NoeudOperateurBinaire(operateur,expGauche,expDroit); // const. du noeud
	}
	return expGauche;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::facteur() {
// <facteur> ::= <entier>  |  <variable>  |  <opUnaire> <expBool>  |  ( <expBool> )

	Noeud* fact=NULL;

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
		fact = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else if (ls.getSymCour()=="-" || ls.getSymCour() == "non") {
		Symbole sym = ls.getSymCour();
		ls.suivant();
		// on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
		fact = new NoeudOperateurBinaire(sym, ts.chercheAjoute(Symbole("0")), expBool());
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		fact=expBool();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
	return fact;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opAdd() {
// <opAdd> ::= + | -
	Symbole operateur;
	if (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opAdd>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opMult() {
// <opMult> ::= * | /
	Symbole operateur;
	if (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opMult>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opEt() {
// <opEt> ::= et
	Symbole operateur;
	if (ls.getSymCour()=="et") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opEt>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opOu() {
// <opOu> ::= ou
	Symbole operateur;
	if (ls.getSymCour()=="ou") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opOu>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opRel() {
// <opRel> ::= == | != | < | > | <= | >=
	Symbole operateur;
	if (ls.getSymCour()=="==" || ls.getSymCour() == "!=" || ls.getSymCour() == "<" ||
			ls.getSymCour() == ">" || ls.getSymCour() == "<=" || ls.getSymCour() == ">=") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opRel>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opUnaire() {
// <opUnaire> ::= ! | -
	Symbole operateur;
	if (ls.getSymCour()=="non" || ls.getSymCour() == "-") {
		operateur=ls.getSymCour();
		ls.suivant();
	}
	else
		erreur("<opUnaire>");
	return operateur;
}

////////////////////////////////////////////////////////////////////////////////
// Methodes publiques
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::translateAda() {
	unsigned int m(0);
	cout << "Traduction en ADA" << endl;
	cout << "1. Dans un fichier" << endl;
	cout << "2. Sur la console" << endl;
	do {
		cout << "Choix menu ? (1 ou 2) ";
		cin >> m;
	} while (m < 1 or m > 2);
	switch (m) {
		case 1: {
			string f("");
			cout << "Nom du fichier (extension adb ajoutee automatiquement) ? ";
			cin >> f;
			f += ".adb";
			fichier.open(f.c_str(), ios_base::out);
			if (fichier.fail()) {
				cout << "Impossible de creer le fichier " << f << endl;
				exit(0);
			}
			fichier << "WITH p_esiut; USE p_esiut;" << endl << "PROCEDURE " << f << " IS" << endl;
			for (unsigned int i = ts.getFirstVariable(); i < ts.getNbVariables(); i++)
			{
				cout << setw(5) << ts.getNomVariable(i) << " : integer;" << endl;
			}
			fichier << "BEGIN" << endl;
			this->getArbre()->translateAda(fichier);
			fichier << "END " << f << ";";
			fichier.close();
			break; }
		case 2: {
			cout << "WITH p_esiut; USE p_esiut;" << endl << "PROCEDURE main IS" << endl;
			for (unsigned int i = ts.getFirstVariable(); i < ts.getNbVariables(); i++)
			{
				cout << setw(5) << ts.getNomVariable(i) << " : integer;" << endl;
			}
			cout << "BEGIN" << endl;
			this->getArbre()->translateAda(cout);
			cout << "END main;" << endl;
			break; }
	}
}

void LecteurPhraseAvecArbre::translatePhp() {
	unsigned int m(0);
	cout << "Traduction en PHP" << endl;
	cout << "1. Dans un fichier" << endl;
	cout << "2. Sur la console" << endl;
	do {
		cout << "Choix menu ? (1 ou 2) ";
		cin >> m;
	} while (m < 1 or m > 2);
	switch (m) {
		case 1: {
			string f("");
			cout << "Nom du fichier (extension php ajoutee automatiquement) ? ";
			cin >> f;
			f += ".php";
			fichier.open(f.c_str(), ios_base::out);
			if (fichier.fail()) {
				cout << "Impossible de creer le fichier " << f << endl;
				exit(0);
			}
			fichier << "<?php" << endl;
			this->getArbre()->translatePhp(fichier);
			fichier << "?>" << endl;
			fichier.close();
			break; }
		case 2: {
			cout << "<?php" << endl;
			this->getArbre()->translatePhp(cout);
			cout << "?>" << endl;
			break; }
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		throw MauvaisSymbole(ch); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
