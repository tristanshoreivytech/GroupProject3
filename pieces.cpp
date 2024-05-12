#include "pieces.h"

#include <iostream>
#include <string>

PieceColor Pieces::getPieceColor(Piece p) {
    int color = p >> 3;


    if (color == 1) {
        return PEARL;
    }
    else if (color == 2) {
        return DIAMOND;
    }
    else {
        return NOCOLOR;
    }
}

PieceClass Pieces::getPieceClass(Piece p) {
    int pc = p & 7;

    return static_cast<PieceEnum>(pc);
}



std::string Pieces::getPieceFilename(Piece p) {
    PieceColor color = getPieceColor(p);
    PieceClass pc = getPieceClass(p);

    std::string filename;

    switch (color) {
    case DIAMOND:
        filename += "D";
        break;
    case PEARL:
        filename += "P";
        break;
    }

    switch (pc) {
    case PAWN:
        filename += "Pawn";
        break;
    case KNIGHT:
        filename += "Knight";
        break;
    case BISHOP:
        filename += "Bishop";
        break;
    case ROOK:
        filename += "Rook";
        break;
    case QUEEN:
        filename += "Queen";
        break;
    case KING:
        filename += "King";
        break;
    }

    filename += ".png";

    return filename;
}

Piece Pieces::makePiece(PieceClass c, PieceColor pc) {
    return c | pc;
}