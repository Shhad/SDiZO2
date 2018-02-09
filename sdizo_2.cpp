// sdizo_2.cpp : Defines the entry point for the console application.
//

//Program byl pisany samodzielnie
//Podczas jego pisania korzystalem z materialow zawartych na stronach:
//www.pl.wikibooks.org
//www.eduinf.waw.pl
//http://www.cplusplus.com
//www.cpp0x.pl

//Autor:
//Filip Baszak



//Przyfatne informacje:
//gestosc - stosunek wierzcholkow do krawedzi
#include "stdafx.h";
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <list>
#include <chrono>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>


using namespace std;


//Musimy zaprogramowac grafy w pameici komputera w formie macierzy incydencji oraz listy nastepnikow/poprzednikow
//Dodatkowo mamy zrealizowac te same operacje na 2 formach reprezentacji grafow; algorytmy: Prima, Kruskala, Dijkstry, Forda-Bellmana, Forda Fulkersona
//Potrzebujemy 2 klasy ktore beda reprezentowaly podejscie reprezentacji grafu
//Dodatkwoo przyda³aby sie klasa MENU
//Oraz klasa ktora bedzie generowala dane(albo dwie klasy)

class IncidenceMatrix {
public:
	IncidenceMatrix(); //konstruktor
	IncidenceMatrix(IncidenceMatrix & model); //konstruktor kopiujacy
	ifstream file; //plik z ktorego odczytujemy dane
	short vertices; //liczba wierzcholkow
	short edges; //liczba krawedzi
	signed short **matrix; //macierz incydencji
	short **edgesWeight; //wskaznik do tablicy pomocniczej zawierajacej informacje o krawedziach
	short *alreadyWasSaved;
	void printMatrix(); //metoda wypisujaca macierz
	void menu(); //metoda zarzadzajaca struktura
	void createMatrix(); //metoda tworzaca macierz z danych z pliku
	void kruskalMST(); //algorytm Kruskala
	int resize(list <int> x); //metoda pomocnicza do algorytmu Kruskala
	void primMST(int ver); //algorytm Prima
	void nextEdge(list<int> &x, bool *y, bool &czy); //metoda pomocnicza do algorytmu Prima
	void dijkstra(int ver); //algorytm Dijkstry
	void fb(int ver); //algorytm Forda - Bellmana
	void ff(int &path, int *x, int stop, short **y); //metoda pomocnicza do algorytmu Forda - Fulkersona DFS
	void fff(int &path, int x[], int stop, short **y); //metoda pomocnicza do algorytmu Forda - Fulkersona BFS
	void ffDFS(int start, int stop); //algorytm Forda - Fulkersona za pomoca DFS
	void ffBFS(int start, int stop); //algorytm Forda - Fulkersona za pomoca BFS
	bool findCycle(list <int> x, int verticle2); //metoda szykujaca cyklu w liscie krawedzi
	void doRandMatrix(); //metoda tworzaca graf losowo
	void doRandMatrix(int ver, int gestosc); //metoda tworzaca graf losowo
};

IncidenceMatrix::IncidenceMatrix() {
	matrix = NULL;
	vertices = 0;
	edges = 0;
	//size = 0;
	createMatrix();
	//menu();
}

void IncidenceMatrix::menu() { //Metoda zarzadzajaca dzialaniem i operacjach algorytmicznych na strukturze 
	int liczba;
	short exit = 0;
	short choise;
	//int index = 0;
	int number = 0;
	while (exit == 0) {
		if (exit == 1) break;
		cout << "Co chcesz zrobic?\n";
		cout << "1. Wczytaj dane z pliku. \n";
		cout << "2. Wygeneruj dane losowo. \n";
		cout << "3. Algorytm Prima. \n";
		cout << "4. Algorytm Kruskala. \n";
		cout << "5. Algorytm Dijkstry. \n";
		cout << "6. Algorytm Forda-Bellmana. \n";
		cout << "7. Algorytm Forda Fulkersona. \n";
		cout << "8. Wypisz macierz. \n";
		cout << "9. Wroc do poprzedniego menu. \n";
		cout << "10. Testy. \n";
		cin >> choise;
		if (!cin) {
			cin.clear();
			cin.ignore();
		}
		switch (choise) {

		case 1:
		{
			cout << "Wczytuje dane z pliku: 'dane.txt'" << endl;
			createMatrix();
			cout << endl;
			break;
		}

		case 2:
		{
			doRandMatrix();
			break;
		}
		case 3:
		{
			cout << endl;
			if (matrix == NULL) {
				cout << "Macierz jest pusta! \n";
				cout << "Wczytaj dane do amcierzy!\n";
				break;
			}
			primMST(0);
			break;
		}
		case 4:
		{
			cout << endl;
			if (matrix == NULL) {
				cout << "Macierz jest pusta! \n";
				cout << "Wczytaj dane do amcierzy!\n";
				break;
			}
			kruskalMST();
			break;
		}
		case 5:
		{
			cout << endl;
			if (matrix == NULL) {
				cout << "Macierz jest pusta! \n";
				cout << "Wczytaj dane do amcierzy!\n";
				break;
			}
			cout << endl;
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			if (liczba < 0 || liczba >= vertices) {
				cout << "Niepoprawny wierzcholek!\n";
			}
			else {
				dijkstra(liczba);
			}
			break;
		}
		case 6:
		{
			cout << endl;
			if (matrix == NULL) {
				cout << "Macierz jest pusta! \n";
				cout << "Wczytaj dane do amcierzy!\n";
				break;
			}
			cout << endl;
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			if (liczba < 0 || liczba >= vertices) {
				cout << "Niepoprawny wierzcholek!\n";
			}
			else {
				fb(liczba);
			}
			break;
		}
		case 7:
		{
			cout << endl;
			if (matrix == NULL) {
				cout << "Macierz jest pusta! \n";
				cout << "Wczytaj dane do amcierzy!\n";
				break;
			}
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			cout << "Do jakiego wierzcholka chcesz dojsc?\n";
			cin >> number;
			ffDFS(liczba, number);
			number = 0;
			liczba = 0;
			break;
		}
		case 8:
		{
			cout << "Maceirz incydencji: \n";
			printMatrix();
			break;
		}
		case 9:
		{
			cout << "Wychodzisz ze struktury: MACIERZ INCYDENCJI. \n";
			for (int i = 0; i < vertices; i++) {
				delete[] matrix[i];
			}
			delete[] matrix;
			exit = 1;
			break;
		}
		case 10:
		{
			//tests erased
			cout << "Testy rozpoczete!\n";
			break;
		}
		default:
		{
			cout << "Wybrales zla opcje! \n";
			break;
		}

		}
	}
}

IncidenceMatrix::IncidenceMatrix(IncidenceMatrix & model) {

}

void IncidenceMatrix::createMatrix() {
	string x;
	string y;
	string z;
	string line;
	istringstream iss;
	string path = "dane.txt";
	file.open(path.c_str());
	if (file.good()) {
		getline(file, line);
		istringstream iss(line);
		//iss.str(line);
		iss >> x;
		iss >> y;
		edges = stoi(x);
		vertices = stoi(y);
		alreadyWasSaved = new short[edges];
		matrix = new signed short*[vertices];
		edgesWeight = new short*[edges];
		for (int i = 0; i < vertices; i++) {
			matrix[i] = new signed short[edges];
		}
		for (int i = 0; i < vertices; i++) {
			for (int j = 0; j < edges; j++) {
				matrix[i][j] = 0;
			}
		}
		for (int i = 0; i < edges; i++) {
			edgesWeight[i] = new short[3];
		}
		int counter = 0;
		int verticeP;
		int verticeK;
		signed short weight;
		
		while (counter<edges) {
			getline(file, line);
			istringstream iss(line);
			iss >> x;
			iss >> y;
			iss >> z;
			verticeP = stoi(x);
			verticeK = stoi(y);
			weight = stoi(z);

			matrix[verticeP][counter] = 1;
			matrix[verticeK][counter] = -1;
			edgesWeight[counter][0] = verticeP;
			edgesWeight[counter][1] = verticeK;
			edgesWeight[counter][2] = weight;
			counter++;
		}
		cout << "Udalo sie utworzyc macierz. \n";
	}
	else {
		cout << "Nie udalo sie utworzyc macierzy ;c";
		exit(100);
	}
	file.close();
}

void IncidenceMatrix::printMatrix() {
	cout << endl;
	cout << "   ";
	for (int i = 0; i < edges; i++) {
		if (i > 9) {
			cout << i << "  ";
		}
		else {
			cout << i << "   ";
		}
	}
	cout << endl;
	cout << endl;
	for (int i = 0; i < vertices; i++) {
		cout << i << " ";
		for (int j = 0; j < edges; j++) {
			if (matrix[i][j] ==-1) {
				cout << matrix[i][j];
			}
			else {
				cout <<" " << matrix[i][j];
			}
			
			cout << "  ";
		}
		cout << endl;
	}
	cout << "Wagi krawedzi: " << endl;
	for (int i = 0; i < edges; i++) {
		cout << "Krawedz " << i << " : " << edgesWeight[i][2] << endl;
	}
	cout << endl;
}

void IncidenceMatrix::kruskalMST() {
	//short **sortedWeight;            //Utworzenie macierzy pomocniczej posortowanych krawedzi
	//int size = edges;
	int p = 0;
	int k = 0;
	int w = 0;
	bool czyPelne = true;
	for (int i = 0; i < edges; i++) {
		alreadyWasSaved[i] = -1;
	}

	int counter = 0;
	short min = 0;
	bool wasnt = true;
	for (int j = 0; j < edges;) {
		wasnt = true;
		for (int i = j; i < edges;) {
			if ((edgesWeight[i][2]) < (edgesWeight[j][2])) {
					p = edgesWeight[i][0];
					k = edgesWeight[i][1];
					w = edgesWeight[i][2];
					edgesWeight[i][0] = edgesWeight[j][0];
					edgesWeight[i][1] = edgesWeight[j][1];
					edgesWeight[i][2] = edgesWeight[j][2];
					edgesWeight[j][0] = p;
					edgesWeight[j][1] = k;
					edgesWeight[j][2] = w;
				}
			i++;
			}
		j++;
	}

	list <int> tree;
	list <int> help;
	list <int> help2;
	//nasza glowna petla
	bool *visited;
	visited = new bool[vertices];
	for (int i = 0; i < vertices; i++) {
		visited[i]=false;
	}
	bool visi = true;
	bool nieCounter2;
	int counter2 = 0;
	tree.push_front(counter2);
	visited[edgesWeight[counter2][0]] = true;
	visited[edgesWeight[counter2][1]] = true;
	counter2++;
	int counterFalse=0;
	while (visi) {
		counter = 0;
		if (counter2 == edges) {
			visi = false;
			cout << "Nie mozna utworzyc pelnego drzewa MST!" << endl;
			czyPelne = false;
		}
		//----------------------------------------------------------------------------
		help = tree;
		help2 = tree;
		if (counter2 != edges) {
			if (findCycle(help, edgesWeight[counter2][1]) && findCycle(help2, edgesWeight[counter2][0])) {
				//cout << "Dodano krawedz: " << counter2 << endl;
				tree.push_front(counter2);
				visited[edgesWeight[counter2][0]] = true;
				visited[edgesWeight[counter2][1]] = true;
				counter2++;
				counterFalse++;

			}
		}
		
		if (counter2 != edges) {
			if ((!findCycle(help, edgesWeight[counter2][1])) || (!findCycle(help2, edgesWeight[counter2][0]))) {
				//cout << "Nie mozna dodac krawedzi" << endl;
				counter2++;
				counterFalse++;
			}
		}
		
		help.clear();
		help2.clear();
		//----------------------------------------------------------------------------
		if (counterFalse == vertices) {
			cout << "Nie mozna utworzyc pelnego drzewa MST!" << endl;
			czyPelne = false;
			visi = false;
		}
		for (int i = 0; i < vertices; i++) {
			if (visited[i]) {
				counter++;

			}
		}
		//cout << "Odwiedzonych wierzcholkow: " << counter << endl;
		if (counter == (vertices)) {
			visi = false;
		}
		//Sleep(1000);
	}
	cout << "Wyszlo z whilea" << endl;
	cout << endl << "Oto nasze minimalne drzewo rozpinajace: " << endl;
	list<int>::iterator it;
	it = tree.begin();
	for (int i = 0; i < tree.size(); i++) {
		counter2 = *it;
		cout << edgesWeight[counter2][0] << " --- " << edgesWeight[counter2][1] << "  |  waga: " << edgesWeight[counter2][2] << endl;
		it++;
	}
	if (!czyPelne) {
		cout << "Oto wierzcholki do ktorych nie doszlismy: \n";
		for (int i = 0; i < vertices; i++) {
			if (!visited[i]) {
				cout << i << endl;
			}
		}
	}
	cout << "-------------------------------------" << endl;
	delete[] visited;
	return;
}

bool IncidenceMatrix::findCycle(list <int> x, int verticle2) {
	int motherV = verticle2;
	int size = x.size();
	list<int>::iterator it;
	it = x.begin();
	int number;
	//cout << "Wchodzi do findCycle" << endl;
	for (int i = 0; i < size; i++) {
		//cout << i << endl;
		number = *it;
		
		if ((edgesWeight[number][0]==verticle2) || (edgesWeight[number][1] == verticle2)) {
			
			i = -1;
			x.erase(it);
			size = resize(x);
			it = x.begin();
			if (size == 0) {
				//cout << "Zwraca true" << endl;
				return true;
			}
			if ((edgesWeight[number][0] == verticle2)) {
				verticle2 = edgesWeight[number][1];
				if ((verticle2 == motherV)) {
					//cout << "Zwraca false" << endl;
					return false;
				}
			}
			else {
				verticle2 = edgesWeight[number][0];
				if ((verticle2 == motherV)) {
					//cout << "Zwraca false" << endl;
					return false;
				}
			}
		}
		else {
			it++;
		}
		
	}
	//cout << "Zwraca true" << endl;
	return true;
}

int IncidenceMatrix::resize(list <int> x) {
	return x.size();
}

void IncidenceMatrix::primMST(int ver) {
	bool *visited = new bool[vertices];
	bool visi = true;
	bool dodane = false;
	int counterFalse = 0;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
	}
	visited[ver] = true;
	int counter;
	//int counter2=0;
	list <int> tree;
	//list <int> help;
	//list <int> help2;
	while (visi) {
		counter = 0;
		//----------------------------------------------------------------------------
		nextEdge(tree, visited,dodane);
		if (!dodane) {
			counterFalse++;
		}
		//----------------------------------------------------------------------------
		for (int i = 0; i < vertices; i++) {
			if (visited[i]) {
				counter++;

			}
		}
		if (counterFalse > (edges - tree.size())) {
			visi = false;
		}
		//cout << "Odwiedzonych wierzcholkow: " << counter << endl;
		if (counter == (vertices)) {
			visi = false;
		}
		//Sleep(1000);
	}
	counter = 0;
	//cout << "Wyszlo z whilea" << endl;
	cout << endl << "Oto nasze minimalne drzewo rozpinajace: " << endl;
	list<int>::iterator it;
	it = tree.begin();
	for (int i = 0; i < tree.size(); i++) {
		counter = *it;
		cout << edgesWeight[counter][0] << " --- " << edgesWeight[counter][1] << "  |  waga: " << edgesWeight[counter][2] << endl;
		it++;
	}


	delete[] visited;
}

void IncidenceMatrix::nextEdge(list<int> &x, bool *y, bool &czy) {
	int min = 2147483647;
	int minE;
	bool zmianaMinE = false;
	for (int i = 0; i < vertices; i++) {
		if(!y[i]){
			minE = i;
		}
	}

	for (int i = 0; i < vertices; i++) {
		if (y[i]) {
			for (int j = 0; j < edges; j++) {
				if (((edgesWeight[j][0] == i) || (edgesWeight[j][1] == i))) {
					if ((edgesWeight[j][0] == i) && (!y[edgesWeight[j][1]]) && (edgesWeight[j][2]<min)) {
						minE = j;
						min = edgesWeight[minE][2];
						zmianaMinE = true;
					}
					if ((edgesWeight[j][1] == i) && (!y[edgesWeight[j][0]]) && (edgesWeight[j][2]<min)) {
						minE = j;
						min = edgesWeight[minE][2];
						zmianaMinE = true;
					}
				}

			}
		}
	}
	if (zmianaMinE) {
		if (y[edgesWeight[minE][0]]) {
			y[edgesWeight[minE][1]] = true;
		}
		else {
			y[edgesWeight[minE][0]] = true;
		}
		x.push_front(minE);
		czy = true;
		return;
	}
	else {
		for (int i = 0; i < edges; i++) {
			if (((edgesWeight[i][0] == minE) || (edgesWeight[i][1] == minE))) {

				y[edgesWeight[i][1]] = true;
				y[edgesWeight[i][0]] = true;
				x.push_front(i);
				czy = true;
				return;
			}

		}
	}
	czy = false;
	return;
}

void IncidenceMatrix::dijkstra(int ver) {
	int maxint = 2147483647;
	int *pastV = new int[vertices];
	int *costV = new int[vertices];
	bool *visited = new bool[vertices];
	bool firstLoop = true;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
		costV[i] = maxint;
	}
	cout << ver << endl;
	int stackPointer = 0;
	costV[ver] = 0;
	visited[ver] = true;
	for (int j = 0; j < vertices; j++) {
		firstLoop = true;
		for (int i = ver; i<vertices; i++) {
			for (int k = 0; k < vertices; k++) {
				
				if ((!visited[k]) && (costV[k] < costV[i])) {
					ver = k;
					//cout << "if wszedl" << endl;
				}
			}
			if (firstLoop && (i == vertices - 1))
			{
				i = -1;
				firstLoop = false;
			}
		}
		cout << ver << endl;
		visited[ver] = true;
		for (int i = 0; i < edges; i++) {
			if (edgesWeight[i][0] == ver) {
				if ((!visited[edgesWeight[i][1]]) && (costV[edgesWeight[i][1]] >(costV[ver] + edgesWeight[i][2]))) {
					costV[edgesWeight[i][1]] = (costV[ver] + edgesWeight[i][2]);
					pastV[edgesWeight[i][1]] = ver;
					
				}
			}
		}
	}
	int destination;
	cout << endl;
	cout << "Dostepne wierzcholki: ";
	for (int i = 0; i < vertices; i++) {
		if (pastV[i] != -1) {
			cout << i << ", ";
		}
	}
	cout << endl;
	cout << "Do jakiego wierzcholka chcesz sie dostac?\n";
	cin >> destination;
	if (destination < 0 || destination >= vertices) {
		cout << "Nie ma takiego wierzcholka!\n";
		return;
	}
	stackPointer = 0;
	
	int *path = new int[vertices];
	cout << pastV[destination] << endl;
	for(int i = destination; i > -1; i = pastV[i]) {
		path[stackPointer] = i;
		stackPointer++;
	}
	cout << endl;
	cout << "Sciezka to :";
	while (stackPointer) {
		stackPointer--;
		cout << path[stackPointer] << " - ";
		
	}
	cout << endl;
	cout << "Jej koszt to: " << costV[destination];
	cout << endl;

	delete[] costV;
	delete[] pastV;
	delete[] path;
	delete[] visited;
}

void IncidenceMatrix::fb(int ver) {
	bool changes;
	bool fCycle = false;
	int maxint = 2147483647;
	int *pastV = new int[vertices];
	int *costV = new int[vertices];
	int *neigbors;
	for (int i = 0; i < vertices; i++) {
		pastV[i] = -1;
		costV[i] = maxint;
	}
	cout << ver << endl;
	int stackPointer = 0;
	costV[ver] = 0;
	int neighbor = -1;
	for (int i = 1; i <vertices; i++) { //petla musi przejsc n-1 raz, gdzie n oznacza liczbe wierzcholkow
		changes = true;
		fCycle = true;
		for (short j = ver; j<vertices; ) {   // Przechodzimy przez kolejne wierzcho³ki grafu
			for (int x = 0; x < edges ;  ) { // Przegl¹damy listê krawedzi aby znalezc sasiadow wierzcholka j
				if ((edgesWeight[x][0] == j) && (costV[edgesWeight[x][1]] >(costV[j] + edgesWeight[x][2]))) {
					if (costV[j] != maxint) {
						changes = false;           // Jest zmiana w tablicach cost i past
						costV[edgesWeight[x][1]] = costV[j] + edgesWeight[x][2]; // wpisujemy koszt przejscia
						pastV[edgesWeight[x][1]] = j;           // Poprzednikiem s¹siada bêdzie wierzcholek j
					}
				}
				x++;
			}
			if (fCycle && (j == vertices - 1)) {
				j = -1;
				fCycle = false;
			}
			j++;
		}
		if (changes) maxint=0;
		if (maxint == 0) break;
	}
	for (int i = 0; i < vertices; i++) {
		cout << pastV[i] << " : " << costV[i] << endl;
	}
	for (int i = 0; i < vertices; i++) {
		for (int x = 0; x < edges; x++) {
			if ((edgesWeight[x][0] == i) && (costV[edgesWeight[x][1]] >(costV[edgesWeight[x][0]] + edgesWeight[x][2]))) {
				fCycle = true;
			}
		}
	}
	cout << endl;
	if (fCycle) {
		cout << "Mamy do czynienia  zcyklem ujemnym badz z neidostepnym wierzcholkiem.\n";
		return;
	}
	int destination;
	cout << endl;
	cout << "Dostepne wierzcholki: ";
	for (int i = 0; i < vertices; i++) {
		if (pastV[i] != -1) {
			cout << i << ", ";
		}
	}
	cout << endl;
	cout << "Do jakiego wierzcholka chcesz sie dostac?\n";
	cin >> destination;
	if (destination < 0 || destination >= vertices) {
		cout << "Nie ma takiego wierzcholka!\n";
		return;
	}
	stackPointer = 0;

	int *path = new int[vertices];
	cout << pastV[destination] << endl;
	for (int i = destination; i > -1; i = pastV[i]) {
		path[stackPointer] = i;
		stackPointer++;
	}
	cout << endl;
	cout << "Sciezka to :";
	while (stackPointer) {
		stackPointer--;
		cout << path[stackPointer] << " - ";

	}
	cout << endl;
	cout << "Jej koszt to: " << costV[destination];
	cout << endl;

	delete[] costV;
	delete[] pastV;
	delete[] path;
	//delete[] visited;
}

void IncidenceMatrix::ff(int &path, int *x, int stop, short **y) {
	int stopV = 0;
	int min = 222222;
	int *edge;
	int help=stop;
	while(help!=(-2)) {
		help = x[help];
		stopV++;
	}
	int z = stopV - 1;
	edge = new int[stopV];
	stopV = stop;
	int counter = 0;
	int counterFalse = 0;
	while (stopV != -2) {
		for (int i = 0; i < edges; i++) {
			if (y[i][0] == x[stopV] && y[i][1] == stopV) {
				if ((min > y[i][2]) && (y[i][2] > 0)) {
					min = y[i][2];
				}
				counterFalse = 0;
				edge[counter] = i;
				counter++;
				stopV = x[stopV];
			//	cout << stopV << endl;
				if (x[stopV] == -2) {
					stopV = -2;
				}
				break;
			}
			
			counterFalse++;
		}
		//cout << "while w ff" << endl;
	}
	if (counterFalse >= edges - z) {
		return;
	}
	//cout << "min wynosi: " << min << endl;
	for (int i = 0; i < z; i++) {
		y[edge[i]][3] = min;
		y[edge[i]][2] = y[edge[i]][2] - min;
	}
	path = path + min;
	delete[] edge;
	//cout << "wychodzi z ff" << endl;
}

void IncidenceMatrix::ffDFS(int start, int stop) {
	//int maxint = 2147483647;
	int sciezka = 0;
	bool done = false;
	bool change = true;
	int *pastV;
	pastV = new int[vertices];
	short **edg;
	edg = new short *[edges];
	for (int i = 0; i < edges; i++) {
		edg[i] = new short[4];
		edg[i][0] = edgesWeight[i][0];
		edg[i][1] = edgesWeight[i][1];
		edg[i][2] = edgesWeight[i][2];
		edg[i][3] = 0;
	}
	bool *visited = new bool[vertices];
	bool loop = true;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
	}
	int currentV = start;
	int counter = 0;
	int counterFalse = 0;
	visited[start] = true;
	pastV[start] = -2;
	//cout << "currentV: " << currentV << endl;
	if (start == stop) {
		loop = false;
	}
	int stopH = stop;
	int counterBreak = 0;
	while (loop) {
		change = false;
		for (int i = 0; i < edges; i++) {
			if ((edg[i][0] == currentV) && (!visited[edg[i][1]]) && (edg[i][2]>0)) {
				currentV = edg[i][1];
				pastV[currentV] = edg[i][0];
				visited[edg[i][1]] = true;
				change = true;
				//cout << "zmiana" << endl;
				//cout << "currentV: " << currentV << endl;
				//cout << "past od currentV: " << pastV[currentV] << endl;
				//cout << "waga krawedzi: " << edg[i][2] << endl;
				break;
			}
			
		}
		//Sleep(10000);
		if (currentV == stop) {
			//Sleep(1000);
			ff(sciezka,pastV,stopH,edg);
			if (stopH == -1) {
				counterBreak++;
			}
			for (int j = 0; j < vertices; j++) {
				pastV[j] = -1;
				visited[j] = false;
			}
			visited[start] = true;
			pastV[start] = -2;
			change = true;
			currentV = start;
			done = true;
		}
		if (!change) {
			//cout << "cofamy sie!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			//cout << "currentV: " << currentV << endl;
			//cout << "past od currentV: " << pastV[currentV] << endl;
			counter = pastV[currentV];
			pastV[currentV] = -1;
			currentV = counter;
			//cout << "currentV: " << currentV << endl;
			//cout << "past od currentV: " << pastV[currentV] << endl;
			counterFalse++;
		}
		
		if ((currentV == (-2))) {
			loop = false;
		}
		if (counterBreak == vertices) {
			loop = false;
		}
	}
	if (!done) {
		cout << "Sciezka nie istnieje z podanego wierzcholka do wierzcholka podanego.\n";
		return;
	}
	cout << "Sciezka maksymalnego przeplywu wynosi: " << sciezka << endl;
	for (int i = 0; i < edges; i++) {
		delete edg[i];
	}
	delete[] edg;
	delete[] visited;
}

void IncidenceMatrix::fff(int &path, int x[], int stop, short **y) {
	int stopV = 0;
	int min = 222222;
	int *edge;
	int help = stop;;
	for (int i = 0; help!=(-2); i++) {
		help = x[help];
		stopV++;
	}
	int z = stopV-1;
	edge = new int[stopV];
	stopV = stop;
	int counter = 0;
	while (stopV != -2) {
		for (int i = 0; i < edges; i++) {
			if (y[i][0] == x[stopV] && y[i][1] == stopV) {
				if (min > y[i][2]) {
					min = y[i][2];
				}
				edge[counter] = i;
				counter++;
				stopV = x[stopV];
				//cout << stopV << endl;
				break;
			}
			if (x[stopV] == -2) {
				stopV = -2;
			}
		}
		//cout << "while w ff" << endl;
	}
	for (int i = 0; i < z; i++) {
		y[edge[i]][3] = min;
		y[edge[i]][2] = y[edge[i]][2] - min;
	}
	path = path + min;
	//cout << "wychodzi z ff" << endl;
}

void IncidenceMatrix::ffBFS(int start, int stop) {
	//int maxint = 2147483647;
	int sciezka = 0;
	bool done = false;
	bool change = true;
	int *pastV;
	pastV = new int[vertices];
	short **edg;
	edg = new short *[edges];
	for (int i = 0; i < edges; i++) {
		edg[i] = new short[4];
		edg[i][0] = edgesWeight[i][0];
		edg[i][1] = edgesWeight[i][1];
		edg[i][2] = edgesWeight[i][2];
		edg[i][3] = 0;
	}
	bool *visited = new bool[vertices];
	bool loop = true;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
	}
	int currentV = start;
	int counter = 0;
	int counterFalse = 0;
	visited[start] = true;
	pastV[start] = -2;
	//cout << "currentV: " << currentV << endl;
	if (start == stop) {
		loop = false;
	}
	while (loop) {
		change = false;
		for (int j = 0; j<vertices; j++) {
			if (visited[j]) {
				for (int i = 0; i<edges; i++) {
					//cout << i << "ta petla w " << j << endl;
					if ((edg[i][0] == j) && (!visited[edg[i][1]]) && (edg[i][2]>0)) {
						//currentV = edg[i][1];
						pastV[edg[i][1]] = edg[i][0];
						//visited[edg[i][1]] = true;
						change = true;
						//cout << "zmiana" << endl;
						//cout << "currentV: " << currentV << endl;
						//cout << "past od currentV: " << pastV[currentV] << endl;
						//i = edges;
						counterFalse++;
					}

				}
			}
			
		}
		for (int i = 0; i < vertices; i++) {
			if (!visited[i] && (pastV[i] != -1)) {
				visited[i] = true;
				currentV = i;
				if (currentV == stop) {
					fff(sciezka, pastV, stop, edg);
					for (int j = 0; j < vertices; j++) {
						pastV[j] = -1;
						visited[j] = false;
					}
					visited[start] = true;
					pastV[start] = -2;
					change = true;
					currentV = start;
					done = true;
					i = vertices;
				}
			}
		}
		
		
		if (!change) {
			counterFalse++;
		}
		if (counterFalse > edges) {
			loop = false;
		}

		if (currentV == (-2)) {
			loop = false;
		}
	}
	if (!done) {
		cout << "Sciezka nie istnieje z podanego wierzcholka do wierzcholka podanego.\n";
		return;
	}
	cout << "Sciezka maksymalnego przeplywu wynosi: " << sciezka << endl;
	for (int i = 0; i < edges; i++) {
		delete edg[i];
	}
	delete[] edg;
	delete[] visited;
}

void IncidenceMatrix::doRandMatrix() {
	for (int i = 0; i < vertices; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	for (int i = 0; i < edges; i++) {
		delete[] edgesWeight[i];
	}
	delete[] edgesWeight;
	int gestosc;
	cout << "Podaj liczbe wierzcholkow: \n";
	cin >> vertices;
	cout << "Podaj gestosc krawedzi :\n";
	cin >> gestosc;
	edges = (100 * vertices) / gestosc;
	alreadyWasSaved = new short[edges];
	matrix = new signed short*[vertices];
	edgesWeight = new short*[edges];
	for (int i = 0; i < vertices; i++) {
		matrix[i] = new signed short[edges];
	}
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < edges; j++) {
			matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < edges; i++) {
		edgesWeight[i] = new short[3];
	}
	int counter = 0;
	int verticeP;
	int verticeK;
	int znak;
	signed short weight;
	srand(time(NULL));
	while (counter<edges) {
		verticeP = (rand() % vertices) + 0;
		verticeK = (rand() % vertices) + 0;
		while (verticeK == verticeP) {
			verticeK = (rand() % vertices) + 0;
		}
		weight = (rand() % 10) + 0;
		/*znak = (rand() % 2) + 0;
		if (znak == 1) {
			weight = 0 + weight;
		}
		else {
			weight = 0 - weight;
		}*/
		
		matrix[verticeP][counter] = 1;
		matrix[verticeK][counter] = -1;
		edgesWeight[counter][0] = verticeP;
		edgesWeight[counter][1] = verticeK;
		edgesWeight[counter][2] = weight;
		counter++;
	}
}

void IncidenceMatrix::doRandMatrix(int ver, int gestosc) {
	for (int i = 0; i < vertices; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	for (int i = 0; i < edges; i++) {
		delete[] edgesWeight[i];
	}
	delete[] edgesWeight;

	vertices = ver;
	edges = (100 * vertices) / gestosc;
	alreadyWasSaved = new short[edges];
	matrix = new signed short*[vertices];
	edgesWeight = new short*[edges];
	for (int i = 0; i < vertices; i++) {
		matrix[i] = new signed short[edges];
	}
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < edges; j++) {
			matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < edges; i++) {
		edgesWeight[i] = new short[3];
	}
	int counter = 0;
	int verticeP;
	int verticeK;
	int znak;
	signed short weight;
	srand(time(NULL));
	while (counter<edges) {
		verticeP = (rand() % vertices) + 0;
		verticeK = (rand() % vertices) + 0;
		while (verticeK == verticeP) {
			verticeK = (rand() % vertices) + 0;
		}
		weight = (rand() % 10) + 0;
		/*znak = (rand() % 2) + 0;
		if (znak == 1) {
			weight = 0 + weight;
		}
		else {
			weight = 0 - weight;
		}*/

		matrix[verticeP][counter] =1;
		matrix[verticeK][counter] =-1;
		edgesWeight[counter][0] = verticeP;
		edgesWeight[counter][1] = verticeK;
		edgesWeight[counter][2] = weight;
		counter++;
	}
}

struct NList {//Struktura listy nastepnikow
	NList *next;
	int v;   //numer wierzcholka obecnego
	int w;   //waga krawedzi
};

struct NeList {//tworzymy nowa strukture w celu ulatwienia sobie zaimplementowania algorytmu Forda - fulkersona
	NeList *next;
	int v;   //numer wierzcholka obecnego
	int w;   //waga krawedzi
	int p;   //przepustowosc
};

class NextList {
public:
	NextList(); //Konstruktor
	NextList(NextList & model); //Konstruktor kopiujacy
	ifstream file; //Plik z ktorego wczytujemy dane
	short vertices; //Liczba wierzcholkow grafu
	short edges;  //Liczba krawedzi grafu
	NList **lista; //lista nastepnikow
	short **edgesWeight; //wskaznik na tablice pomocnicza ktora przechowuje wiadomosci o krawedziach
	short *alreadyWasSaved;
	void printList(); //metoda wypisujaca liste nastepnikow
	void menu(); //metoda zarzadzajaca opcjami na reprezentacji grafu
	void createList(); //metoda tworzaca graf
	void kruskalMST(); //algorytm Kruskala
	int resize(list <int> x); //metoda pomocnicza dla algorytmu Kruskala
	void primMST(int ver); //algorytm Prima
	void nextEdge(list<int> &x, bool *y, bool &czy); //metoda pomocnicza dla algorytmu Prima
	void dijkstra(int ver); //algorytm Dijkstry
	void fb(int ver); //algorytm Forda-Bellmana
	bool findCycle(list <int> x, int verticle2);//metoda ktora szuka cyklu w liscie krawedzi
	void doRandList(); //metoda tworzaca graf losowo
	//void ff(int start, int stop); 
	void ff(int &path, int *x, int stop, short **y); //algorytm Forda-Fulkersona pomocniczy
	void fff(int &path, int x[], int stop, short **y); //algorytm Forda-Fulkersona pomocniczy
	void ffDFS(int start, int stop); //algorytm Forda-Fulkersona za pomoca DFS
	void ffBFS(int start, int stop); //algorytm Forda-Fulkersona za pomoca BFS
	void doRandList(int ver, int g);
};



NextList::NextList() {
	lista = NULL;
	vertices = 0;
	edges = 0;
	//size = 0;
	createList();
	//menu();
}

void NextList::menu() { //Metoda zarzadzajaca dzialaniem i operacjach algorytmicznych na strukturze 
	int liczba;
	short exit = 0;
	short choise;
	//int index = 0;
	int number = 0;
	while (exit == 0) {
		if (exit == 1) break;
		cout << "Co chcesz zrobic?\n";
		cout << "1. Wczytaj dane z pliku. \n";
		cout << "2. Wygeneruj dane losowo. \n";
		cout << "3. Algorytm Prima. \n";
		cout << "4. Algorytm Kruskala. \n";
		cout << "5. Algorytm Dijkstry. \n";
		cout << "6. Algorytm Forda-Bellmana. \n";
		cout << "7. Algorytm Forda Fulkersona. \n";
		cout << "8. Wypisz liste. \n";
		cout << "9. Wroc do poprzedniego menu. \n";
		cout << "10. Rozpocznij testy. \n";
		cin >> choise;
		if (!cin) {
			cin.clear();
			cin.ignore();
		}
		switch (choise) {

		case 1:
		{
			cout << "Wczytuje dane z pliku: 'dane.txt'" << endl;
			createList();
			cout << endl;
			break;
		}

		case 2:
		{
			doRandList();
			break;
		}
		case 3:
		{
			cout << endl;
			if (lista == NULL) {
				cout << "Lista jest pusta! \n";
				cout << "Wczytaj dane do listy!\n";
				break;
			}
			primMST(0);
			break;
		}
		case 4:
		{
			cout << endl;
			if (lista == NULL) {
				cout << "Lista jest pusta! \n";
				cout << "Wczytaj dane do listy!\n";
				break;
			}
			kruskalMST();
			break;
		}
		case 5:
		{
			cout << endl;
			if (lista == NULL) {
				cout << "Lista jest pusta! \n";
				cout << "Wczytaj dane do listy!\n";
				break;
			}
			cout << endl;
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			if (liczba < 0 || liczba >= vertices) {
				cout << "Niepoprawny wierzcholek!\n";
			}
			else {
				dijkstra(liczba);
			}
			break;
		}
		case 6:
		{
			cout << endl;
			if (lista == NULL) {
				cout << "Lista jest pusta! \n";
				cout << "Wczytaj dane do listy!\n";
				break;
			}
			cout << endl;
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			if (liczba < 0 || liczba >= vertices) {
				cout << "Niepoprawny wierzcholek!\n";
			}
			else {
				fb(liczba);
			}
			break;
		}
		case 7:
		{
			cout << endl;
			if (lista == NULL) {
				cout << "Lista jest pusta! \n";
				cout << "Wczytaj dane do listy!\n";
				break;
			}
			cout << "Z jakiego wierzcholka rozpoczac?\n";
			cin >> liczba;
			cout << "Do jakiego wierzcholka chcesz dojsc?\n";
			cin >> number;
			ffDFS(liczba, number);
			number = 0;
			liczba = 0;
			break;
		}
		case 8:
		{
			cout << "Lista nastepnikow: \n";
			printList();
			break;
		}
		case 9:
		{
			cout << "Wychodzisz ze struktury: LISTA NASTEPNIKOW. \n";
			for (int i = 0; i < vertices; i++) {
				delete[] lista[i];
			}
			delete[] lista;
			exit = 1;
			break;
		}
		case 10:
		{
			//tests erased
			break;
		}
		default:
			cout << "Wybrales zla opcje! \n";
			break;
		}

	}
}

NextList::NextList(NextList & model) {

}

void NextList::createList() {
	string x;
	string y;
	string z;
	string line;
	istringstream iss;
	string path = "dane.txt";
	NList *help;
	file.open(path.c_str());
	if (file.good()) {
		getline(file, line);
		istringstream iss(line);
		//iss.str(line);
		iss >> x;
		iss >> y;
		edges = stoi(x);
		vertices = stoi(y);
		alreadyWasSaved = new short[edges];
		lista = new NList*[vertices];
		edgesWeight = new short*[edges];
		for (int i = 0; i < edges; i++) {
			edgesWeight[i] = new short[3];
		}
		for (int i = 0; i < vertices; i++) {
			lista[i] = NULL;
		}
		int counter = 0;
		int verticeP;
		int verticeK;
		signed short weight;

		while (counter<edges) {
			getline(file, line);
			istringstream iss(line);
			iss >> x;
			iss >> y;
			iss >> z;
			verticeP = stoi(x);
			verticeK = stoi(y);
			weight = stoi(z);
			help = new NList;
			help->v = verticeK;
			help->w = weight;
			help->next = lista[verticeP];
			lista[verticeP] = help;
			edgesWeight[counter][0] = verticeP;
			edgesWeight[counter][1] = verticeK;
			edgesWeight[counter][2] = weight;
			counter++;
		}
		cout << "Udalo sie utworzyc liste. \n";
	}
	else {
		cout << "Nie udalo sie utworzyc listy ;c";
		Sleep(5000);
		exit(0);
	}
	file.close();
}

void NextList::printList() {
	cout << endl;
	NList *help;
	for (int i = 0; i < vertices; i++)
	{
		cout << "Wierzcholek[" << i << "] =";
		help = lista[i];
		while (help)
		{
			cout << setw(3) << help->v;
			cout << " waga: " << help->w<<"  ||  ";
			help = help->next;
		}
		cout << endl;
	}
	cout << endl;
}

void NextList::kruskalMST() {
	short **sortedWeight;            //Utworzenie macierzy pomocniczej posortowanych krawedzi
									 //int size = edges;
	int p = 0;
	int k = 0;
	int w = 0;
	for (int i = 0; i < edges; i++) {
		alreadyWasSaved[i] = -1;
	}
	sortedWeight = new short*[edges];     //sortujemy od najmniejszej wagi do najwiekszej
	for (int i = 0; i < edges; i++) {
		sortedWeight[i] = new short[3];
	}
	int counter = 0;
	short min = 0;
	bool wasnt = true;
	for (int j = 0; j < edges;) {
		wasnt = true;
		for (int i = j; i < edges;) {
			if ((edgesWeight[i][2]) < (edgesWeight[j][2])) {
				p = edgesWeight[i][0];
				k = edgesWeight[i][1];
				w = edgesWeight[i][2];
				edgesWeight[i][0] = edgesWeight[j][0];
				edgesWeight[i][1] = edgesWeight[j][1];
				edgesWeight[i][2] = edgesWeight[j][2];
				edgesWeight[j][0] = p;
				edgesWeight[j][1] = k;
				edgesWeight[j][2] = w;
			}
			i++;
		}
		j++;
	}
	cout << endl;
	for (int i = 0; i < edges; i++) {
		cout << edgesWeight[i][0] << " : " << edgesWeight[i][1] << " : " << edgesWeight[i][2] << endl;
	}
	list <int> tree;
	list <int> help;
	list <int> help2;
	//nasza glowna petla
	bool *visited;
	bool czyPelne = true;;
	visited = new bool[vertices];
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
	}
	bool visi = true;
	int counter2 = 0;
	tree.push_front(counter2);
	visited[edgesWeight[counter2][0]] = true;
	visited[edgesWeight[counter2][1]] = true;
	counter2++;
	int counterFalse=0;
	while (visi) {
		counter = 0;
		//----------------------------------------------------------------------------
		help = tree;
		help2 = tree;
		if (counter2 == edges) {
			visi = false;
			cout << "Nie mozna utworzyc pelnego drzewa MST!" << endl;
			czyPelne = false;
		}
		if (counter2 != edges) {
			if (findCycle(help, edgesWeight[counter2][1]) && findCycle(help2, edgesWeight[counter2][0])) {
				//cout << "Dodano krawedz: " << counter2 << endl;
				tree.push_front(counter2);
				visited[edgesWeight[counter2][0]] = true;
				visited[edgesWeight[counter2][1]] = true;
				counter2++;
				counterFalse++;

			}
		}
		
		if (counter2 != edges) {
			if ((!findCycle(help, edgesWeight[counter2][1])) || (!findCycle(help2, edgesWeight[counter2][0]))) {
				//cout << "Nie mozna dodac krawedzi" << endl;
				counter2++;
				counterFalse++;
			}
		}
		help.clear();
		help2.clear();
		if (counterFalse == vertices) {
			cout << "Nie mozna utworzyc pelnego drzewa MST!" << endl;
			czyPelne = false;
			visi = false;
		}
		//----------------------------------------------------------------------------
		for (int i = 0; i < vertices; i++) {
			if (visited[i]) {
				counter++;

			}
		}
		cout << "Odwiedzonych wierzcholkow: " << counter << endl;
		if (counter == (vertices)) {
			visi = false;
		}
		//Sleep(1000);
	}
	//cout << "Wyszlo z whilea" << endl;
	cout << endl << "Oto nasze minimalne drzewo rozpinajace: " << endl;
	list<int>::iterator it;
	it = tree.begin();
	for (int i = 0; i < tree.size(); i++) {
		counter2 = *it;
		cout << edgesWeight[counter2][0] << " --- " << edgesWeight[counter2][1] << "  |  waga: " << edgesWeight[counter2][2] << endl;
		it++;
	}
	if (!czyPelne) {
		cout << "Oto wierzcholki do ktorych nie doszlismy: \n";
		for (int i = 0; i < vertices; i++) {
			if (!visited[i]) {
				cout << i << endl;
			}
		}
	}
	cout << "-------------------------------------" << endl;
	delete[] visited;
	return;
}

bool NextList::findCycle(list <int> x, int verticle2) {
	int motherV = verticle2;
	int size = x.size();
	list<int>::iterator it;
	it = x.begin();
	int number;
	//cout << "Wchodzi do findCycle" << endl;
	for (int i = 0; i < size; i++) {
		//cout << i << endl;
		number = *it;

		if ((edgesWeight[number][0] == verticle2) || (edgesWeight[number][1] == verticle2)) {

			i = -1;
			x.erase(it);
			size = resize(x);
			it = x.begin();
			if (size == 0) {
				//cout << "Zwraca true" << endl;
				return true;
			}
			if ((edgesWeight[number][0] == verticle2)) {
				verticle2 = edgesWeight[number][1];
				if ((verticle2 == motherV)) {
					//cout << "Zwraca false" << endl;
					return false;
				}
			}
			else {
				verticle2 = edgesWeight[number][0];
				if ((verticle2 == motherV)) {
					//cout << "Zwraca false" << endl;
					return false;
				}
			}
		}
		else {
			it++;
		}

	}
	//cout << "Zwraca true" << endl;
	return true;
}

int NextList::resize(list <int> x) {
	return x.size();
}

void NextList::primMST(int ver) {
	bool *visited = new bool[vertices];
	bool visi = true;
	bool dodane = false;
	int counterFalse = 0;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
	}
	visited[ver] = true;
	int counter;
	//int counter2=0;
	list <int> tree;
	//list <int> help;
	//list <int> help2;
	while (visi) {
		counter = 0;
		//----------------------------------------------------------------------------
		nextEdge(tree, visited, dodane);
		if (!dodane) {
			counterFalse++;
		}
		//----------------------------------------------------------------------------
		for (int i = 0; i < vertices; i++) {
			if (visited[i]) {
				counter++;

			}
		}
		if (counterFalse >(edges - tree.size())) {
			visi = false;
		}
		//cout << "Odwiedzonych wierzcholkow: " << counter << endl;
		if (counter == (vertices)) {
			visi = false;
		}
		//Sleep(1000);
	}
	counter = 0;
	//cout << "Wyszlo z whilea" << endl;
	cout << endl << "Oto nasze minimalne drzewo rozpinajace: " << endl;
	list<int>::iterator it;
	it = tree.begin();
	for (int i = 0; i < tree.size(); i++) {
		counter = *it;
		cout << edgesWeight[counter][0] << " --- " << edgesWeight[counter][1] << "  |  waga: " << edgesWeight[counter][2] << endl;
		it++;
	}


	delete[] visited;
}

void NextList::nextEdge(list<int> &x, bool *y, bool &czy) {
	int min = 2147483647;
	int minE;
	bool zmianaMinE = false;
	for (int i = 0; i < vertices; i++) {
		if (!y[i]) {
			minE = i;
		}
	}

	for (int i = 0; i < vertices; i++) {
		if (y[i]) {
			for (int j = 0; j < edges; j++) {
				if (((edgesWeight[j][0] == i) || (edgesWeight[j][1] == i))) {
					if ((edgesWeight[j][0] == i) && (!y[edgesWeight[j][1]]) && (edgesWeight[j][2]<min)) {
						minE = j;
						min = edgesWeight[minE][2];
						zmianaMinE = true;
					}
					if ((edgesWeight[j][1] == i) && (!y[edgesWeight[j][0]]) && (edgesWeight[j][2]<min)) {
						minE = j;
						min = edgesWeight[minE][2];
						zmianaMinE = true;
					}
				}

			}
		}
	}
	if (zmianaMinE) {
		if (y[edgesWeight[minE][0]]) {
			y[edgesWeight[minE][1]] = true;
		}
		else {
			y[edgesWeight[minE][0]] = true;
		}
		x.push_front(minE);
		czy = true;
		return;
	}
	else {
		for (int i = 0; i < edges; i++) {
			if (((edgesWeight[i][0] == minE) || (edgesWeight[i][1] == minE))) {

				y[edgesWeight[i][1]] = true;
				y[edgesWeight[i][0]] = true;
				x.push_front(i);
				czy = true;
				return;
			}

		}
	}
	czy = false;
	return;
}

void NextList::dijkstra(int ver) {
	int maxint = 2147483647;
	int *pastV = new int[vertices];
	int *costV = new int[vertices];
	bool *visited = new bool[vertices];
	NList *help;
	help = new NList;
	int counter;
	int current=0;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
		costV[i] = maxint;
	}
	cout << ver << endl;
	int stackPointer = 0;
	costV[ver] = 0;
	//visited[ver] = true;
	//cout << "current : " << current << endl;
	for (int j = 0; j < vertices; j++) {
		for (counter = 0; (visited[counter]); counter++) {}
			//cout << "i w petli wyzej wynosi: " << i << endl;
			for (current = counter++; counter < vertices; counter++) {
				//cout << "i wynosi: " << i << endl;
				//cout << "current wynosi: " << current << endl;
				if ((!visited[counter]) && (costV[counter] < costV[current])) {
					//cout << "if wszedl";
					current = counter;
					//cout << "currentIF wynosi: " << current << endl;
				}
			}
		
		//cout << "Obecny wierz" << current << endl;
		visited[current] = true;
		for (help = lista[current]; help; help=help->next) {
			if ((!visited[help->v]) && (costV[help->v] > (costV[current] + help->w))) {
				costV[help->v] = costV[current] + help->w;
				pastV[help->v] = current;
			}
		}
	}
	int destination;
	cout << endl;
	cout << "Dostepne wierzcholki: ";
	for (int i = 0; i < vertices; i++) {
		if (pastV[i] != -1) {
			cout << i << ", ";
		}
	}
	
	delete[] costV;
	delete[] pastV;
	//delete[] path;
	delete[] visited;
}

void NextList::fb(int ver) {
	bool changes;
	bool minusCycle = false;
	bool fCycle = false;
	int maxint = 214748364;
	int *pastV = new int[vertices];
	int *costV = new int[vertices];
	NList *help;
	for (int i = 0; i < vertices; i++) {
		pastV[i] = -1;
		costV[i] = maxint;
	}
	cout << ver << endl;
	int stackPointer = 0;
	costV[ver] = 0;
	int neighbor = -1;
	for (int i = 1; i <vertices; i++) {                       // Petla musi przejsc n-1 raz, gdzie n oznacza liczbe wierzcholkow
		changes = true;                                       // Oznacza, ¿e algorytm nie wprowadzi³ zmian do tablic past i cost
		for (short j = 0; j <vertices ; j++) {                // Przechodzimy przez kolejne wierzcho³ki grafu
			for (help = lista[j]; help ; help = help->next) { // Przegl¹damy listê krawedzi aby znalezc sasiadow wierzcholka j
				if (costV[help->v] > (costV[j] + help->w)) {
					changes = false;                          // Jest zmiana w tablicach cost i past
					costV[help->v] = costV[j] + help->w;      // Wpisujemy koszt przejscia
					pastV[help->v] = j;                       // Poprzednikiem s¹siada bêdzie wierzcholek j
				}
			}
		}
		if (changes) maxint = 0;
		if (maxint == 0) break;
	}
	for (int i = 0; i < vertices; i++) {
		for (help = lista[i]; help; help = help->next) {
			if (costV[help->v] > costV[i] + help->w) {
				minusCycle = true;
			}
		}
	}
	if (minusCycle){
		cout << "Mamy tu cykl ujemny!\n";
	return;
	}
	/*for (int i = 0; i < vertices; i++) {
		cout << pastV[i] << " : " << costV[i] << endl;
	}*/
	int destination;
	cout << endl;
	cout << "Dostepne wierzcholki: ";
	for (int i = 0; i < vertices; i++) {
		if (pastV[i] != -1) {
			cout << i << ", ";
		}
	}
	cout << endl;
	cout << "Do jakiego wierzcholka chcesz sie dostac?\n";
	cin >> destination;
	if (destination < 0 || destination >= vertices) {
		cout << "Nie ma takiego wierzcholka!\n";
		return;
	}
	stackPointer = 0;

	int *path = new int[vertices];
	cout << pastV[destination] << endl;
	for (int i = destination; i > -1; i = pastV[i]) {
		cout << "past : " << pastV[i]<<endl;
		cout << "stackp :" << stackPointer << endl;
		path[stackPointer] = i;
		stackPointer++;
		Sleep(500);
	}
	cout << endl;
	cout << "Sciezka to :";
	while (stackPointer) {
		stackPointer--;
		cout << path[stackPointer] << " - ";

	}
	cout << endl;
	cout << "Jej koszt to: " << costV[destination];
	cout << endl;

	delete[] costV;
	delete[] pastV;
	delete[] path;
	//delete[] visited;
}

void NextList::ff(int &path, int *x, int stop, short **y) {
	int stopV = 0;
	int min = 222222;
	int *edge;
	int help = stop;
	while (help != (-2)) {
		help = x[help];
		stopV++;
	}
	int z = stopV - 1;
	edge = new int[stopV];
	stopV = stop;
	int counter = 0;
	int counterFalse = 0;
	while (stopV != -2) {
		for (int i = 0; i < edges; i++) {
			if (y[i][0] == x[stopV] && y[i][1] == stopV) {
				if ((min > y[i][2]) && (y[i][2] > 0)) {
					min = y[i][2];
				}
				counterFalse = 0;
				edge[counter] = i;
				counter++;
				stopV = x[stopV];
				//	cout << stopV << endl;
				if (x[stopV] == -2) {
					stopV = -2;
				}
				break;
			}

			counterFalse++;
		}
		//cout << "while w ff" << endl;
	}
	if (counterFalse >= edges - z) {
		return;
	}
	//cout << "min wynosi: " << min << endl;
	for (int i = 0; i < z; i++) {
		y[edge[i]][3] = min;
		y[edge[i]][2] = y[edge[i]][2] - min;
	}
	path = path + min;
	delete[] edge;
	//cout << "wychodzi z ff" << endl;
}

void NextList::ffDFS(int start, int stop) {
	//int maxint = 2147483647;
	int sciezka = 0;
	bool done = false;
	bool change = true;
	int *pastV;
	pastV = new int[vertices];
	short **edg;
	edg = new short *[edges];
	for (int i = 0; i < edges; i++) {
		edg[i] = new short[4];
		edg[i][0] = edgesWeight[i][0];
		edg[i][1] = edgesWeight[i][1];
		edg[i][2] = edgesWeight[i][2];
		edg[i][3] = 0;
	}
	bool *visited = new bool[vertices];
	bool loop = true;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
	}
	int currentV = start;
	int counter = 0;
	int counterFalse = 0;
	visited[start] = true;
	pastV[start] = -2;
	//cout << "currentV: " << currentV << endl;
	if (start == stop) {
		loop = false;
	}
	int stopH = stop;
	int counterBreak = 0;
	while (loop) {
		change = false;
		for (int i = 0; i < edges; i++) {
			if ((edg[i][0] == currentV) && (!visited[edg[i][1]]) && (edg[i][2]>0)) {
				currentV = edg[i][1];
				pastV[currentV] = edg[i][0];
				visited[edg[i][1]] = true;
				change = true;
				//cout << "zmiana" << endl;
				//cout << "currentV: " << currentV << endl;
				//cout << "past od currentV: " << pastV[currentV] << endl;
				//cout << "waga krawedzi: " << edg[i][2] << endl;
				break;
			}

		}
		//Sleep(10000);
		if (currentV == stop) {
			//Sleep(1000);
			ff(sciezka, pastV, stopH, edg);
			if (stopH == -1) {
				counterBreak++;
			}
			for (int j = 0; j < vertices; j++) {
				pastV[j] = -1;
				visited[j] = false;
			}
			visited[start] = true;
			pastV[start] = -2;
			change = true;
			currentV = start;
			done = true;
		}
		if (!change) {
			//cout << "cofamy sie!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			//cout << "currentV: " << currentV << endl;
			//cout << "past od currentV: " << pastV[currentV] << endl;
			counter = pastV[currentV];
			pastV[currentV] = -1;
			currentV = counter;
			//cout << "currentV: " << currentV << endl;
			//cout << "past od currentV: " << pastV[currentV] << endl;
			counterFalse++;
		}

		if ((currentV == (-2))) {
			loop = false;
		}
		if (counterBreak == vertices) {
			loop = false;
		}
	}
	if (!done) {
		cout << "Sciezka nie istnieje z podanego wierzcholka do wierzcholka podanego.\n";
		return;
	}
	cout << "Sciezka maksymalnego przeplywu wynosi: " << sciezka << endl;
	for (int i = 0; i < edges; i++) {
		delete edg[i];
	}
	delete[] edg;
	delete[] visited;
}

void NextList::fff(int &path, int x[], int stop, short **y) {
	int stopV = 0;
	int min = 222222;
	int *edge;
	int help = stop;;
	for (int i = 0; help != (-2); i++) {
		help = x[help];
		stopV++;
	}
	//Sleep(5);
	int z = stopV - 1;
	edge = new int[stopV];
	stopV = stop;
	int counter = 0;
	while (stopV != -2) {
		for (int i = 0; i < edges; i++) {
			if (y[i][0] == x[stopV] && y[i][1] == stopV) {
				if (min > y[i][2]) {
					min = y[i][2];
				}
				edge[counter] = i;
				counter++;
				stopV = x[stopV];
				//cout << stopV << endl;
				break;
			}
			if (x[stopV] == -2) {
				stopV = -2;
			}
		}
		//cout << "while w ff" << endl;
	}
	for (int i = 0; i < z; i++) {
		y[edge[i]][3] = min;
		y[edge[i]][2] = y[edge[i]][2] - min;
	}
	path = path + min;
	//cout << "wychodzi z ff" << endl;
}

void NextList::ffBFS(int start, int stop) {
	//int maxint = 2147483647;
	int sciezka = 0;
	bool done = false;
	bool change = true;
	int *pastV;
	pastV = new int[vertices];
	short **edg;
	edg = new short *[edges];
	for (int i = 0; i < edges; i++) {
		edg[i] = new short[4];
		edg[i][0] = edgesWeight[i][0];
		edg[i][1] = edgesWeight[i][1];
		edg[i][2] = edgesWeight[i][2];
		edg[i][3] = 0;
	}
	bool *visited = new bool[vertices];
	bool loop = true;
	for (int i = 0; i < vertices; i++) {
		visited[i] = false;
		pastV[i] = -1;
	}
	int currentV = start;
	int counter = 0;
	int counterFalse = 0;
	visited[start] = true;
	pastV[start] = -2;
	//cout << "currentV: " << currentV << endl;
	if (start == stop) {
		loop = false;
	}
	while (loop) {
		change = false;
		for (int j = 0; j<vertices; j++) {
			if (visited[j]) {
				for (int i = 0; i<edges; i++) {
					//cout << i << "ta petla w " << j << endl;
					if ((edg[i][0] == j) && (!visited[edg[i][1]]) && (edg[i][2]>0)) {
						//currentV = edg[i][1];
						pastV[edg[i][1]] = edg[i][0];
						//visited[edg[i][1]] = true;
						change = true;
						//cout << "zmiana" << endl;
						//cout << "currentV: " << currentV << endl;
						//cout << "past od currentV: " << pastV[currentV] << endl;
						//i = edges;
						counterFalse++;
					}

				}
			}

		}
		for (int i = 0; i < vertices; i++) {
			if (!visited[i] && (pastV[i] != -1)) {
				visited[i] = true;
				currentV = i;
				if (currentV == stop) {
					fff(sciezka, pastV, stop, edg);
					for (int j = 0; j < vertices; j++) {
						pastV[j] = -1;
						visited[j] = false;
					}
					visited[start] = true;
					pastV[start] = -2;
					change = true;
					currentV = start;
					done = true;
					i = vertices;
				}
			}
		}


		if (!change) {
			counterFalse++;
		}
		if (counterFalse > edges) {
			loop = false;
		}

		if (currentV == (-2)) {
			loop = false;
		}
	}
	if (!done) {
		cout << "Sciezka nie istnieje z podanego wierzcholka do wierzcholka podanego.\n";
		return;
	}
	cout << "Sciezka maksymalnego przeplywu wynosi: " << sciezka << endl;
	for (int i = 0; i < edges; i++) {
		delete edg[i];
	}
	delete[] edg;
	delete[] visited;
}

void NextList::doRandList(int ver, int g) {
	for (int i = 0; i < vertices; i++) {
		delete[] lista[i];
	}
	delete[] lista;
	for (int i = 0; i < edges; i++) {
		delete[] edgesWeight[i];
	}
	delete[] edgesWeight;
	vertices = ver;
	
	edges = (100 * vertices) / g;
	alreadyWasSaved = new short[edges];
	lista = new NList*[vertices];
	edgesWeight = new short*[edges];
	for (int i = 0; i < edges; i++) {
		edgesWeight[i] = new short[3];
	}
	for (int i = 0; i < vertices; i++) {
		lista[i] = NULL;
	}
	int counter = 0;
	int verticeP;
	int verticeK;
	int znak;
	signed short weight;
	NList *help;
	while (counter<edges) {
		help = new NList;
		verticeP = (rand() % vertices) + 0;
		verticeK = (rand() % vertices) + 0;
		while (verticeK == verticeP) {
			verticeK = (rand() % vertices) + 0;
		}
		weight = (rand() % 10) + 0;
		znak = (rand() % 2) + 0;
		if (znak == 1) {
			weight = 0 + weight;
		}
		else {
			weight = 0 - weight;
		}
		help->v = verticeK;
		help->w = weight;
		help->next = lista[verticeP];
		lista[verticeP] = help;
		edgesWeight[counter][0] = verticeP;
		edgesWeight[counter][1] = verticeK;
		edgesWeight[counter][2] = weight;
		counter++;
	}
}

void NextList::doRandList() {
	for (int i = 0; i < vertices; i++) {
		delete[] lista[i];
	}
	delete[] lista;
	for (int i = 0; i < edges; i++) {
		delete[] edgesWeight[i];
	}
	delete[] edgesWeight;
	int gestosc;
	cout << "Podaj liczbe wierzcholkow: \n";
	cin >> vertices;
	cout << "Podaj gestosc krawedzi :\n";
	cin >> gestosc;
	edges = (100 * vertices) / gestosc;
	alreadyWasSaved = new short[edges];
	lista = new NList*[vertices];
	edgesWeight = new short*[edges];
	for (int i = 0; i < edges; i++) {
		edgesWeight[i] = new short[3];
	}
	for (int i = 0; i < vertices; i++) {
		lista[i] = NULL;
	}
	int counter = 0;
	int verticeP;
	int verticeK;
	int znak;
	signed short weight;
	NList *help;
	while (counter<edges) {
		help = new NList;
		verticeP = (rand() % vertices) + 0;
		verticeK = (rand() % vertices) + 0;
		while (verticeK == verticeP) {
			verticeK = (rand() % vertices) + 0;
		}
		weight = (rand() % 10) + 0;
		/*znak = (rand() % 2) + 0;
		if (znak == 1) {
			weight = 0 + weight;
		}
		else {
			weight = 0 - weight;
		}*/
		help->v = verticeK;
		help->w = weight;
		help->next = lista[verticeP];
		lista[verticeP] = help;
		edgesWeight[counter][0] = verticeP;
		edgesWeight[counter][1] = verticeK;
		edgesWeight[counter][2] = weight;
		counter++;
	}
}


class Dane {
public:
	short iloscWierzcholkow;
	short iloscKrawedzi;
	void doData();

};

void Dane::doData() {
	short znak;
	short waga;;
	short liczbaWierzcholkaP;
	short liczbaWierzcholkaK;
	ofstream plik("dane.txt");
	if (plik) {
		plik << iloscKrawedzi << " " << iloscWierzcholkow << endl;;
		srand(time(NULL));
		for (int i = 0; i < iloscKrawedzi; i++) {
			
			liczbaWierzcholkaP = (rand() % iloscWierzcholkow) + 0;
			waga = (rand() % 10) + 0;
			znak = (rand() % 1);
			if (znak == 1) {
				waga = 0 + waga;
			}
			else {
				waga = 0 - waga;
			}
			liczbaWierzcholkaK = (rand() % iloscWierzcholkow) + 0;
			if (liczbaWierzcholkaK == liczbaWierzcholkaP) {
				if (znak == 1) {
					liczbaWierzcholkaK = liczbaWierzcholkaK-1;
				}
				else {
					liczbaWierzcholkaK = liczbaWierzcholkaK+1;
				}
			}
			if ((liczbaWierzcholkaK >= iloscWierzcholkow) || (liczbaWierzcholkaP >= iloscWierzcholkow) || (liczbaWierzcholkaK < 0) || (liczbaWierzcholkaP < 0)) {
				iloscKrawedzi--;
			}
			else
			{
				plik << liczbaWierzcholkaP << " " << liczbaWierzcholkaK << " " << waga << endl;
			}
			
		}

	}
	else {
		cout << "B£¥D: nie mo¿na otworzyæ pliku." << endl;
	}
	plik.close();
}


class Menu { //Klasa zarzadzajaca glownym Menu programu

public:
	unsigned short option;

	Menu();
	void AskForOption() {
		while (true) {//Nasze glowne menu
			cout << "Witaj w programie uzytkowniku.\n";
			cout << "Wpisz numer polecenia: \n";
			cout << "Opcje reprezentacji grafu: \n";
			cout << "1. Macierz indykacji. STRUKTURA NIEGOTOWA.\n";
			cout << "2. Lista nastepnikow. STRUKTURA NIEGOTOWA.\n";
			cout << "5. Wygeneruj nowe dane. \n";
			//cout << "9. Rozpocznij testy.\n";
			cout << "Aby wyjsc wpisz cokolwiek innego. \n";
			cin >> option;
			if (!cin) {
				cin.clear();
				cin.ignore();
			}
			switch (option) {
			case 1://Macierz
			{
				IncidenceMatrix matrix;
				matrix.menu();
				break;
			}
			case 2://Lista
			{
				NextList lis;
				lis.menu();
				break;
			}
			
			case 5://Stworzmy inne dane
			{
				Dane data;
				cout << "Ile wierzcholkow ma byc w grafie?\n";
				cin >> data.iloscWierzcholkow;
				cout << "Ile krawedzi ma byc w grafie?\n";
				cin >> data.iloscKrawedzi;
				data.doData();
				break;
			}
			
			default:
				cout << "Zla opcja.";
				exit(1000);
				break;
			}
		}
	}


};
Menu::Menu() {
	option = 0;
}
int main()
{
	Menu menu;
	menu.AskForOption();
	return 0;
}

