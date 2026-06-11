#include <raylib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define BLOCKS 8
#define BLOCK_SIZE (SCREEN_WIDTH / BLOCKS)

const char* init_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

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

void drawPiece(Texture2D texture, int file, int rank) {
    DrawTexturePro(
        texture,
        (Rectangle) {0, 0, (float)texture.width, (float)texture.height},
        (Rectangle) {
            file * BLOCK_SIZE,
            rank * BLOCK_SIZE,
            BLOCK_SIZE,
            BLOCK_SIZE
        },
        (Vector2) {0, 0},
        0.0f,
        WHITE
    );
}

void fenParseBoard(const char *board, ChessPieces *pieces) {
    int rank = 0, file = 0;

    for (int i = 0; board[i] != '\0'; i++) {
        char c = board[i];

        if (c == '/') {
            rank++;
            file = 0;
        }
        else if (c >= '1' && c <= '8') {
            file += (c - '0');
        }
        else {
            PieceType p = getPiece(c);
            if (p!= EMPTY) {
                drawPiece(pieces->textures[p], file, rank);
            }
            file++;
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

        fenParseBoard(init_board, &chessPieces);

        EndDrawing();
    }

    UnloadPieces(&chessPieces);

    CloseWindow();
    return 0;
}