#include "board.h"
#include "board_state.h"

const int NUMBER_OF_FLAGS = 10;
const int BLACK_ANT_ROW = 1;
const int WHITE_ANT_ROW = 6;
const int MIN_ROW = 0;
const int MAX_ROW = 7;
const int MIN_COL = 0;
const int MAX_COL = 9;

Icon *getBlackImagePiece(Icon **images, Rank rank)
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
    case PAWN:
        return images[BLACK_ANT];
    case EMPTY:
        return NULL;
    default:
        return NULL;
    }
}

Icon *getWhiteImagePiece(Icon **images, Rank rank)
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
    case PAWN:
        return images[WHITE_ANT];
    case EMPTY:
        return NULL;
    default:
        return NULL;
    }
}

Icon *getImagePiece(Icon **images, Color color, Rank rank)
{
    return (color == BLACK) ? getBlackImagePiece(images, rank) : getWhiteImagePiece(images, rank);
}

Piece *createPiecePtr(Icon *img, Color color, Rank rank, Pos pos)
{
    Piece *temp = malloc(sizeof(Piece));
    if(temp == NULL){
        printf("ERROR: MALLOC HAS FAILED\n");
        return NULL; 
    }
    
    pieceCtor(temp, img, color, rank, pos);
    return temp;
}

void createPieces(Board *mBoard, Icon **images, Color color)
{
    int order[BOARD_WIDTH] = {ROOK, KNIGHT, BISHOP, ANTEATER, QUEEN, KING, ANTEATER, BISHOP, KNIGHT, ROOK};
    int row = (color == BLACK) ? MIN_ROW : MAX_ROW;
    int rank;
    Pos pos;
    Piece *piece;
    Icon *image;

    for (int col = 0; col < BOARD_WIDTH; col++)
    {
        posCtor(&pos, row, col);
        rank = order[col];
        image = getImagePiece(images, color, rank);
        piece = createPiecePtr(image, color, rank, pos);
        addPiece(mBoard, piece, pos);
    }
}

void createWhitePieces(Board *mBoard, Icon **images)
{
    createPieces(mBoard, images, WHITE);
}

void createBlackPieces(Board *mBoard, Icon **images)
{
    createPieces(mBoard, images, BLACK);
}

void createAnts(Board *mBoard, Icon **images, Color color)
{
    int row = (color == BLACK) ? BLACK_ANT_ROW : WHITE_ANT_ROW;
    Icon *image = (color == BLACK) ? getBlackImagePiece(images, PAWN) : getWhiteImagePiece(images, PAWN);
    Piece *piece;
    Pos pos;
    for (int col = 0; col < BOARD_WIDTH; col++)
    { // initialize black ants
        posCtor(&pos, row, col);
        piece = (color == BLACK) ? createPiecePtr(image, BLACK, PAWN, pos) : createPiecePtr(image, WHITE, PAWN, pos);
        addPiece(mBoard, piece, pos);
    }
}

void createBlackAnts(Board *mBoard, Icon **images)
{
    createAnts(mBoard, images, BLACK);
}

void createWhiteAnts(Board *mBoard, Icon **images)
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

void initializeBoard(Board *mBoard, Icon **images)
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

Piece* getPiece(const Board* board, int row, int col){ 
    return (hasPiece(board, row, col)) ?  getSquare(board, row, col) : NULL; 
}

Piece* getSquare(const Board *board, int row, int col)
{
    return board->board[row][col];
}

void setSquare(Board *board, Piece *piece, int row, int col)
{
    board->board[row][col] = piece;
}

Piece* deletePiece(Board *board, Pos pos)
{
    Piece* delete = board->board[pos.row][pos.col]; 
    board->board[pos.row][pos.col] = NULL;
    return delete; 
}

void addPiece(Board *board, Piece *piece, Pos pos)
{
    setSquare(board, piece, pos.row, pos.col);
    piece->pos = pos;
}

void movePiece(Board *board, Board_State* boardState, Piece* piece, Pos newPos)
{
    deletePiece(board, getPos(piece));
    addPiece(board, piece, newPos);
    resetClickedPiece(boardState);
    resetLegalMoveCount(boardState);
}

void capturePiece(Board* board, Board_State* boardState, Piece* piece, Pos capturePos){
    Piece* deleted = deletePiece(board, capturePos);  
    movePiece(board, boardState, piece, capturePos); 
    free(deleted);
}

void sendInput(Board *board, Board_State *boardState, Pos clickPos)
{

    if (!isPosValid(clickPos))
    {
        printf("invalid pos! -> row: %d col: %d\n", clickPos.row, clickPos.col);
        return;
    } // making sure input is valid

    int row = clickPos.row, col = clickPos.col;

    Piece *clicked = getPiece(board, row, col);

    if(hasPiece(board, row, col) && !isLegalMoveSquare(boardState, clickPos)){ //if square has piece and we havent clicked a piece before --> a newPiece was clicked
        printf("new piece was clicked\n"); 
        setClickedPiece(boardState, clicked);
        generateLegalMoves(boardState, board, clicked, getPos(clicked));  //highlight
        setUpdate(boardState, true);
    }
    else if(hasPiece(board, row, col) && aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)){ //we clicked a piece before and its legal to move there
        printf("capture\n");
        capturePiece(board, boardState, getClickedPiece(boardState), getPos(clicked)); 
        setUpdate(boardState, true);
    }
    else if(aPieceWasClicked(boardState) && isLegalMoveSquare(boardState, clickPos)){
        printf("moving\n"); 
        Piece *clickedPiece = getClickedPiece(boardState);
        movePiece(board, boardState, clickedPiece, clickPos); 
        setUpdate(boardState, true);
    } 

    //if not legal move --> just cancel move --> reset clicked piece
}


void promotion(Board *board, Pos pos, Rank newRank)
{
    Piece *piece = getSquare(board, pos.row, pos.col);
    piece->type = newRank;
}