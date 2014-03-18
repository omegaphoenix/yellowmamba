#include "player.h"
#include "stdio.h"
#include <sys/time.h>
Board *b;
Side s;
static struct timeval start_time;
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
    return iterativeDeepening(opponentsMove, msLeft);
}

Move *Player::iterativeDeepening(Move *opponentsMove, int msLeft)
{
    gettimeofday(&start_time,NULL);
    struct timeval t;
    int depth = 9;
    bool maximizing = (s==WHITE); 
    double alpha = -99999;
    double beta = 99999;
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    std::vector<Move> posMoves = getPossibleMoves(b,s);
    std::vector<double> valueTrack;
    if(posMoves.empty())
    {
        return NULL;
    }
    if(posMoves.size() == 1)
    {
        b->doMove(&posMoves[0], s);
        Move *ans = new Move(posMoves[0].x,posMoves[0].y);
        return ans;
    }
    Move move(posMoves[0].x,posMoves[0].y);
    for(int i = 3; i<depth; i++)
    {
        alpha = -99999;
        beta = 99999;
        for(std::vector<Move>::iterator it = posMoves.begin(); it != posMoves.end(); ++it)
        {
            gettimeofday(&t,NULL);
            if(((t.tv_sec - start_time.tv_sec)*1000000 + t.tv_usec-start_time.tv_usec)/1000.0>msLeft/(b->movesLeft()))
            {
                Move *ans = new Move(posMoves[0].x,posMoves[0].y);
                b->doMove(ans,s);
                std::cerr << i << endl;
                return ans;
            }
            Board *c = b->copy();
            c->doMove(&(*it), s);
            double val = alphaBeta(c, i+1, not maximizing,alpha,beta);
            valueTrack.push_back(val);
            if (maximizing) 
            {
                if (val > alpha)
                {
                    alpha = val;
                    move = (*it);
                }
            }
            else
            {
                if (val < beta)
                {
                    beta = val;
                    move = (*it);
                }
            }
            delete c;
        }
        if(maximizing)
        {
            for(unsigned int k = 1; k <posMoves.size(); k++)
            {
                int j=k;
                double temp = valueTrack[k];
                Move tempMove = posMoves[k];
                while(j>0 && valueTrack[j-1] < temp)
                {
                    valueTrack[j] = valueTrack[j-1];
                    posMoves[j] = posMoves[j-1];
                    j--;
                }
                valueTrack[j] = temp;
                posMoves[j] = tempMove;
            }
        }
        else
        {
            for(unsigned int k = 1; k <posMoves.size(); k++)
            {
                int j=k;
                double temp = valueTrack[k];
                Move tempMove = posMoves[k];
                while(j>0 && valueTrack[j-1] > temp)
                {
                    valueTrack[j] = valueTrack[j-1];
                    posMoves[j] = posMoves[j-1];
                    j--;
                }
                valueTrack[j] = temp;
                posMoves[j] = tempMove;
            }
        }
        valueTrack.clear();
    }
    Move *ans = new Move(posMoves[0].x,posMoves[0].y);
    b->doMove(ans, s);
    return ans;
}

Move *Player::mamba1alphaBeta(Move *opponentsMove, int msLeft)
{
    int depth = 6;
    bool maximizing = (s==WHITE); 
    double alpha = -99999;
    double beta = 99999;
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b->doMove(opponentsMove,other);
    }
    std::vector<Move> posMoves = getPossibleMoves(b,s);
    if(posMoves.empty())
    {
        return NULL;
    }
    if(posMoves.size() == 1)
    {
        b->doMove(&posMoves[0], s);
        Move *ans = new Move(posMoves[0].x,posMoves[0].y);
        return ans;
    }
    Move move(posMoves[0].x,posMoves[0].y);
    for(std::vector<Move>::iterator it = posMoves.begin(); it != posMoves.end(); ++it)
    {
        Board *c = b->copy();
        c->doMove(&(*it), s);
        double val = alphaBeta(c, depth, not maximizing,alpha,beta);
        if (maximizing) 
        {
            if (val > alpha)
            {
                alpha = val;
                move = (*it);
            }
        }
        else
        {
            if (val < beta)
            {
                beta = val;
                move = (*it);
            }
        }
        delete c;
    }
    Move *ans = new Move(move.x,move.y);
    b->doMove(ans, s);
    return ans;
}

double Player::alphaBeta(Board* b, int depth, bool maximizing, double alpha, double beta)
{
    if (depth == 0)
    {
        return value(b); 
    }
    if (maximizing) 
    {
        std::vector<Move> posMoves = getPossibleMoves(b, WHITE); 
        if (posMoves.empty())
        {
            std::vector<Move> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        }
        double val;
        for(std::vector<Move>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(&(*it), WHITE);
            val = alphaBeta(c, depth - 1, false, alpha, beta);
            if (val > alpha)
            {
                alpha = val;
                if(beta <= alpha)
                {
                    delete c;
                    break;
                }
            }
            delete c;
        }
        return alpha;
    }
    else
    {
        std::vector<Move> posMoves = getPossibleMoves(b, BLACK);
        if (posMoves.empty())
        {
            std::vector<Move> oppMoves = getPossibleMoves(b, BLACK);
            if (oppMoves.empty())
            {
                return 1000 * (b->countWhite() - b->countBlack());
            }
            return value(b);
        } 
        double val;
        for(std::vector<Move>::iterator it = posMoves.begin(); it!=posMoves.end(); ++it)
        {
            Board *c = b->copy();
            c->doMove(&(*it), BLACK);
            val = alphaBeta(c, depth - 1, true, alpha, beta);
            if (val < beta)
            {
                beta = val;
                if(beta <= alpha)
                {
                    delete c;
                    break;
                }
            }
            delete c;
        }
        return beta;
    }
}

Move *Player::mamba1(Move *opponentsMove, int msLeft)
{
    /*int depth = 4;
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
    return move;*/
    return NULL;
}

double Player::recursiveMinMax(Board* b, int depth, bool maximizing)
{
    /*if (depth == 0)
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
    }*/
    return 0.0;
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
    B = numMoves(boardState, BLACK);
    W = numMoves(boardState, WHITE); 
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
    B = boardState->blackFrontierSquares();
    W = boardState->frontierSquares() - B;
    double frontierScore;
    if (W == B)
    {
        frontierScore = 0;
    }
    else if (W > B)
    {
        frontierScore = -100 * W / (B + W);
    }
    else 
    {
        frontierScore = 100 * B / (B + W);
    }
    double discScore = boardState->discScore();
    return 10 * pieceDif + 801.724 * corners + 382.026 * cornerClose
     + 380 * edges
    + 78.922 * mobility + 74.396 * frontierScore + 10 * discScore;
}

Move *Player::doMinimax(Move *opponentsMove, int msLeft)
{
    /*Side other = (s == BLACK) ? WHITE : BLACK;
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
    return ans;*/
    return NULL;
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

std::vector<Move> Player::getPossibleMoves(Board *boardState,Side side)
{
    std::vector<Move> posMoves = std::vector<Move>();
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move move = Move(i, j);
            if (boardState->checkMove(&move, side))
            {
                posMoves.push_back(move);
            }
        }
    }
    return posMoves;
}

int Player::numMoves(Board *boardState, Side side)
{
    int num = 0;
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move move = Move(i, j);
            if (boardState->checkMove(&move, side))
            {
                num++;
            }
        }
    }
    return num;
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
