#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chess.h"

PieceType getPiece(char piece) {
    switch (piece) {
        case 'p' : return W_PAWN;
        case 'n' : return W_KNIGHT;
        case 'b' : return W_BISHOP;
        case 'r' : return W_ROOK;
        case 'q' : return W_QUEEN;
        case 'k' : return W_KING;
        case 'P' : return B_PAWN;
        case 'N' : return B_KNIGHT;
        case 'B' : return B_BISHOP;
        case 'R' : return B_ROOK;
        case 'Q' : return B_QUEEN;
        case 'K' : return B_KING;
        default  : return EMPTY;
    }
}

void LoadPieces (ChessPieces* pieces) {
    pieces->textures[W_PAWN] = LoadTexture("img/wp.png");
    pieces->textures[B_PAWN] = LoadTexture("img/bp.png");
    pieces->textures[W_KNIGHT] = LoadTexture("img/wn.png");
    pieces->textures[B_KNIGHT] = LoadTexture("img/bn.png");
    pieces->textures[W_BISHOP] = LoadTexture("img/wb.png");
    pieces->textures[B_BISHOP] = LoadTexture("img/bb.png");
    pieces->textures[W_ROOK] = LoadTexture("img/wr.png");
    pieces->textures[B_ROOK] = LoadTexture("img/br.png");
    pieces->textures[W_QUEEN] = LoadTexture("img/wq.png");
    pieces->textures[B_QUEEN] = LoadTexture("img/bq.png");
    pieces->textures[W_KING] = LoadTexture("img/wk.png");
    pieces->textures[B_KING] = LoadTexture("img/bk.png");
    pieces->textures[W_GOLEM] = LoadTexture("img/wg.png");
    pieces->textures[B_GOLEM] = LoadTexture("img/bg.png");
}

void UnloadPieces (ChessPieces* pieces) {
    for (int i = 0; i < PIECE_COUNT; i++) {
        UnloadTexture(pieces->textures[i]);
    }
}

void drawBoard() {
    for (int rank = 0; rank < BLOCKS; rank++) {
        for (int file = 0; file < BLOCKS; file++) {
            Color color = ((rank + file) % 2 == 0)
                ? (Color){122, 153, 90, 255}
                : (Color){240, 240, 211, 255};

            DrawRectangle(file * BLOCK_SIZE,
                          rank * BLOCK_SIZE,
                          BLOCK_SIZE,
                          BLOCK_SIZE,
                          color);
        }
    }
}

void drawPiece(Texture2D texture, int file, int rank)
{
    DrawTexturePro(
        texture,
        (Rectangle){0, 0, texture.width, texture.height},
        (Rectangle){
            file * BLOCK_SIZE,
            (7 - rank) * BLOCK_SIZE,
            BLOCK_SIZE,
            BLOCK_SIZE
        },
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

void drawPieces(GameState *gameState, ChessPieces *pieces)
{
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            PieceType p = gameState->chessBoard[rank][file];
            if (p != EMPTY) {
                drawPiece(pieces->textures[p], file, rank);
            }
        }
    }
}

void placeGolems(GameState *state)
{
    for (int i = 0; i < 2; i++)
    {
        int file, rank;

        do {
            file = rand() % 8;
            rank = rand() % 8;
        } while (state->chessBoard[rank][file] != EMPTY);

        state->chessBoard[rank][file] =
            (i == 0) ? W_GOLEM : B_GOLEM;
    }
}

void updateGameState(GameState *gameState, int fromFile, int fromRank, int toFile, int toRank) {
    PieceType pieceToMove = gameState->chessBoard[fromFile][fromRank]; 
    gameState->chessBoard[fromFile][fromRank] = EMPTY;

    gameState->chessBoard[toFile][toRank] = pieceToMove;
}

void loadBoardFromFEN(GameState *state, const char *board)
{
    int rank = 0;
    int file = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            state->chessBoard[i][j] = EMPTY;
        }
    }

    for (int i = 0; board[i] != '\0'; i++) {
        char c = board[i];

        if (c == '/') {
            rank++;
            file = 0;
        }
        else if (c >= '1' && c <= '8') {
            file += c - '0';
        }
        else {
            state->chessBoard[rank][file] = getPiece(c);
            file++;
        }
    }
}



int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(60);

    ChessPieces chessPieces;
    GameState gameState;

    LoadPieces(&chessPieces);
    loadBoardFromFEN(&gameState, init_board);
    placeGolems(&gameState);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RED);

        drawBoard();
        drawPieces(&gameState, &chessPieces);

        EndDrawing();
    }

    UnloadPieces(&chessPieces);

    CloseWindow();
    return 0;
}