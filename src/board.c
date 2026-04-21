#include "board.h"

const int NUMBER_OF_FLAGS = 10;
const int BLACK_ANT_ROW = 6;
const int WHITE_ANT_ROW = 1;
const int MIN_ROW = 0;
const int MAX_ROW = 7;
const int MIN_COL = 0;
const int MAX_COL = 9;

Piece_Icon *getBlackImagePiece(Piece_Icon **images, Rank rank)
{
    switch (rank)
    {
    case ROOK:
        return images[BLACK_ROOK];
    case KNIGHT:
        return images[BLACK_KNIGHT];
    case BISHOP:
        return images[BLACK_BISHOP];
    case ANTEATER:
        return images[BLACK_ANTEATER];
    case QUEEN:
        return images[BLACK_QUEEN];
    case KING:
        return images[BLACK_KING];
    case ANT:
        return images[BLACK_ANT];
    case EMPTY:
        return NULL;
    default:
        return NULL;
    }
}

Piece_Icon *getWhiteImagePiece(Piece_Icon **images, Rank rank)
{
    switch (rank)
    {
    case ROOK:
        return images[WHITE_ROOK];
    case KNIGHT:
        return images[WHITE_KNIGHT];
    case BISHOP:
        return images[WHITE_BISHOP];
    case ANTEATER:
        return images[WHITE_ANTEATER];
    case QUEEN:
        return images[WHITE_QUEEN];
    case KING:
        return images[WHITE_KING];
    case ANT:
        return images[WHITE_ANT];
    case EMPTY:
        return NULL;
    default:
        return NULL;
    }
}

Piece_Icon *getImagePiece(Piece_Icon **images, Color color, Rank rank)
{
    return (color == BLACK) ? getBlackImagePiece(images, rank) : getWhiteImagePiece(images, rank);
}

Piece *createPiecePtr(Piece_Icon *img, Color color, Rank rank, Pos pos)
{
    Piece *temp = malloc(sizeof(Piece));
    // check if malloc NULL

    pieceCtor(temp, img, color, rank, pos, NULL);
    return temp;
}

void createPieces(Board *mBoard, Piece_Icon **images, Color color)
{
    int order[BOARD_WIDTH] = {ROOK, KNIGHT, BISHOP, ANTEATER, QUEEN, KING, ANTEATER, BISHOP, KNIGHT, ROOK};
    int row = (color == BLACK) ? MIN_ROW : MAX_ROW;
    int rank;
    Pos pos;
    Piece *piece;
    Piece_Icon *image;

    for (int col = 0; col < BOARD_WIDTH; col++)
    {
        posCtor(&pos, row, col);
        rank = order[col];
        image = getImagePiece(images, color, rank);
        piece = createPiecePtr(image, color, rank, pos);
        addPiece(mBoard, piece, pos);
    }
}

void createWhitePieces(Board *mBoard, Piece_Icon **images)
{
    createPieces(mBoard, images, WHITE);
}

void createBlackPieces(Board *mBoard, Piece_Icon **images)
{
    createPieces(mBoard, images, BLACK);
}

void createAnts(Board *mBoard, Piece_Icon **images, Color color)
{
    int row = (color == BLACK) ? BLACK_ANT_ROW : WHITE_ANT_ROW;
    Piece_Icon *image = (color == BLACK) ? getBlackImagePiece(images, ANT) : getWhiteImagePiece(images, ANT);
    Piece *piece;
    Pos pos;
    for (int col = 0; col < BOARD_WIDTH; col++)
    { // initialize black ants
        posCtor(&pos, row, col);
        piece = (color == BLACK) ? createPiecePtr(image, BLACK, ANT, pos) : createPiecePtr(image, WHITE, ANT, pos);
        addPiece(mBoard, piece, pos);
    }
}

void createBlackAnts(Board *mBoard, Piece_Icon **images)
{
    createAnts(mBoard, images, BLACK);
}

void createWhiteAnts(Board *mBoard, Piece_Icon **images)
{
    createAnts(mBoard, images, WHITE);
}

// BOARD FUNCTIONS
void defaultInitializeBoard(Board *mBoard)
{
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int col = 0; col < BOARD_WIDTH; col++)
        {
            mBoard->board[row][col] = NULL;
        }
    }
}

void initializeBoard(Board *mBoard, Piece_Icon **images)
{ // temporarily we will just alloc for pawns
    createBlackAnts(mBoard, images);
    createWhiteAnts(mBoard, images);
    createBlackPieces(mBoard, images);
    createWhitePieces(mBoard, images);
}

// BOARD FUNCTIONS

bool hasPiece(const Board *board, int row, int col)
{
    return board->board[row][col];
}

bool isRowEdge(int row)
{
    return (row == MIN_ROW || row >= MAX_ROW);
}

bool isColEdge(int col)
{
    return (col <= MIN_COL || col >= MAX_COL);
}

bool isBoardEdge(int row, int col)
{
    return (isRowEdge(row) || isColEdge(col));
}

Piece *getSquare(const Board *board, int row, int col)
{
    return board->board[row][col];
}

void setSquare(Board *board, Piece *piece, int row, int col)
{
    board->board[row][col] = piece;
}

void deletePiece(Board *board, Pos pos)
{
    board->board[pos.row][pos.col] = NULL;
}

void addPiece(Board *board, Piece *piece, Pos pos)
{
    setSquare(board, piece, pos.row, pos.col);
    piece->pos = pos;
}

void movePiece(Board *board, Pos oldPos, Pos newPos)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool aPieceWasClicked(const Board_State *boardState)
{
    return boardState->clickedPiece;
}

bool newPieceWasClicked(const Board_State *boardState, Piece *clickedPiece)
{
    return (clickedPiece != boardState->clickedPiece && clickedPiece != NULL);
}

bool hasUpdate(const Board_State *boardState)
{
    return boardState->hasUpdate;
}

void setUpdate(Board_State *boardState, bool update)
{
    boardState->hasUpdate = update;
}

Piece *getClickedPiece(const Board_State *boardState)
{
    return boardState->clickedPiece;
}

void setClickedPiece(Board_State *boardState, Piece *piece)
{
    boardState->clickedPiece = piece;
}

void resetClickedPiece(Board_State *boardState)
{
    setClickedPiece(boardState, NULL);
}

void sendInput(Board *board, Board_State *boardState, Pos clickPos)
{

    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    } // making sure input is valid

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getSquare(board, row, col);

    if (hasPiece(board, row, col))
    { // if the square we clicked has a piece
        printf("new piece was clicked!\n");
        setClickedPiece(boardState, clicked);
        setUpdate(boardState, false); // for now is false since we haven't implemented move highlighting
    }
    else if (aPieceWasClicked(boardState))
    { // square was clicked // but we clicked a piece before //set update flag
        printf("a piece is moving\n");
        Piece *clickedPiece = getClickedPiece(boardState);
        Pos oldPos = getPos(clickedPiece);
        addPiece(board, clickedPiece, clickPos);
        deletePiece(board, oldPos);
        resetClickedPiece(boardState);
        setUpdate(boardState, true);
    }
}