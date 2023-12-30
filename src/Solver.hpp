#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <unordered_set>
#include "BitBoard.hpp"

namespace RUSH {

class Solver {
   private:
    BitBoard initBoard;
    std::vector<Node> BFS_Q;
    std::unordered_set<idPair, myHash> visitedNodes;

   public:
    Solver() = default;

    void setBoard(const BitBoard &board) {
        initBoard = board;
    }

    void MoveLeft(int m, int k, int l, size_t idx, unsigned char mask) {
        BitBoard &board = BFS_Q[idx].board;
        BitBoard newBoard(board);
        do {
            newBoard[m][k] &= ~mask;
            newBoard[m][k] >>= 1;
            if (newBoard[m][k] & mask) break;
            newBoard[m][k] |= mask;
            --l;
            if (l < 0 || newBoard.conflict(m, k, l)) break;
            idPair id = std::make_pair(newBoard.rowID(), newBoard.colID());
            if (visitedNodes.count(id)) continue;
            BFS_Q.emplace_back(Node(newBoard, BFS_Q[idx].dis + 1, idx));
            visitedNodes.insert(id);
        } while (1);
    }

    void MoveRight(int m, int k, int r, size_t idx, unsigned char mask) {
        const BitBoard &board = BFS_Q[idx].board;
        BitBoard newBoard(board);
        do {
            newBoard[m][k] &= ~mask;
            newBoard[m][k] <<= 1;
            if (newBoard[m][k] & mask) break;
            newBoard[m][k] |= mask;
            ++r;
            if (r >= 6 || newBoard.conflict(m, k, r)) break;
            idPair id = std::make_pair(newBoard.rowID(), newBoard.colID());
            if (visitedNodes.count(id)) continue;
            BFS_Q.emplace_back(Node(newBoard, BFS_Q[idx].dis + 1, idx));
            visitedNodes.insert(id);
        } while (1);
    }

    std::pair<bool, std::vector<BitBoard>> &solve() {
        static std::pair<bool, std::vector<BitBoard>> solution;
        BFS_Q.emplace_back(Node(initBoard, 0, 0));
        visitedNodes.insert(std::make_pair(initBoard.rowID(), initBoard.colID()));
        size_t idx = 0;
        while (idx != BFS_Q.size()) {
            if (BFS_Q[idx].board.solved()) {
                BitBoard newBoard(BFS_Q[idx].board);
                newBoard[0][2] <<= 5 - BFS_Q[idx].board.last_bit(0, 2);
                BFS_Q.emplace_back(Node(newBoard, BFS_Q[idx].dis + 1, idx));
                idx = BFS_Q.size() - 1;
                do {
                    solution.second.push_back(BFS_Q[idx].board);
                    idx = BFS_Q[idx].last;
                } while (BFS_Q[idx].dis != 0);
                solution.second.push_back(initBoard);
                solution.first = true;
                return solution;
            }
            for (int m = 0; m < 2; ++m)
                for (int k = 0; k < 6; ++k) {
                    int bitCnt = m == 0 ? BFS_Q[idx].board.rowCnt[k] : BFS_Q[idx].board.colCnt[k];
                    switch (bitCnt) {
                        case 2:
                        case 3: {
                            MoveLeft(m, k, BFS_Q[idx].board.first_bit(m, k), idx, 0);
                            MoveRight(m, k, BFS_Q[idx].board.last_bit(m, k), idx, 0);
                            break;
                        }
                        case 4: {
                            int l1 = BFS_Q[idx].board.first_bit(m, k);
                            int r2 = BFS_Q[idx].board.last_bit(m, k);
                            int r1 = l1 + 1;
                            int l2 = r2 - 1;
                            unsigned char mask1 = (1 << l2) | (1 << r2);
                            unsigned char mask2 = (1 << l1) | (1 << r1);
                            MoveLeft(m, k, l1, idx, mask1);
                            MoveRight(m, k, r1, idx, mask1);
                            MoveLeft(m, k, l2, idx, mask2);
                            MoveRight(m, k, r2, idx, mask2);
                            break;
                        }
                        case 5: {
                            int l1 = BFS_Q[idx].board.first_bit(m, k);
                            int r2 = BFS_Q[idx].board.last_bit(m, k);
                            if (BFS_Q[idx].board[m][k] & (1 << 7)) {
                                int r1 = l1 + 2;
                                int l2 = r2 - 1;
                                unsigned char mask1 = (1 << l2) | (1 << r2) | (1 << 7);
                                unsigned char mask2 = (1 << l1) | (1 << (l1 + 1)) | (1 << r1) | (1 << 7);
                                MoveLeft(m, k, l1, idx, mask1);
                                MoveRight(m, k, r1, idx, mask1);
                                MoveLeft(m, k, l2, idx, mask2);
                                MoveRight(m, k, r2, idx, mask2);
                            } else {
                                int r1 = l1 + 1;
                                int l2 = r2 - 2;
                                unsigned char mask1 = (1 << l2) | (1 << (l2 + 1)) | (1 << r2);
                                unsigned char mask2 = (1 << l1) | (1 << r1);
                                MoveLeft(m, k, l1, idx, mask1);
                                MoveRight(m, k, r1, idx, mask1);
                                MoveLeft(m, k, l2, idx, mask2);
                                MoveRight(m, k, r2, idx, mask2);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
            ++idx;
        }
        solution.second.push_back(initBoard);
        solution.first = false;
        return solution;
    }
};

}  // namespace RUSH

#endif