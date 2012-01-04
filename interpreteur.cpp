#include <iostream>
#include <cstdlib>
#include "Exception.h"
using namespace std;
#include "LecteurPhraseAvecArbre.h"

void executionTest(int fichier) {
	string fichiers[8] = {"testSelon.txt", "testSi.txt", "testPour.txt",
		"testTantque.txt", "testFaire.txt", "testInst.txt",
		"testTraductionAda.txt"};
	int sousMenu(0);
	try {
		LecteurPhraseAvecArbre lp(fichiers[fichier]);
		lp.analyse();
		do {
			cout << "********Sous Menu*********" << endl;
			cout << "1. Affichage de l'arbre" << endl;
			cout << "2. Execution du programme" << endl;
			cout << "3. Traduction ADA" << endl;
			cout << "4. Traduction PHP" << endl;
			cout << "0. Retour" << endl;
			cout << "**************************" << endl;
			do {
				cout << "Choix ? (de 0 à 4) ";
				cin >> sousMenu;
			} while (sousMenu < 0 or sousMenu > 4);
			system("clear");
			switch (sousMenu) {
				case 1:
					cout << endl << "***Affichage de l'arbre***" << endl << endl;
					lp.getArbre()->afficher();
					break;
				case 2:
					cout << endl << "***Execution du programme***" << endl << endl;
					lp.getArbre()->getValeur();
					break;
				case 3:
					cout << endl << "***Traduction en ADA***" << endl << endl;
					lp.translateAda();
					break;
				case 4:
					cout << endl << "***Traduction en PHP***" << endl << endl;
					lp.translatePhp();
					break;
				case 0:
					break;
			}
			cout << endl;
		} while (sousMenu != 0);
	} catch (Erreur e) {
		cout << e.what() << endl;
	} catch (MauvaisSymbole e) {
		cout << e.what() << endl;
	}
}

int main(void) {

	int menuTest(0);

	do {
		cout << "***********Menu Principal***********" << endl;
		cout << "1. Test Selon" << endl;
		cout << "2. Test Si" << endl;
		cout << "3. Test Pour" << endl;
		cout << "4. Test Tant que" << endl;
		cout << "5. Test Faire jusqua" << endl;
		cout << "6. Test Instructions supplémentaires" << endl;
		cout << "7. Test Traductions" << endl;
		cout << "0. Quitter" << endl;
		cout << "************************************" << endl;
		do {
			cout << "Choix ? (de 0 à 7) ";
			cin >> menuTest;
		} while (menuTest < 0 or menuTest > 7);
		system("clear");
		if (menuTest != 0)
			executionTest(menuTest - 1);
	} while (menuTest != 0);
	cout << "******** Fin du Programme interpreteur ********" << endl;
	cout << "*                                             *" << endl;
	cout << "*   Fait par PARRENO Florian et MARTINEZ PJ   *" << endl;
	cout << "*                                             *" << endl;
	cout << "*                     Merci!                  *" << endl;
	cout << "***********************************************" << endl;
	return 0;
}

