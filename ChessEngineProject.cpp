// ChessEngineProject.cpp 
#include <iostream>
#include <array>
using namespace std;
#include "stdio.h"
#include "defs.h"
#include "stdlib.h"
#include<time.h>

S_BOARD board[1];


string GetPieceName(const int piece) {

	string pieceName;

	switch (piece) {
	case wP: pieceName = "white pawn"; break;
	case wN: pieceName = "white knight"; break;
	case wB: pieceName = "white bishop"; break;
	case wR: pieceName = "white rook"; break;
	case wQ: pieceName = "white queen"; break;
	case wK: pieceName = "white king"; break;
	case bP: pieceName = "black pawn"; break;
	case bN: pieceName = "black knight"; break;
	case bB: pieceName = "black bishop"; break;
	case bR: pieceName = "black rook"; break;
	case bQ: pieceName = "black queen"; break;
	case bK: pieceName = "black king"; break;
	}

	return pieceName;

}

void PieceAttacked(const int file, const int rank, const S_BOARD* pos) {

	int sq = FileAndRank_To_Square(file, rank);
	int piece = pos->pieces[sq];
	int opponentColour = 1 - PieceCol[piece];
	int i = 0;

	char fileLetter = char(65 + file);
	int rankNumber = rank + 1;

	std::string pieceName = GetPieceName(piece);

	if (piece == EMPTY_SQUARE || piece == OFF_BOARD) {
		cout << "This square is empty.";
		return;
	}
	
	array<int, 16> attackers = SqAttacked(sq, opponentColour, pos);

	while (attackers[i] != EMPTY_SQUARE) {
		if(i==0)	cout << "The " << pieceName << " on square " << fileLetter << rankNumber << " is attacked by: \n";
		cout << "A " << GetPieceName(attackers[i]) << endl;

		i++;
	}

	if (i == 0) {
		cout << "The " << pieceName << " on square " << fileLetter << rankNumber << " is not attacked.";
	}
	
}

void AnalysePos() {

	int materialImbalance = MaterialDifference(board);

	if (materialImbalance > 0) {
		cout << "White has a material advantage of " << materialImbalance << " points.";
	}
	if (materialImbalance < 0) {
		cout << "Black has a material advantage of " << -materialImbalance << " points.";
	}
	if (materialImbalance == 0) {
		cout << "Material is equal";
	}

}

void AnalyseSqr(const int file, const int rank) {
	PieceAttacked(file, rank, board);
}

const char* RandomFen() {
	const char* FEN_Database[] = { "r3k2r/1bpp1ppp/p7/1pb5/B3P1nq/5N2/PP3PPP/RNBQR1K1 w Qkq - 0 1",
									"b2r3r/k4p1p/p2q1np1/NppP4/3p1Q2/P4PPB/1PP4P/1K1RR3 w - - 1 24",
									"4kb1r/p2n1ppp/4q3/4p1B1/4P3/1Q6/PPP2PPP/2KR4 w k - 0 16",
									"2rq1rk1/pb1n1ppN/3bp3/1p6/3P1Pn1/P1NB4/1PQ3PP/R1B2RK1 b - - 0 15",
									"r3r1k1/pp3pbp/1qp3p1/2B5/2BP2b1/Q1n2N2/P4PPP/3R1K1R b - - 3 17" };

	srand(time(0));
	int index = rand() % 5;
	return FEN_Database[index];
}

void EnterAndDisplayFEN() {

	std::string fen;
	cin >> fen;
	if (fen[0] == 'f') {
		fen = RandomFen();
	}
	const char* fenDisplay = fen.c_str();

	ParseFen((char*)fenDisplay, board);
	PrintBoard(board);
}

int main() {

	AllInit();
	
	cout << "To begin please enter a fen of the position you wish to analyse. Fen entered should be in the form \n rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1 \n";
	cout << "Or you can analyse a pre-defined fen by entering 'f'";

	EnterAndDisplayFEN();

	cout << "\nTo find out information about the position press 'p'.\n";
	cout << "To find out information about a piece enter the square it resides on.\n";

	std::cin.ignore(INT_MAX, '\n'); // Clear cin buffer to recieve next input.

	std::string decision;
	cin >> decision;
	
	if ((char)tolower(decision[0]) == 'p') {
		AnalysePos();
	}
	else { 
		int file = int((char)tolower(decision[0]));
		int rank = int(decision[1]);

		if (file >= 97 && file <= 104 && rank >= 49 && rank <= 56) { // Check ascii values of input to make sure a valid coordinate has been entered.

			AnalyseSqr(file - 97, rank - 49);
		}
		else {
			cout << "Input not recognised";
		}
	}	

	return 0;
}





