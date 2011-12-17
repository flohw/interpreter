#ifndef LECTEURPHRASESIMPLE_H_
#define LECTEURPHRASESIMPLE_H_

#include "LecteurSymbole.h"

class LecteurPhraseSimple {
public:
    LecteurPhraseSimple(string nomFich); // Construit un lecteur de phrase pour vérifier
                                         // la syntaxe du programme dans le fichier nomFich

    void analyse();   // Si le contenu du fichier est conforme à la grammaire,
                      // cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                      // Sinon, le programme sera interrompu (exit).
        	
private:
    LecteurSymbole ls;  // le lecteur de symboles utilisé pour lyser le fichier		

    // implémentation de la grammaire
    void programme();   // <programme> ::= debut <seqInst> fin <EOF>
    void seqInst();	    // <seq_ins> ::= <inst> ; { <inst> ; }
    void inst();	    // <inst> ::= <affectation> || <instSi> || <instTq> || <instRepeter> || <insEcrire>
	
	void instEcrire();	// <instEcrire> ::= ecrire(<chaine>)
	void instLire();	// <instLire> ::= lire(<variable>)
	
	void instSi();		// <instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> } [ sinon <seqInst> ] finsi
	void instTq();		// <instTq> ::= tq ( <expBool> ) <seqInst> fintq
	void instFaire();	// <instFaire> ::= faire <seqInst> jusqua ( <expBool> )
	void instPour();	// <instPour> ::= pour ( <affectation> ; <expBool> ; <affectation> ) <seqInst> finpour
	
    void affectation();	// <affectation> ::= <variable> = <expression>
	void chaine();		// <chaine> ::= "<des mots>"
    void facteur();     // <facteur> ::= <entier>  |  <variable>  | <opUnaire> <expBool>  |  ( <expBool> )
	void terme();		// <terme> ::= <facteur> { <opMult> <facteur> }
	void termeBool();	// <termeBool> ::= <relation> { <opEt> <relation> }
	
	void opAdd();		// <opAdd> ::= + | -
	void opMult();		// <opMult> ::= * | /
	void opEt();		// <opEt> ::= et
	void opOu();		// <opOu> ::= ou
	void opRel();		// <opRel> ::= == | != | < | > | <= | >=
	void opUnaire();	// <opUnaire> ::= non | -
	
	void expression();	// <expression> ::= <terme> { <opAdd> <terme> }
	void expBool();		// <expBool> ::= <termeBool> { <opOu> <termeBool> }
	void relation();	// <relation> ::= <expression> { <opRel> <expression> }
	
    // outils pour se simplifier l'analyse syntaxique
    void testerSymCour (string ch);  // si symbole courant != ch, erreur : on arrete le programme, sinon rien
    void sauterSymCour (string ch);  // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
    void erreur (string mess);      // affiche les message d'erreur mess et arrete le programme
};

#endif /* LECTEURPHRASESIMPLE_H_ */
