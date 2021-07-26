#ifndef DEFS_H
#define DEFS_H
#include <array>
#include "stdlib.h"
using namespace std;

typedef unsigned long long U64; // For 'Bitboards'. Use for each piece type, 0 for an empty square, 1 for a filled square. 

#define NAME "Chess.com Technical Project"
#define BRD_SQR_NUM 120

#define MAXGAMEMOVES 2048

#define START_FEN  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum {EMPTY_SQUARE, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};
enum {WHITE, BLACK, BOTH};
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQR, OFF_BOARD
};

enum {wCastleKing = 1, wCastleQueen = 2, bCastleKing = 4, bCastleQueen = 8};
// for example 0110 tells us white can casyle queenside and black can castle kingside

typedef struct {

	int move;
	int castle_permission;
	int enPas_sqr;
	int fifty_move;

	U64 pos_key;

} S_UNDO;

typedef struct {
	
	int pieces[BRD_SQR_NUM];
	U64 pawns[3]; 

	int KingSq[2];
	int side_to_move;
	int enPas_sqr;
	int fifty_move;
	int ply;
	int hisPly;

	int castle_permission;

	U64 pos_key;

	int piece_type_total[13]; // 13 because of the extra 'EMPTY_SQUARE' 
	int non_pawn_pieces[2]; // holds white, black and both totals of pieces that aren't pawns
	int minor_pieces[2]; // knights and bishops
	int major_pieces[2]; // rooks, queens and kings
	int material[2];

	S_UNDO history[MAXGAMEMOVES];

	// piece list
	int pList[13][10]; // first field is piece type, second field is number of that piece
	
	// pList[wB][0] = C1; -> adds a white bishop to C1;	

} S_BOARD;

// MACROS

#define FileAndRank_To_Square(file, rank)	((21+file) + (rank*10))
#define SQ64(sq120) Sq120ToSq64[sq120]
#define SQ120(sq64) Sq64ToSq120[sq64]
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb, sq) (bb &= ClearMask[sq])
#define SETBIT(bb, sq) (bb |= SetMask[sq])

#define IsBQ(p) (PieceBishopQueen[p])
#define IsRQ(p) (PieceRookQueen[p])
#define IsKn(p) (PieceKnight[p])
#define IsKi(p) (PieceKing[p])

// GLOBALS

extern int Sq120ToSq64[BRD_SQR_NUM];
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];

extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13]; // non pawn pieces
extern int PieceMaj[13]; 
extern int PieceMin[13]; 
extern int PieceVal[13];
extern int PieceCol[13];

extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];

// FUNCTIONS

extern void AllInit();

// Bitboards.cpp

extern void PrintBitBoard(U64 bb);
extern int PopBit(U64* bb);
extern int CountBits(U64 b);

// haskkeys.cpp

extern U64 GeneratePosKey(const S_BOARD *pos);

// Board.cpp

extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD* pos);
extern int MaterialDifference(S_BOARD* pos);

// attack.cpp

extern array<int, 16> SqAttacked(const int sq, const int side, const S_BOARD* pos);

#endif
