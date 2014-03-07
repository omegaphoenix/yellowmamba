#include "player.h"
#include "stdio.h"
Board b;
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
    b = Board();
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
    if (true) //change to testingMinimax once we have a better method
    {
        //fprintf(stderr,"%d,%d,\n",0,0);
        return random(opponentsMove, msLeft);
    }
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    return NULL;
}
Move *Player::doMinimax(Move *opponentsMove, int msLeft)
{
    return NULL;
}
Move *Player::random(Move *opponentsMove, int msLeft)
{
    Side other = (s == BLACK) ? WHITE : BLACK;
    if(opponentsMove!=NULL)
    {
        b.doMove(opponentsMove,other);
    }
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            Move *move = new Move(i, j);
            if (b.checkMove(move, s))
            {
                b.doMove(move,s);
                return move;
            }
        }
    }
    return NULL;
}
Move *Player::heuristic(Move *opponentsMove, int msLeft)
{
    return NULL;
}
