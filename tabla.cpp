#include "tabla.h"

using std::get;
turn tabla::t = x;

tabla::tabla() {
	polja = new int*[3]();
	for (auto& polje : *this)
		polje = new int[3]();
}

tabla::~tabla() {
	for (auto& polje : *this) if (polje != nullptr) { delete[] polje; polje = nullptr; }
	delete[] polja;
	polja = nullptr;
}

int tabla::handleInput(int x, int y) {
	if (isFreeField(x, y)) {
		remOld();
		polja[x][y] = t;
		rem.push(std::make_tuple(x, y));
		if (didWin(x, y)) return 101;
		t = t == ::x ? ::o : ::x;
		return 0;
	}
	return 1;
}

bool tabla::didWin(int x, int y) const {
	int num = 0;
	for (int dy = 0; dy < 3; dy++) {
		if (polja[x][dy] == t) num++;
		else break;
	}
	if (num >= 3) return true;
	num = 0;
	for (int dx = 0; dx < 3; dx++) {
		if (polja[dx][y] == t) num++;
		else break;
	}
	if (num >= 3) return true;
	num = 0;
	if (x == y) {
		for (int k = 0; k < 3; k++) {
			if (polja[k][k] == t) num++;
			else break;
		}
	}
	if (num >= 3) return true;
	num = 0;
	if (x + y == 2) {
		for (int k = 0; k < 3; k++) {
			if (polja[k][2-k] == t) num++;
			else break;
		}
	}
	if (num >= 3) return true;
	return false;
}

bool tabla::isFreeField(int x, int y) const {
	if (x >= 3 || x < 0 || y >= 3 || y < 0) return false;
	return !polja[x][y];
}

int tabla::remOld() {
	if (rem.size() < RM_FQ) return 0;
	try {
		tuple<int, int> a;
		a = rem.front();
		polja[get<0>(a)][get<1>(a)] = 0;
		rem.pop();
		return 0;
	} catch (int e) { return e; }
}

int tabla::getValue(int x, int y) const {
	return polja[x][y];
}
