#include <iostream>
#include <cstdlib>
#include "Exception.h"
using namespace std;
#include "LecteurPhraseAvecArbre.h"

void executionTest(int fichier) {
	string fichiers[8] = {	"testSelon.txt", "testSi.txt", "testPour.txt",
							"testTantque.txt", "testFaire.txt", "testInst.txt",
							"testTraductionAda.txt", "testTraductionPhp.txt" };
	int sousMenu(0);
	try{
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
					cout << endl << "***Affichage de l'arbre***" << endl;
					lp.getArbre()->afficher();
					break;
				case 2:
					cout << endl << "***Execution du programme***" << endl;
					lp.getArbre()->getValeur();
					break;
				case 3:
					cout << endl << "***Traduction en ADA***" << endl;
					lp.translateAda();
					break;
				case 4:
					cout << endl << "***Traduction en PHP***" << endl;
					lp.translatePhp();
					break;
				case 0:
					break;
			}
			cout << endl;
		} while (sousMenu != 0);
	} 
	catch (Erreur e) {
        cout << e.what() << endl;
	}
    catch (MauvaisSymbole e) {
        cout << e.what() << endl;
    }
}

int main(void) {
	
	int menuTest(0);
	
	do {
		cout << "***********Menu Principal***********" << endl;
		cout << "1. Test selon" << endl;
		cout << "2. Test si" << endl;
		cout << "3. Test pour" << endl;
		cout << "4. Test tant que" << endl;
		cout << "5. Test faire jusqua" << endl;
		cout << "6. Test instructions supplémentaires" << endl;
		cout << "7. Test traduction ADA" << endl;
		cout << "8. Test traduction PHP" << endl;
		cout << "0. Quitter" << endl;
		cout << "************************************" << endl;
		do {
			cout << "Choix ? (de 0 à 8) ";
			cin >> menuTest;
		} while (menuTest < 0 or menuTest > 8);
		system("clear");
		if (menuTest != 0)
			executionTest(menuTest-1);
	} while (menuTest != 0);
	cout << "**** Fin du Programme interpreteur, Merci ****" << endl;
	return 0;
}

