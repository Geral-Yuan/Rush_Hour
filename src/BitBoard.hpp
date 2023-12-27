#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstring>

namespace RUSH{

struct BitBoard{
    unsigned long long rowMat;
    unsigned long long colMat;
    int rowCnt[6];
    int colCnt[6];
    BitBoard():rowMat(0),colMat(0){
        memset(rowCnt,0,sizeof(rowCnt));
        memset(colCnt,0,sizeof(colCnt));
    }
    BitBoard(const BitBoard &rhs):rowMat(rhs.rowMat),colMat(rhs.colMat){
        for (int i=0;i<6;++i){
            rowCnt[i]=rhs.rowCnt[i];
            colCnt[i]=rhs.colCnt[i];
        }
    }
    unsigned long long &operator[](int idx){
        if (idx<0||idx>1) {
            std::cerr<<"Invalid index access"<<std::endl;
            exit(1);
        }
        if (idx==0) return rowMat;
        return colMat;
    }
    bool operator==(const BitBoard &rhs) const{
        return rowMat==rhs.rowMat&&colMat==rhs.colMat;
    }
    void clear(){
        rowMat=colMat=0;
        memset(rowCnt,0,sizeof(rowCnt));
        memset(colCnt,0,sizeof(colCnt));
    }
};

struct Node{
    BitBoard board;
    int dis;
    int last;
    bool operator==(const Node &rhs) const{
        return board==rhs.board;
    }
};

using MatPair = std::pair<unsigned long long,unsigned long long>;

struct myHash{
    std::size_t operator()(const MatPair &obj) const{
        return std::hash<unsigned long long>()(obj.first)+std::hash<unsigned long long>()(obj.second);
    }
};

}

#endif