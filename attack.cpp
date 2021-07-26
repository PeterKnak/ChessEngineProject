#include <iostream>
#include <array>
using namespace std;
#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

const int KnDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };

const int RkDir[4] = { -1, -10,	1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

int PieceKnight[13] = { false, false, true, false, false, false, false, false, true, false, false, false, false };
int PieceKing[13] = { false, false, false, false, false, false, true, false, false, false, false, false, true };
int PieceRookQueen[13] = { false, false, false, false, true, true, false, false, false, false, true, true, false };
int PieceBishopQueen[13] = { false, false, false, true, false, true, false, false, false, true, false, true, false };

array<int, 16> SqAttacked(const int sq, const int side, const S_BOARD* pos) {
	// square attacked, side doing the attacking, board
	int pce, index, t_sq, dir;
	int attackerCount = 0;

	array<int, 16> attackers = { 0 };

	// pawns
	if (side == WHITE) {
		if (pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
			attackers[attackerCount] = wP;
			attackerCount++;
		}
	}
	else {
		if (pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP) {
			attackers[attackerCount] = bP;
			attackerCount++;
		}
	}

	// knights
	for (index = 0; index < 8; ++index) {
		pce = pos->pieces[sq + KnDir[index]];
		if (IsKn(pce) && PieceCol[pce] == side) {
			attackers[attackerCount] =bN;
			attackerCount++;
		}
	}

	// rooks, queens
	for (index = 0; index < 4; ++index) {
		dir = RkDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while (pce != OFF_BOARD) {
			if (pce != EMPTY_SQUARE) {
				if (IsRQ(pce) && PieceCol[pce] == side) {
					if (!PieceCol[pce]) { // if a white piece
						attackers[attackerCount] = IsBQ(pce) ? wQ : wR; // can it move like a bishop as well?
						attackerCount++;
					}
					else {
						attackers[attackerCount] = IsBQ(pce) ? bQ : bR; 
						attackerCount++;
					}
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

	// bishops, queens
	for (index = 0; index < 4; ++index) {
		dir = BiDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while (pce != OFF_BOARD) {
			if (pce != EMPTY_SQUARE) {
				if (IsBQ(pce) && PieceCol[pce] == side) {
					if (!PieceCol[pce]) { // if a white piece
						attackers[attackerCount] = IsRQ(pce) ? wQ : wB; // can it move like a rook as well?
						attackerCount++;
					}
					else {
						attackers[attackerCount] = IsRQ(pce) ? bQ : bB;
						attackerCount++;
					}
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

	// kings
	for (index = 0; index < 8; ++index) {
		pce = pos->pieces[sq + KiDir[index]];
		if (IsKi(pce) && PieceCol[pce] == side) {
			attackers[attackerCount] = PieceCol[pce] ? bK : wK;
			attackerCount++;
		}
	}

	return attackers;

}