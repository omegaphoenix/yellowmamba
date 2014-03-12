#include "player.h"
#include "stdio.h"
Board *b;
Side s;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) 
{
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    b = new Board();
    s = side;
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() 
{
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) 
{
    if (testingMinimax)
    {
        char data[] = 
        {'a','a','a','a','a','a','a','a',
        'a','a','a','a','a','a','a','a',
        'a','b','a','a','a','a','a','a',
        'b','w','b','b','b','b','a','a',
        'a','a','a','a','a','a','a','a',
        'a','a','a','a','a','a','a','a',
        'a','a','a','a','a','a','a','a',
        'a','a','a','a','a','a','a','a'};
        b->setBoard(data);
        return doMinimax(opponentsMove, msLeft);
    }
    return mamba1(opponentsMove, msLeft);
}

Move *Player::mamba1alphaBeta(Move *opponentsMove, int msLeft)
{
    int depth = 4;
    bool maximizing = (s==WHITE); 
    double alpha = -99999;
    double beta = 99999;
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    std::vector<Move*> posMoves = getPossibleMoves(b,s);
    if(posMoves.empty())
    {
        return NULL;
    }
    if(posMoves.size() == 1)
    {
        b->doMove(posMoves[0], s);
        return posMoves[0];
    }
    Move *move;
    for(std::vector<Move*>::iterator it = posMoves.begin(); it != posMoves.end(); ++it)
    {
        Board *c = b->copy();
        c->doMove(*it, s);
        double val = alphaBeta(c, depth, not maximizing,alpha,beta);
        if (maximizing) 
        {
            if (val > alpha)
            {
                alpha = val;
                move = *it; 
            }
        }
        else
        {
            if (val < beta)
            {
                beta = val;
                move = *it;
            }
        }
    }
    b->doMove(move, s);
    return move;
}

double Player::alphaBeta(Board* b, int depth, bool maximizing, double alpha, double beta)
{
    if (depth == 0)
    {
        return value(b); 
    }
    if (maximizing) 
    {
        std::vector<Move*> posMoves = getPossibleMoves(b, WHITE); 
        if (posMoves.empty())
        {
            std::vector<Move*> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        }
        //double alpha = -99999;
        //double beta = 99999;
        double val;
        for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(*it, WHITE);
            val = alphaBeta(c, depth - 1, false, alpha, beta);
            if (val > alpha)
            {
                alpha = val;
                if(beta <= alpha)
                {
                    break;
                }
            }
            delete c;
            delete *it;
        }
        return alpha;
    }
    else
    {
        std::vector<Move*> posMoves = getPossibleMoves(b, BLACK);
        if (posMoves.empty())
        {
            std::vector<Move*> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        } 
        //double bestValue = 100000;
        double val;
        for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(*it, BLACK);
            val = alphaBeta(c, depth - 1, true, alpha, beta);
            if (val < beta)
            {
                beta = val;
                if(beta <= alpha)
                {
                    break;
                }
            }
            delete c;
            delete *it;
        }
        return beta;
    }
}

Move *Player::mamba1(Move *opponentsMove, int msLeft)
{
    int depth = 4;
    bool maximizing; 
    double bestValue;
    if (s == WHITE)
    {
        maximizing = true;
        bestValue = -100000;
    }
    else
    {
        maximizing = false;
        bestValue = 100000;
    }
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    std::vector<Move*> posMoves = getPossibleMoves(b,s);
    if(posMoves.empty())
    {
        return NULL;
    }
    if(posMoves.size() == 1)
    {
        b->doMove(posMoves[0], s);
        return posMoves[0];
    }
    Move *move;
    for(std::vector<Move*>::iterator it = posMoves.begin(); it != posMoves.end(); ++it)
    {
        Board *c = b->copy();
        c->doMove(*it, s);
        double val = recursiveMinMax(c, depth, not maximizing);
        if (maximizing) 
        {
            if (val > bestValue)
            {
                bestValue = val;
                move = *it; 
            }
        }
        else
        {
            if (val < bestValue)
            {
                bestValue = val;
                move = *it;
            }
        }
    }
    b->doMove(move, s);
    return move;
}

double Player::recursiveMinMax(Board* b, int depth, bool maximizing)
{
    if (depth == 0)
    {
        return value(b); 
    }
    if (maximizing) 
    {
        std::vector<Move*> posMoves = getPossibleMoves(b, WHITE); 
        if (posMoves.empty())
        {
            std::vector<Move*> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        }
        double bestValue = -100000;
        double val;
        for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(*it, WHITE);
            val = recursiveMinMax(c, depth - 1, false);
            if (val > bestValue)
            {
                bestValue = val;
            }
            delete c;
            delete *it;
        }
        return bestValue;
    }
    else
    {
        std::vector<Move*> posMoves = getPossibleMoves(b, BLACK);
        if (posMoves.empty())
        {
            std::vector<Move*> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        } 
        double bestValue = 100000;
        double val;
        for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(*it, BLACK);
            val = recursiveMinMax(c, depth - 1, true);
            if (val < bestValue)
            {
                bestValue = val;
            }
            delete c;
            delete *it;
        }
        return bestValue;
    }
}

double Player::value(Board *boardState)
{
    double pieceDif;
    double W = boardState->countWhite();
    double B = boardState->countBlack();
    if (W == B)
    {
        pieceDif = 0;
    }
    else if( W > B)
    {
        pieceDif = 100 * W / (B + W);
    }
    else
    {
        pieceDif = -100 * B / (B + W);
    }
    double corners = 25 * (boardState->whiteCorners() - boardState->blackCorners());
    W = boardState->whiteCloseCorner(); 
    B = boardState->blackCloseCorner();
    double cornerClose = 12.5 * (B - W);
    W = boardState->whiteEdges();
    B = boardState->blackEdges();
    double edges = 12.5 * (W - B);
// NOTE: the mobility heuristic greatly slows computation and leads to a malloc error 
/*   std::vector<Move*> blackMoves = getPossibleMoves(boardState, BLACK); 
    std::vector<Move*> whiteMoves = getPossibleMoves(boardState, WHITE);
    B = blackMoves.size();
    W = whiteMoves.size(); 
    double mobility;
    if (W == B)
    {
        mobility = 0;
    }
    else if (W > B)
    {
        mobility = 100 * W / (B + W);
    }
    else 
    {
        mobility = -100 * B / (B + W);
    }
*/

    return 10 * pieceDif + 801.724 * corners + 382.026 * cornerClose + 382 * edges; 
// + 78.922 * mobility;
}

Move *Player::doMinimax(Move *opponentsMove, int msLeft)
{
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    std::vector<Move*> posMoves = getPossibleMoves(b,s);
    if(posMoves.empty())
    {
        return NULL;
    }
    if(posMoves.size()==1)
    {
        b->doMove(posMoves[0],s);
        return posMoves[0];
    }
    std::vector<int> maximumMin;
    std::vector<Move*> oppPosMoves;
    for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
    {
        Board *c = b->copy();
        c->doMove(*it,s);
        oppPosMoves = getPossibleMoves(c,other);
        if(oppPosMoves.empty())
        {
            maximumMin.push_back(score(c));
        }
        else
        {
            int min = 65;
            for(std::vector<Move*>::iterator iter = oppPosMoves.begin(); iter!=oppPosMoves.end(); ++iter)
            {
                Board *d = c->copy();
                d->doMove(*iter,other);
                int temp = score(d);
                if(temp<min)
                {
                    min = temp;
                }
            }
            maximumMin.push_back(min);
        }
    }
    Move *ans = posMoves[0];
    int max = maximumMin[0];
    int i = 0;
    for(std::vector<Move*>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
    {
        if(maximumMin[i]>max)
        {
            ans = *it;
            max = maximumMin[i];
        }
        i++;
    }
    b->doMove(ans,s);
    return ans;
}

int Player::score(Board *boardState)
{
    int ans = boardState->countWhite() - boardState->countBlack();
    if(s == WHITE)
    {
        return ans;
    }
    else
    {
        return -1*ans;
    }
}

std::vector<Move*> Player::getPossibleMoves(Board *boardState,Side side)
{
    std::vector<Move*> posMoves = std::vector<Move*>();
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move *move = new Move(i, j);
            if (boardState->checkMove(move, side))
            {
                posMoves.push_back(move);
            }
        }
    }
    return posMoves;
}

Move *Player::random(Move *opponentsMove, int msLeft)
{
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move *move = new Move(i, j);
            if (b->checkMove(move, s))
            {
                b->doMove(move,s);
                return move;
            }
        }
    }
    return NULL;
}

Move *Player::heuristic(Move *opponentsMove, int msLeft)
{
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    int score = -1000;
    Move *move = NULL;
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move *temp = new Move(i, j);
            if (b->checkMove(temp, s))
            {
                Board *c = b->copy();
                c->doMove(temp,s);
                if (s == BLACK)
                {
                    if ( c->countBlack() - c->countWhite() > score)
                    {
                        score = c->countBlack() - c->countWhite();
                        move = temp;
                    } 
                }
                else
                {
                    if (c->countWhite() - c->countBlack() > score)
                    {
                        score = c->countWhite() - c->countBlack();
                        move = temp;
                    }
                }
                delete c;
            }
        }
    }
    b->doMove(move, s);
    return move;
}
