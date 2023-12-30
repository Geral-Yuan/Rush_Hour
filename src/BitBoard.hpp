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
    BitBoard() {
        memset(rowBits, 0, sizeof(rowBits));
        memset(colBits, 0, sizeof(colBits));
        memset(rowCnt, 0, sizeof(rowCnt));
        memset(colCnt, 0, sizeof(colCnt));
    }
    BitBoard(const BitBoard &rhs) {
        for (int i = 0; i < 6; ++i) {
            rowBits[i] = rhs.rowBits[i];
            colBits[i] = rhs.colBits[i];
            rowCnt[i] = rhs.rowCnt[i];
            colCnt[i] = rhs.colCnt[i];
        }
    }
    BitBoard &operator=(const BitBoard &rhs) {
        if (this == &rhs) return *this;
        for (int i = 0; i < 6; ++i) {
            rowBits[i] = rhs.rowBits[i];
            colBits[i] = rhs.colBits[i];
            rowCnt[i] = rhs.rowCnt[i];
            colCnt[i] = rhs.colCnt[i];
        }
        return *this;
    }
    unsigned char *operator[](int idx) {
        if (idx < 0 || idx > 1) {
            std::cerr << "Invalid index access" << std::endl;
            exit(1);
        }
        if (idx == 0) return (unsigned char *)rowBits;
        return (unsigned char *)colBits;
    }
    const unsigned char *operator[](int idx) const {
        if (idx < 0 || idx > 1) {
            std::cerr << "Invalid index access" << std::endl;
            exit(1);
        }
        if (idx == 0) return (unsigned char *)rowBits;
        return (unsigned char *)colBits;
    }
    bool operator==(const BitBoard &rhs) const {
        for (int i = 0; i < 6; ++i)
            if (rowBits[i] ^ rhs.rowBits[i])
                return false;
        for (int j = 0; j < 6; ++j)
            if (colBits[j] ^ rhs.colBits[j])
                return false;
        return true;
    }
    unsigned long long rowID() const {
        unsigned long long ret = 0;
        for (int i = 0; i < 6; ++i)
            ret = ret << 8 | rowBits[i];
        return ret;
    }
    unsigned long long colID() const {
        unsigned long long ret = 0;
        for (int j = 0; j < 6; ++j)
            ret = ret << 8 | colBits[j];
        return ret;
    }
    void clear() {
        memset(rowBits, 0, sizeof(rowBits));
        memset(colBits, 0, sizeof(colBits));
        memset(rowCnt, 0, sizeof(rowCnt));
        memset(colCnt, 0, sizeof(colCnt));
    }
    int first_bit(int m, int k) const {
        if (m == 0) {
            for (int j = 0; j < 6; ++j)
                if (rowBits[k] & (1 << j))
                    return j;
            return -1;
        }
        for (int i = 0; i < 6; ++i)
            if (colBits[k] & (1 << i))
                return i;
        return -1;
    }
    int last_bit(int m, int k) const {
        if (m == 0) {
            for (int j = 5; j >= 0; --j)
                if (rowBits[k] & (1 << j))
                    return j;
            return -1;
        }
        for (int i = 5; i >= 0; --i)
            if (colBits[k] & (1 << i))
                return i;
        return -1;
    }
    bool conflict(int m, int i, int j) const {
        if (m) std::swap(i, j);
        return (rowBits[i] & (1 << j)) && (colBits[j] & (1 << i));
    }
    bool solved() const {
        for (int j = 5; j >= 0; --j) {
            if (colBits[j] & (1 << 2))
                return false;
            if (rowBits[2] & (1 << j))
                return true;
        }
        return false;
    }
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