#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstring>

namespace RUSH {

class BitBoard {
    unsigned char rowBits[6];
    unsigned char colBits[6];

   public:
    int rowCnt[6];
    int colCnt[6];
    BitBoard();
    BitBoard(const BitBoard &rhs);
    BitBoard &operator=(const BitBoard &rhs);
    unsigned char *operator[](int idx);
    const unsigned char *operator[](int idx) const;
    bool operator==(const BitBoard &rhs) const;
    unsigned long long rowID() const;
    unsigned long long colID() const;
    void clear();
    int first_bit(int m, int k) const;
    int last_bit(int m, int k) const;
    bool conflict(int m, int i, int j) const;
    bool solved() const;
};

struct Node {
    BitBoard board;
    int dis;
    size_t last;
    Node(BitBoard &_board, int _dis, size_t _last) : board(_board), dis(_dis), last(_last) {}
    bool operator==(const Node &rhs) const {
        return board == rhs.board;
    }
};

using idPair = std::pair<unsigned long long, unsigned long long>;

struct myHash {
    std::size_t operator()(const idPair &obj) const {
        return std::hash<unsigned long long>()(obj.first) + std::hash<unsigned long long>()(obj.second);
    }
};

}  // namespace RUSH

#endif