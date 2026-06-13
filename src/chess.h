#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define BLOCKS 8
#define BLOCK_SIZE (SCREEN_WIDTH / BLOCKS)

const char* init_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

typedef struct Position {
    int rank;
    int file;
} Position;

typedef enum PieceType {
    W_PAWN, B_PAWN, W_KNIGHT, B_KNIGHT,
    W_BISHOP, B_BISHOP, W_ROOK, B_ROOK,
    W_QUEEN, B_QUEEN, W_KING, B_KING,
    W_GOLEM, B_GOLEM,
    EMPTY,
    PIECE_COUNT
} PieceType;

typedef struct ChessPieces {
    Texture2D textures[PIECE_COUNT];
} ChessPieces;

typedef struct GameState {
    PieceType chessBoard[8][8];
    bool whiteToMove;
} GameState;

typedef struct Move {
    PieceType piece;
    int fromRank, fromFile;
    int toRank, toFile;
} Move;

typedef struct DragState {
    bool isDragging;
    PieceType piece;
    Position from;
} DragState;