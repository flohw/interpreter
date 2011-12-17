#ifndef LECTEURPHRASEAVECARBRE_H_
#define LECTEURPHRASEAVECARBRE_H_

#include "Symbole.h"
#include "LecteurSymbole.h"
#include "TableSymboles.h"
#include "Arbre.h"
#include <string>
using namespace std;

class LecteurPhraseAvecArbre
{
public:
	LecteurPhraseAvecArbre(string nomFich);	 // Construit un lecteur de phrase pour interpreter
	                                         //  le programme dans le fichier nomFich

	void analyse();  // Si le contenu du fichier est conforme à la grammaire,
	                 //   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                         //   la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
	                 // Sinon, le programme sera interrompu (exit).

	inline	TableSymboles getTs ()    { return ts;    } // accesseur	
	inline	Noeud*        getArbre () { return arbre; } // accesseur
	void	translateAda();
	

private:
    LecteurSymbole	ls;    // le lecteur de symboles utilisé pour analyser le fichier
    TableSymboles	ts;    // la table des symboles valués
    Noeud*			arbre; // l'arbre abstrait
	ofstream		fichier;

    // implémentation de la grammaire
    Noeud*  programme();	//	<programme> ::= debut <seqInst> fin FIN_FICHIER
    Noeud*  seqInst();		//	<seqInst> ::= <inst> ; { <inst> ; }
//	Noeud*	fonction();		//	<fonction> ::= fonction <nomFonction> ({<param>}) <seqInst> { <seqInst> } [retour <facteur>] finfonc
    Noeud*  inst();			//	<inst> ::= <affectation>
	
	Noeud*	instTq();		//	<instTq> ::= tq ( <expBool> ) <seqInst> fintq
	Noeud*	instFaire();	//	<instFaire> ::= faire <seqInst> jusqua ( <expBool> )
	Noeud*	instPour();		//	<instPour> ::= pour ( <affectation> ; <expBool> ; <affectation> ) <seqInst> finpour
	Noeud*	instEcrire();	//	<instEcrire> ::= ecrire | ecrire_ligne(<chaine> | <expression>)
	Noeud*	instLire();		//	<instLire> ::= lire(<VARIABLE>)
	Noeud*	instSi();		//	<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> } [ sinon <seqInst> ] finsi
	Noeud*	instSwitch();	//	<instSwitch> ::= switch (<VARIABLE>) { case <ENTIER>: [<seqInst> break;] } [default: <seqInst> break;] endswitch
	Noeud*	instSautLigne();//	<instSautLigne> ::= alaligne ( [<ENTIER>] )
	Noeud*	instEcrLigne();	//	<instEcrLigne> ::= <instEcrire> + NoeudSautLigne
	
    Noeud*  affectation();	//	<affectation> ::= <variable> = <expression>
    Noeud*  expression();	//	<expression> ::= <terme> { <opAdd> <terme> }
	Noeud*	expBool();		//	<expBool> ::= <termeBool> { <opOu> <termeBool> }
	Noeud*	relation();		//	<relation> ::= <expression> { <opRel> <expression> }
	Noeud*  terme();		//	<terme> ::= <facteur> { <opMult> <facteur> }
	Noeud*	termeBool();	//	<termeBool> ::= <relation> { <opEt> <relation> }
    Noeud*  facteur();		//	<facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

	Symbole opAdd();		//	<opAdd> ::= + | -
	Symbole opMult();		//	<opMult> ::= * | /
	Symbole opEt();			//	<opEt> ::= et
	Symbole opOu();			//	<opOu> ::= ou
	Symbole opRel();		//	<opRel> ::= == | != | < | > | <= | >=
	Symbole opUnaire();		//	<opUnaire> ::= non | -
	
	
    // outils pour se simplifier l'analyse syntaxique
    void testerSymCour (string ch);  // si symbole courant != ch, erreur : on arrete le programme, sinon rien
    void sauterSymCour (string ch);  // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
    void erreur (string mess);      // affiche les message d'erreur mess et arrete le programme
};

#endif /* LECTEURPHRASEAVECARBRE_H_ */
