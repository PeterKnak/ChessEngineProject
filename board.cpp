#include <iostream>
using namespace std;
#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

char PceChar[] = ".PNBRQKpnbrqk";
char SideChar[] = "wb-";
char RankChar[] = "12345678";
char FileChar[] = "abcdefgh";

int PieceBig[13] = { false, false, true, true, true, true, true, false, true, true, true, true, true };
int PieceMaj[13] = { false, false, false, false, true, true, true, false, false, false, true, true, true };
int PieceMin[13] = { false, false, true, true, false, false, false, false, true, true, false, false, false };
int PieceVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
int PieceValHuman[13] = { 0, 1, 3, 3, 5, 9, 200, -1, -3, -3, -5, -9, -200 };
int PieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

int MaterialDifference(S_BOARD* pos) {

	int piece, sq;
	int materialDiff = 0;

	for (int index = 0; index < BRD_SQR_NUM; ++index) {
		sq = index;
		piece = pos->pieces[index];
		if (piece != OFF_BOARD && piece != EMPTY_SQUARE) {
			materialDiff += PieceValHuman[piece];
		}
	}

	return materialDiff;

}

void UpdateListsMaterial(S_BOARD* pos) {

	int piece, sq, colour;

	for (int index = 0; index < BRD_SQR_NUM; ++index) {
		sq = index;
		piece = pos->pieces[index];
		if (piece != OFF_BOARD && piece != EMPTY_SQUARE) {
			colour = PieceCol[piece];

			if (PieceBig[piece] == true) pos->non_pawn_pieces[colour]++;
			if (PieceMin[piece] == true) pos->minor_pieces[colour]++;
			if (PieceMaj[piece] == true) pos->major_pieces[colour]++;

			pos->material[colour] += PieceVal[piece];

			pos->pList[piece][pos->piece_type_total[piece]] = sq;
			pos->piece_type_total[piece]++;

			if (piece == wK) pos->KingSq[WHITE] = sq;
			if (piece == bK) pos->KingSq[BLACK] = sq;
		}
	}

}

int ParseFen(char *fen, S_BOARD *pos) {

	int  rank = RANK_8;
	int  file = FILE_A;
	int  piece = 0;
	int  count = 0;
	int  sq64 = 0;
	int  sq120 = 0;

	ResetBoard(pos);

	while ((rank >= RANK_1) && *fen) {
		count = 1;
		switch (*fen) {
		case 'p': piece = bP; break;
		case 'r': piece = bR; break;
		case 'n': piece = bN; break;
		case 'b': piece = bB; break;
		case 'k': piece = bK; break;
		case 'q': piece = bQ; break;
		case 'P': piece = wP; break;
		case 'R': piece = wR; break;
		case 'N': piece = wN; break;
		case 'B': piece = wB; break;
		case 'K': piece = wK; break;
		case 'Q': piece = wQ; break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			piece = EMPTY_SQUARE;
			count = *fen - '0';
			break;

		case '/':
		case ' ':
			rank--;
			file = FILE_A;
			fen++;
			continue;

		default:
			printf("FEN error \n");
			return -1;
		}

		for (int counter = 0; counter < count; counter++) {
			sq64 = rank * 8 + file;
			sq120 = SQ120(sq64);
			if (piece != EMPTY_SQUARE) {
				pos->pieces[sq120] = piece;
			}
			file++;
		}
		fen++;
	}

	pos->side_to_move = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;

	for (int counter = 0; counter < 4; counter++) {
		if (*fen == ' ') {	// check if we have finished reading castling permission part of fen
			break;
		}
		switch (*fen) {
		case 'K': pos->castle_permission |= wCastleKing; break;
		case 'Q': pos->castle_permission |= wCastleQueen; break;
		case 'k': pos->castle_permission |= bCastleKing; break;
		case 'q': pos->castle_permission |= bCastleQueen; break;
		default:	     break;
		}
		fen++;
	}
	fen++;

	if (*fen != '-') {	// if there is a valid en passant square:
		file = fen[0] - 'a'; 
		rank = fen[1] - '1';

		pos->enPas_sqr = FileAndRank_To_Square(file, rank);
	}

	pos->pos_key = GeneratePosKey(pos);

	return 0;

}

void ResetBoard(S_BOARD* pos) {

	for (int index = 0; index < BRD_SQR_NUM; ++index) {
		pos->pieces[index] = OFF_BOARD;
	}

	for (int index = 0; index < 64; ++index) {
		pos->pieces[SQ120(index)] = EMPTY_SQUARE;
	}

	for (int index = 0; index < 3; ++index) {
		pos->non_pawn_pieces[index] = 0;
		pos->major_pieces[index] = 0;
		pos->minor_pieces[index] = 0;
		pos->pawns[index] = 0ULL;
	}

	for (int index = 0; index < 13; ++index) {
		pos->piece_type_total[index] = 0;
	}

	pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQR;

	pos->side_to_move = BOTH;
	pos->enPas_sqr = NO_SQR;
	pos->fifty_move = 0;

	pos->ply = 0;
	pos->hisPly = 0;

	pos->castle_permission = 0;

	pos->pos_key = 0ULL;

}

void PrintBoard(const S_BOARD* pos) {

	int sq, piece;

	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ", rank + 1);
		for (int file = FILE_A; file <= FILE_H; file++) {
			sq = FileAndRank_To_Square(file, rank);
			piece = pos->pieces[sq];
			printf("%3c", PceChar[piece]);
		}
		printf("\n");
	}

	printf("\n   ");
	for (int file = FILE_A; file <= FILE_H; file++) {
		printf("%3c", 'a' + file);
	}
	printf("\n");

	/*
	printf("castle:%c%c%c%c\n",
		pos->castle_permission & wCastleKing ? 'K' : '-',
		pos->castle_permission & wCastleQueen ? 'Q' : '-',
		pos->castle_permission & bCastleKing ? 'k' : '-',
		pos->castle_permission & bCastleQueen ? 'q' : '-'
	);
	*/
}