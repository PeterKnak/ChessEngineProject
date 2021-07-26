#include "stdio.h"
#include "defs.h"

U64 GeneratePosKey(const S_BOARD* pos) {

	U64 finalKey = 0;
	int piece = EMPTY_SQUARE;

	// pieces
	for (int sq = 0; sq < BRD_SQR_NUM; ++sq) {
		piece = pos->pieces[sq];
		if (piece != NO_SQR && piece != EMPTY_SQUARE && piece != OFF_BOARD) {
			finalKey ^= PieceKeys[piece][sq];
		}
	}

	if (pos->side_to_move == WHITE) {
		finalKey ^= SideKey;
	}

	if (pos->enPas_sqr != NO_SQR) {
		finalKey ^= PieceKeys[EMPTY_SQUARE][pos->enPas_sqr];
	}

	finalKey ^= CastleKeys[pos->castle_permission];

	return finalKey;
}