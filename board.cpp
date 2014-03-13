#include "board.h"

int disc [16] =
    {20, -3, 11, 8,
    -3, -7, -4, 1,
    11, -4, 2, 2,
    8, 1, 2, -3};

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::blackOccupied(int x, int y) {
    return black[x + 8 * y]; 
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 *
 */
int Board::countCorners(Side side) 
{
    return (side == BLACK) ? blackCorners() : whiteCorners();
}

/*
 * Current count of black corners.
 */
int Board::blackCorners()
{
    int ans = 0;
    if (black[0 + 8 * 0])
    {
        ans++;
    }
    if (black[0 + 8 * 7])
    {
        ans++;
    }
    if (black[7 + 8 * 0])
    {
        ans++;
    }
    if (black[7 + 8 * 7])
    {
        ans++;
    }
    return ans;
} 

/*
 * Current count of white corners.
 */
int Board::whiteCorners()
{
    int ans = 0;
    if (taken[0 + 8 * 0])
    {
        ans++;
    }
    if (taken[0 + 8 * 7])
    {
        ans++;
    }
    if (taken[7 + 8 * 0])
    {
        ans++;
    }
    if (taken[7 + 8 * 7])
    {
        ans++;
    }
    return ans - blackCorners();
}

int Board::bitCloseCorner(bitset<64> bit)
{
    int ans = 0;
    if (bit[0 + 8 * 1])
    {
        ans++;
    }
    if (bit[1 + 8 * 0])
    {
        ans++;
    }
    if (bit[1 + 8 * 1])
    {
        ans++;
    }
    if (bit[0 + 8 * 6])
    {
        ans++;
    }
    if (bit[1 + 8 * 6])
    {
        ans++;
    }
    if (bit[1 + 8 * 7])
    {
        ans++;
    }
    if (bit[6 + 8 * 0])
    {
        ans++;
    }
    if (bit[6 + 8 * 1])
    {
        ans++;
    }
    if (bit[7 + 8 * 1])
    {
        ans++;
    }
    if (bit[6 + 8 * 6])
    {
        ans++;
    }
    if (bit[7 + 8 * 6])
    {
        ans++;
    }
    if (bit[6 + 8 * 7])
    {
        ans++;
    }
    return ans;
} 

int Board::blackCloseCorner()
{
    int ans = bitCloseCorner(black);
    return ans;
}

int Board::whiteCloseCorner()
{
    int black = blackCloseCorner();
    return bitCloseCorner(taken) - black;
}

int Board::bitEdges(bitset<64> bit)
{
    int ans = 0;
    for (int i = 2; i < 5; ++i)
    {
        ans += bit[i + 8 * 0] + bit[i + 8 * 7];
    }
    for (int i = 2; i < 5; ++i)
    {
        ans += bit[0 + 8 * i] + bit[7 + 8 * i];
    }
    return ans;  
}

int Board::blackEdges()
{
    int ans = bitEdges(black);
    return ans;
} 

int Board::whiteEdges()
{
    int black = blackEdges();
    return bitEdges(taken) - black;
}

int Board::discScore() {
    int x, y;
    int disks = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (occupied(i, j)) {
                x = i;
                y = j;
                if (i > 3) {
                    x = 7 - i;
                }
                if (j > 3) {
                    y = 7 - j;
                }
                if (blackOccupied(i, j)) {
                    disks -= disc[x + 4 * y];
                }
                else {
                    disks += disc[x + 4 * y];
                }
            }
        }        
    }
    return disks;
}

bool Board::isFrontier(int i, int j) {
    if (occupied(i, j)) {
        if (i == 0) {
            if (j != 0 and j != 7) {
                if (not occupied(i + 1, j)) {
                    return true; 
                }
                else if (not occupied(i, j + 1)) {
                    return true; 
                }
                else if (not occupied(i, j - 1)) {
                    return true;
                }
                else if (not occupied(i + 1, j + 1)) {
                    return true;
                }
                else if (not occupied(i + 1, j - 1)) {
                    return true;
                }
            }
        }
        else if (i == 7) {
            if (j != 0 and j != 7) {
                if (not occupied(i - 1, j)) {
                    return true; 
                }
                else if (not occupied(i, j + 1)) {
                    return true; 
                }
                else if (not occupied(i, j - 1)) {
                    return true;
                }
                else if (not occupied(i - 1, j + 1)) {
                    return true;
                }
                else if (not occupied(i - 1, j - 1)) {
                    return true;
                }
            }
        }
        else {
            if (j == 0) {
                if (not occupied(i - 1, j + 1)) {
                    return true; 
                }
                else if (not occupied(i, j + 1)) {
                    return true; 
                }
                else if (not occupied(i + 1, j + 1)) {
                    return true;
                }
                else if (not occupied(i - 1, j)) {
                    return true;
                }
                else if (not occupied(i + 1, j)) {
                    return true;
                }
            }
            else if (j == 7) {
	        if (not occupied(i - 1, j - 1)) {
                    return true; 
                }
                else if (not occupied(i, j - 1)) {
                    return true; 
                }
                else if (not occupied(i + 1, j - 1)) {
                    return true;
                }
                else if (not occupied(i - 1, j)) {
                    return true;
                }
                else if (not occupied(i + 1, j)) {
                    return true;
                }
            }
            else {
                if (not occupied(i - 1, j - 1)) {
                    return true; 
                }
                else if (not occupied(i, j - 1)) {
                    return true; 
                }
                else if (not occupied(i + 1, j - 1)) {
                    return true;
                }
                else if (not occupied(i - 1, j)) {
                    return true;
                }
                else if (not occupied(i + 1, j)) {
                    return true;
                }
                else if (not occupied(i - 1, j + 1)) {
                    return true; 
                }
                else if (not occupied(i, j + 1)) {
                    return true; 
                }
                else if (not occupied(i + 1, j + 1)) {
                    return true;
                }
            }
        }   
    }
    return false;
}

int Board::frontierSquares() {
    int ans = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (isFrontier(i, j)) {
                ans++;
            }
        }
    }
    return ans;
}

int Board::blackFrontierSquares() {
    int ans = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (isFrontier(i, j) and blackOccupied(i, j)) {
                ans++;
            }
        }
    }
    return ans;
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
