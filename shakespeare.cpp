#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#define N 30000
#define NMONITOR 1000
#define MAXCOL 100
#define INPUTFNAME "shakespeare.txt"
#define OUTPUTFNAME "db_shakespeare.txt"

using namespace std;

int comp(const void *a, const void *b);
bool isLetter(char);
bool isEmpty(char*);
void inPut();

class Dict {
public:
	struct Node {
		int nrow;
		string pred;
		string succ;
		Node *next;
	};

	struct Root {
		int nNode;
		string word;
		Node *next;
	};

	Dict::Dict() {
		nTotal = 0;
	}

	Dict::~Dict() {
	}

	void Dict::add(string word, string pred, string succ, int nrow) {
		capToSma(&word);
		capToSma(&pred);
		capToSma(&succ);
		int pos = -1;
		for (int i = 0; i < nTotal; i++) {
			if (wordList[i].word == word) {
				pos = i;
				break;
			}
		}
		if (pos == -1) {
			pos = nTotal++;
			wordList[pos].nNode = 1;
			wordList[pos].word = word;
			Node *temp = new Node;
			temp->nrow = nrow;
			temp->pred = pred;
			temp->succ = succ;
			temp->next = NULL;
			wordList[pos].next = temp;
		}
		else {
			wordList[pos].nNode++;
			Node *newNode = NULL;
			newNode = wordList[pos].next;
			while (newNode->next != NULL) {
				newNode = newNode->next;
			}
			Node *temp = new Node;
			temp->nrow = nrow;
			temp->pred = pred;
			temp->succ = succ;
			temp->next = NULL;
			newNode->next = temp;
		}
		return;
	}

	void Dict::sort() {
		qsort((void*)wordList, nTotal, sizeof(wordList[0]), comp);
		return;
	}

	void binSearch(string key) {
		capToSma(&key);
		int head = 0;
		int tail = nTotal - 1;
		while (head <= tail) {
			int med = (head + tail) / 2;
			if (key == wordList[med].word) {
				Node *next = wordList[med].next;
				while (next != NULL) {
					cout << "Line: " << next->nrow;
					cout << "   " << next->pred;
					cout << " " << wordList[med].word;
					cout << " " << next->succ << endl;
					next = next->next;
				}
				cout << "Done." << endl << endl;
				return;
			}
			else if (key < wordList[med].word) {
				tail = med - 1;
			}
			else {
				head = med + 1;
			}
		}
		cout << "Not found." << endl << endl;
		return;
	}

	void Dict::print() {
		ofstream fout(OUTPUTFNAME);
		fout << nTotal << endl;
		for (int i = 0; i < nTotal; i++) {
			fout << " " << wordList[i].word;
			fout << " " << wordList[i].nNode << endl;
			Node *temp = wordList[i].next;
			for (int j = 0; j < wordList[i].nNode; j++) {
				fout << "  " << temp->nrow;
				fout << " " << temp->pred;
				fout << " " << temp->succ << endl;
				if (temp->next != NULL) {
					temp = temp->next;
				}
			}
		}
		fout.close();
		return;
	}

private:
	int nTotal;
	Root wordList[N];
	void Dict::capToSma(string *s) {
		for (int i = 0; i < (int)s->size(); i++) {
			int sint = (int)(*s)[i];
			if ((sint >= 65) && (sint <= 90)) {
				(*s)[i] = (char)(sint + 32);
			}
		}
		return;
	}
};

Dict shakespeare;

int main()
{
	inPut();
	shakespeare.sort();
	string key;
	do {
		cout << "Ready. Please input the key word for inquiring (input '#' to exit): ";
		cin >> key;
		if (key != "#") {
			shakespeare.binSearch(key);
		}
	} while (key != "#");
	cout << "Exit." << endl;
	//shakespeare.print();
	return 0;
}

int comp(const void *a, const void *b) {
	return ((((Dict::Root*)a)->word >((Dict::Root*)b)->word) ? 1 : -1);
}

bool isLetter(char x, bool isNumber) {
	int xint = int(x);
	if ((xint >= 48) && (xint <= 57)) {
		return true;
	}
	if (!isNumber) {
		if ((xint >= 65) && (xint <= 90)) {
			return true;
		}
		if ((xint >= 97) && (xint <= 122)) {
			return true;
		}
	}
	return false;
}

bool isEmpty(char* s) {
	for (int i = 0;i < (int)strlen(s);i++) {
		if (s[i] != ' ') {
			return false;
		}
	}
	return true;
}

void inPut() {
	char buffer_0[MAXCOL] = " ";
	char buffer_1[MAXCOL] = "";
	int nRow = 0;
	int nLeap = 0;
	bool flag;
	string buffer_01("");
	string line[MAXCOL / 3];
	string lastend = "";
	ifstream infile(INPUTFNAME);
	do {
		if ((nRow > 0) && (nRow % NMONITOR == 0)) {
			cout << "Now processing input: " << nRow << " rows finished" << endl;
		}
		flag = false;
		if (infile.getline(buffer_1, MAXCOL)) {
			flag = true;
			for (int i = 0; i < (int)strlen(buffer_1); i++) {
				if (buffer_1[i] == '-') {
					bool flag = false;
					if ((i > 0) && (i < (int)strlen(buffer_1) - 1)) {
						if ((isLetter(buffer_1[i - 1], false)) && (isLetter(buffer_1[i + 1], false))) {
							flag = true;
						}
					}
					if (flag) {
						continue;
					}
					else {
						buffer_1[i] = ' ';
					}
				}
				else if (buffer_1[i] == '.') {
					bool flag = false;
					if ((i > 0) && (i < (int)strlen(buffer_1) - 1)) {
						if ((isLetter(buffer_1[i - 1], true)) && (isLetter(buffer_1[i + 1], true))) {
							flag = true;
						}
					}
					if (flag) {
						continue;
					}
					else {
						buffer_1[i] = ' ';
					}
				}
				else if (isLetter(buffer_1[i], false)) {
					continue;
				}
				else {
					buffer_1[i] = ' ';
				}
			}
		}
		if (isEmpty(buffer_1)) {
			nLeap++;
			continue;
		}
		buffer_01 = " ";
		buffer_01 += buffer_0;
		buffer_01.append(1, ' ');
		buffer_01 += buffer_1;
		buffer_01.append(1, ' ');
		int head = 0;
		int tail = 0;
		int nword = 0;
		for (int i = 1; i <= (int)strlen(buffer_0) + 1;) {
			while (buffer_01[i] == ' ') {
				i++;
				if (i >= (int)buffer_01.size()) {
					break;
				}
			}
			head = i;
			if (i >= (int)buffer_01.size()) {
				break;
			}
			while (buffer_01[i] != ' ') {
				i++;
				if (i >= (int)buffer_01.size()) {
					break;
				}
			}
			tail = i;
			if ((tail > head) && (tail < (int)buffer_01.size())) {
				line[nword] = "";
				line[nword++].append(buffer_01, head, tail - head);
			}
		}
		for (int i = 0; i < nword - 1; i++) {
			if (i == 0) {
				shakespeare.add(line[i], lastend, line[i + 1], nRow);
			}
			else {
				shakespeare.add(line[i], line[i - 1], line[i + 1], nRow);
			}
		}
		if (flag) {
			if (nword - 2 >= 0) {
				lastend = line[nword - 2];
			}
			strcpy_s(buffer_0, buffer_1);
			strcpy_s(buffer_1, "");
		}
		else {
			shakespeare.add(line[nword - 1], line[nword - 2], "", nRow);
		}
		nRow += nLeap + 1;;
		nLeap = 0;
	} while (flag);
	infile.close();
	cout << "Done. totally " << nRow << " rows." << endl << endl;
	return;
}