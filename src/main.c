#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

PieceColor getColor(PieceType p) {
    if (p >= W_PAWN && p <= W_KING) return W;
    if (p >= B_PAWN && p <= B_KING) return B;
    return NONE;
}

const char* pieceToString(PieceType p) {
    switch (p) {
        case W_PAWN:   return "WhitePawn";
        case W_KNIGHT: return "WhiteKnight";
        case W_BISHOP: return "WhiteBishop";
        case W_ROOK:   return "WhiteRook";
        case W_QUEEN:  return "WhiteQueen";
        case W_KING:   return "WhiteKing";
        case B_PAWN:   return "BlackPawn";
        case B_KNIGHT: return "BlackKnight";
        case B_BISHOP: return "BlackBishop";
        case B_ROOK:   return "BlackRook";
        case B_QUEEN:  return "BlackQueen";
        case B_KING:   return "BlackKing";
        default:     return "Unknown";
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

void placePiece (GameState *gameState, PieceType piece, int rank, int file) {
    gameState->chessBoard[rank][file] = piece;
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
        (i==0) ? placePiece(state, W_GOLEM, rank, file) : placePiece(state, B_GOLEM, rank, file);
    }
}

// Simplify for just pawns first
bool validPosition(PieceType piece, Position from, Position to) {
    int direction;
    int startRank;

    switch (piece) {
        case W_PAWN:
            direction = 1;
            startRank = 1;
            break;
        case B_PAWN:
            direction = -1;
            startRank = 6;
            break;
        default:
            return false;
    }

    if (to.file != from.file) return false;
    if (to.rank == from.rank + direction) return true;
    if (to.rank == from.rank + 2 * direction && from.rank == startRank) return true;

    return false;
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

void handleInput(GameState *gameState, DragState *drag) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PieceType piece;
        Position clicked;
        clicked.file = GetMouseX() / BLOCK_SIZE;
        clicked.rank = 7 - (GetMouseY() / BLOCK_SIZE);

        piece = gameState->chessBoard[clicked.rank][clicked.file];
        if (piece != EMPTY) {
            drag->isDragging = true;
            drag->piece = piece;
            drag->from = clicked;
            gameState->chessBoard[clicked.rank][clicked.file] = EMPTY;
        }

        printf("File clicked: %d and rank clicked %d\n", clicked.file, clicked.rank);
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drag->isDragging) {
        Position dropped;
        dropped.file = GetMouseX() / BLOCK_SIZE;
        dropped.rank = 7 - (GetMouseY() / BLOCK_SIZE);

        printf("Dropped %s\n", pieceToString(drag->piece));
        
        if(validPosition(drag->piece, drag->from, dropped)){
            gameState->chessBoard[dropped.rank][dropped.file] = drag->piece;
            printf("Updated gamestate!\n");
        }
        drag->isDragging = false;
        drag->piece = EMPTY;

        printf("file dropped: %d and rank dropped %d\n", dropped.file, dropped.rank);
        // Check if position is valid for piece
    }
}

void render(GameState *gameState, ChessPieces *pieces, DragState *drag) {
    ClearBackground(RED);
    drawBoard();
    drawPieces(gameState, pieces);

    if (drag->isDragging) {
        DrawTexturePro(
            pieces->textures[drag->piece],
            (Rectangle){0, 0, pieces->textures[drag->piece].width, pieces->textures[drag->piece].height},
            (Rectangle){GetMouseX() - BLOCK_SIZE/2, GetMouseY() - BLOCK_SIZE/2, BLOCK_SIZE, BLOCK_SIZE},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(60);

    ChessPieces chessPieces;
    GameState gameState;
    DragState dragState = {0};

    LoadPieces(&chessPieces);
    loadBoardFromFEN(&gameState, init_board);
    placeGolems(&gameState);

    while(!WindowShouldClose()) {
        handleInput(&gameState, &dragState);
        BeginDrawing();
            render(&gameState, &chessPieces, &dragState);
        EndDrawing();
    }
    UnloadPieces(&chessPieces);

    CloseWindow();
    return 0;
}