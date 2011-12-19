#include "LecteurSymbole.h"
#include "Symbole.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	string nomFich;
	if (argc != 2) {
		cout << "Usage : " << argv[0] << " nom_fichier_source" << endl << endl ;
		cout << "Entrez le nom du fichier dont voulez-vous lire les symboles : ";
		getline(cin,nomFich);
	}
	else nomFich=argv[1];

	LecteurSymbole ls(nomFich);
	while (ls.getSymCour()!="<FINDEFICHIER>") {
		cout << "J'ai lu : " << ls.getSymCour() << endl;
		ls.suivant();
	}
	cout << "J'ai lu : " << ls.getSymCour() << endl;

	return EXIT_SUCCESS;
}
