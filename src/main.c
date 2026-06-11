#include <raylib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define BLOCKS 8
#define BLOCK_SIZE (SCREEN_WIDTH / BLOCKS)


typedef struct Position {
    int x;
    int y;
} Position;

typedef enum PieceType {
    W_PAWN, B_PAWN, W_KNIGHT, B_KNIGHT,
    W_BISHOP, B_BISHOP, W_ROOK, B_ROOK,
    W_QUEEN, B_QUEEN, W_KING, B_KING,
    EMPTY,
    PIECE_COUNT
} PieceType;

typedef struct ChessPiece {
    PieceType type;
    Position position;
} ChessPiece;

typedef struct ChessPieces {
    Texture2D textures[PIECE_COUNT];
} ChessPieces;

void LoadPieces (ChessPieces* pieces) {
    pieces->textures[W_PAWN] = LoadTexture("img/wp.png");
    pieces->textures[B_PAWN] = LoadTexture("img/wp.png");
    pieces->textures[W_KNIGHT] = LoadTexture("img/wp.png");
    pieces->textures[B_KNIGHT] = LoadTexture("img/wp.png");
    pieces->textures[W_BISHOP] = LoadTexture("img/wp.png");
    pieces->textures[B_BISHOP] = LoadTexture("img/wp.png");
    pieces->textures[W_ROOK] = LoadTexture("img/wp.png");
    pieces->textures[B_ROOK] = LoadTexture("img/wp.png");
    pieces->textures[W_QUEEN] = LoadTexture("img/wp.png");
    pieces->textures[B_QUEEN] = LoadTexture("img/wp.png");
    pieces->textures[W_KING] = LoadTexture("img/wp.png");
    pieces->textures[B_KING] = LoadTexture("img/wp.png");
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



int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(60);
    ChessPieces chessPieces;
    LoadPieces(&chessPieces);

    
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RED);

        drawBoard();

        EndDrawing();
    }

    UnloadPieces(&chessPieces);

    CloseWindow();
    return 0;
}