#include "LecteurPhraseSimple.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
	string nomFich;
	if (argc != 2) {
		cout << "Usage : " << argv[0] << " nom_fichier_source" << endl << endl;
		cout << "Entrez le nom du fichier que vous voulez analyser : ";
		getline(cin,nomFich);
	} else
		nomFich=argv[1];

	LecteurPhraseSimple lp(nomFich);
	lp.analyse();
}
