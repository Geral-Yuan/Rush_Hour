#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>
#include <cstring>

namespace RUSH{

class BitBoard{
    unsigned long long rowMat;
    unsigned long long colMat;
public:
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
    bool solved(){
        for (int j=5;j>=0;--j){
            if (rowMat&(1ull<<((j<<3)+2)))
                return false;
            if (colMat&(1ull<<((2<<3)+j)))
                return true;
        }
        return false;
    }
    void print(){
        Grid grid;
        Board2Grid(*this,grid);
        grid.print();
    }
};

struct Node{
    BitBoard board;
    int dis;
    size_t last;
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