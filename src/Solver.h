#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <unordered_set>
#include "BitBoard.h"

namespace RUSH {

class Solver {
   private:
    BitBoard initBoard;
    std::vector<Node> BFS_Q;
    std::unordered_set<idPair, myHash> visitedNodes;

   public:
    Solver() = default;
    void setBoard(const BitBoard &board);
    void MoveLeft(int m, int k, int l, size_t idx, unsigned char mask);
    void MoveRight(int m, int k, int r, size_t idx, unsigned char mask);
    std::pair<bool, std::vector<BitBoard> *> solve();
};

}  // namespace RUSH

#endif