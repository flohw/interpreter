#include "LecteurPhraseSimple.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////

LecteurPhraseSimple::LecteurPhraseSimple(string nomFich) :
ls(nomFich) {
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::programme() {
	// <programme> ::= debut <seqInst> fin FIN_FICHIER

	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::seqInst() {
	// <seqInst> ::= <inst> ; { <inst> ; }
	do {
		inst();
		sauterSymCour(";");
	} while (ls.getSymCour() == "si" || ls.getSymCour() == "ecrire" || ls.getSymCour() == "lire" ||
			ls.getSymCour() == "tq" || ls.getSymCour() == "faire" || ls.getSymCour() == "pour" || ls.getSymCour() == "<VARIABLE>");
	// tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::inst() {
	// <inst> ::= <affectation> | <instSi> || <instTq> || <instRepeter> || <insEcrire>
	if (ls.getSymCour() == "ecrire") // SI INSTRUCTION ECRIRE
		instEcrire();
	else if (ls.getSymCour() == "si") // SI INSTRUCTION SI
		instSi();
	else if (ls.getSymCour() == "tq")
		instTq();
	else if (ls.getSymCour() == "faire")
		instFaire();
	else if (ls.getSymCour() == "lire")
		instLire();
	else if (ls.getSymCour() == "pour")
		instPour();
	else if (ls.getSymCour() == "<VARIABLE>") // Si AFFECTATION
		affectation();
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instTq() {
	// <instTq> ::= tq ( <expBool> ) { <seqInst> } fintq
	sauterSymCour("tq");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("fintq");
}

void LecteurPhraseSimple::instPour() {
	// <instPour> ::= pour (<affectation> ; <expBool> ; <affectation>) <seqInst> finpour
	sauterSymCour("pour");
	sauterSymCour("(");
	affectation();
	sauterSymCour(";");
	expBool();
	sauterSymCour(";");
	affectation();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("finpour");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instFaire() {
	// <instFaire> ::= faire <seqInst> jusqua ( <expBool> )
	sauterSymCour("faire");
	seqInst();
	sauterSymCour("jusqua");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instSi() {
	sauterSymCour("si");
	sauterSymCour("(");
	expression();
	sauterSymCour(")");
	seqInst();
	while (ls.getSymCour() == "sinonsi") {
		sauterSymCour("sinonsi");
		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
		seqInst();
	}
	if (ls.getSymCour() == "sinon") {
		sauterSymCour("sinon");
		seqInst();
	}
	sauterSymCour("finsi");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instEcrire() {
	// <instEcrire> ::= ecrire(<chaine> | <expression>)
	sauterSymCour("ecrire");
	sauterSymCour("(");
	if (ls.getSymCour() == "<CHAINE>")
		chaine();
	else
		expression();
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instLire() {
	// <instLire> ::= lire(<variable>)
	sauterSymCour("lire");
	sauterSymCour("(");
	sauterSymCour("<VARIABLE>");
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::chaine() {
	// <chaine> ::= "<des mots>"
	sauterSymCour("<CHAINE>");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::affectation() {
	// <affectation> ::= <variable> = <expression>

	sauterSymCour("<VARIABLE>");
	sauterSymCour("=");
	relation();
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::expression() {
	// <expression> ::= <terme> { <opAdd> <terme> }
	terme();
	while (ls.getSymCour() == "+" || ls.getSymCour() == "-") {
		opAdd();
		terme();
	}
}

void LecteurPhraseSimple::opAdd() {
	if (ls.getSymCour() == "+" || ls.getSymCour() == "-")
		ls.suivant();
	else
		erreur("<opAdd>");
}

void LecteurPhraseSimple::opMult() {
	if (ls.getSymCour() == "*" || ls.getSymCour() == "/")
		ls.suivant();
	else
		erreur("<opMult>");
}

void LecteurPhraseSimple::terme() {
	// <terme> ::= <facteur> { <opMult> <facteur> }
	facteur();
	while (ls.getSymCour() == "*" || ls.getSymCour() == "/") {
		opMult();
		facteur();
	}
}

void LecteurPhraseSimple::termeBool() {
	// <termeBool> ::= <relation> { <opEt> <relation> }
	relation();
	while (ls.getSymCour() == "et") {
		opEt();
		relation();
	}
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::facteur() {
	// <facteur> ::= <entier>  |  <variable>  | <opUnaire> <expBool>  |  ( <expBool> )
	if (ls.getSymCour() == "<VARIABLE>" || ls.getSymCour() == "<ENTIER>")
		ls.suivant();
	else if (ls.getSymCour() == "-" || ls.getSymCour() == "non") {
		opUnaire();
		expBool();
	} else if (ls.getSymCour() == "(") {
		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
}

void LecteurPhraseSimple::expBool() {
	// <expBool> ::= <termeBool> { <opOu> <termeBool> }
	termeBool();
	while (ls.getSymCour() == "ou") {
		opOu();
		termeBool();
	}
}

void LecteurPhraseSimple::opEt() {
	// <opEt> ::= et
	if (ls.getSymCour() == "et")
		ls.suivant();
	else
		erreur("<opEt>");
}

void LecteurPhraseSimple::opOu() {
	// <opOu> ::= ou
	if (ls.getSymCour() == "ou")
		ls.suivant();
	else
		erreur("<opOu>");
}

void LecteurPhraseSimple::relation() {
	// <relation> ::= <expression> { <opRel> <expression> }
	expression();
	while (ls.getSymCour() == "==" || ls.getSymCour() == "!=" || ls.getSymCour() == "<=" ||
			ls.getSymCour() == ">=" || ls.getSymCour() == "<" || ls.getSymCour() == ">") {
		opRel();
		expression();
	}
}

void LecteurPhraseSimple::opRel() {
	// <opRel> ::= == | != | < | > | <= | >=
	if (ls.getSymCour() == "==" || ls.getSymCour() == "!=" || ls.getSymCour() == "<=" ||
			ls.getSymCour() == ">=" || ls.getSymCour() == "<" || ls.getSymCour() == ">")
		ls.suivant();
	else
		erreur("<opRel>");
}

void LecteurPhraseSimple::opUnaire() {
	// <opUnaire> ::= non | -
	if (ls.getSymCour() == "-" || ls.getSymCour() == "non")
		ls.suivant();
	else
		erreur("<opUnaire>");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
