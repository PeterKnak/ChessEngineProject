#include "defs.h"
#include "stdio.h"
#include "stdlib.h"

#define RAND_64 	((U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60 )  

int Sq120ToSq64[BRD_SQR_NUM];
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];

void InitHashKeys() {

	for (int index = 0; index < 13; ++index) {
		for (int index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for (int index = 0; index < 16; ++index) {
		CastleKeys[index] = RAND_64;
	}

}

void InitBitMasks() {
	
	for (int index = 0; index < 64; index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}

	for (int index = 0; index < 64; index++) {
		SetMask[index] |= 1ULL << index;
		ClearMask[index] = ~SetMask[index];
	}

}

void InitSq120To64() {

	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	
	for (int counter = 0; counter < BRD_SQR_NUM; ++counter) {
		Sq120ToSq64[counter] = 65;
	}

	for (int counter = 0; counter < 64; ++counter) {
		Sq64ToSq120[counter] = 120;
	}

	for (int rank = RANK_1; rank <= RANK_8; ++rank) {
		for (int file = FILE_A; file <= FILE_H; ++file) {
			sq = FileAndRank_To_Square(file, rank); // Get the square of the 120 array
			Sq64ToSq120[sq64] = sq; // Set up index converters
			Sq120ToSq64[sq] = sq64;
			sq64++;
		}
	}

}

void AllInit() {
	InitSq120To64();
	InitBitMasks();
	InitHashKeys();
}
