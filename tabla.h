#pragma once
#include "turn.h"
#include <queue>
#include <tuple>
using std::queue;
using std::tuple;

#define CELL_SIZE 100
#define RM_FQ 6

class tabla
{
	int** polja;
	static turn t;
	queue<tuple<int, int>> rem;

	int remOld();
	bool isFreeField(int, int) const;
	bool didWin(int, int) const;
public:
	tabla();
	~tabla();
	inline int** begin() const { return polja; }
	inline int** end() const { return polja + 3; }
	int handleInput(int, int);
	int getValue(int, int) const;
	inline tuple<int, int> nextToVanish() const { if (rem.size() < RM_FQ) return std::make_tuple(-1, -1); return rem.front(); }
	inline int turn_t() const { return (int)t; }
};

