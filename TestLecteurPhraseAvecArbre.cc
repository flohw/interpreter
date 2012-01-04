#include <iostream>
#include "Exception.h"
using namespace std;
#include "LecteurPhraseAvecArbre.h"

int main(int argc, char* argv[]) {
	char nomFich[255];
	try {
		if (argc != 2) {
			cout << "Usage : " << argv[0] << " nom_fichier_source" << endl << endl;
			cout << "Entrez le nom du fichier que voulez-vous interpreter : ";
			cin.getline(nomFich, sizeof(nomFich));
		} else
			strncpy(nomFich, argv[1], sizeof(nomFich));

		LecteurPhraseAvecArbre lp(nomFich);
		lp.analyse();
//		cout << endl << "Arbre Abstrait : " << endl;
//		cout         << "================"  << endl;
//		lp.getArbre()->afficher();
//		cout << endl << "Table des symboles avant evaluation : " << lp.getTs();
		cout << endl << "Evaluation de l'arbre (interpretation)..." << endl;
		lp.getArbre()->getValeur();
//		cout << endl << "Table des symboles apres evaluation : " << lp.getTs();
//		cout << endl << endl << endl;
//		cout << "-----------------ADA---------------" << endl;
//		lp.translateAda();
//		cout << "-----------------PHP---------------" << endl;
//		lp.translatePhp();
		return 0;
		}

	catch (Erreur e) {
        cout << e.what() << endl;
    }

    catch (MauvaisSymbole e) {
        cout << e.what() << endl;
    }
}

