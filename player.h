#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
#include <map>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    Move *iterativeDeepening(Move *opponentsMove, int msLeft);
    double alphaBeta(Board *b, int depth, bool maximizing, double alpha, double beta);
    Move *mamba1(Move *opponentsMove, int msLeft);
    Move *mamba1alphaBeta(Move *opponentsMove, int msLeft);
    double recursiveMinMax(Board *b, int depth, bool maximizing);
    double value(Board *boardState);
    Move *doMinimax(Move *opponentsMove, int msLeft);
    int score(Board *boardState);
    std::vector<Move> getPossibleMoves(Board *boardState, Side side);
    int numMoves(Board *boardState, Side side);
    Move *random(Move *opponentsMove, int msLeft);
    Move *heuristic(Move *opponentsMove, int msLeft);
    std::map<Board*, MoveDepth> bestMoveTable;
    std::map<Board*, MoveDepth> worstMoveTable;
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    
};

#endif
