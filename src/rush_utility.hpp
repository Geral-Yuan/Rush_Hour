#ifndef RUSH_UTILITY_H
#define RUSH_UTILITY_H

#include "BitBoard.hpp"
#include "Grid.hpp"

namespace RUSH{

void Grid2Board(Grid &grid, BitBoard &board){
    board.clear();
    for (int i=0;i<6;++i)
        for (int j=0;j<6;++j)
            if (grid[i][j]){
                if (isHorizontal(grid,i,j)){
                    board[0]|=1ull<<((i<<3)+j);
                    ++board.rowCnt[i];
                }else{
                    board[1]|=1ull<<((j<<3)+i);
                    ++board.colCnt[j];
                }
            }

    for (int i=0;i<6;++i)
        if (board.rowCnt[i]==6||board.colCnt[i]==6){
            std::cerr<<"Invalid board with a stuck row or column"<<std::endl;
            exit(1);
        }
    if (board.rowCnt[2]!=2){
        std::cerr<<"Invalid board as not exactly a car in exit row"<<std::endl;
        exit(1);
    }
    for (int i=0;i<6;++i)
        if (board.rowCnt[i]==5){
            // board[0]|=1ull<<((i<<3)+6);
            for (int j=0;j<6;++j){
                if (grid[i][j]==0||grid[i][j]!=grid[i][j+1]) continue;
                if (grid[i][j+2]==grid[i][j])
                    board[0]|=1ull<<((i<<3)+7);
                break;
            }
        }
    for (int j=0;j<6;++j)
        if (board.colCnt[j]==5){
            // board[1]|=1ull<<((j<<3)+6);
            for (int i=0;i<6;++i){
                if (grid[i][j]==0||grid[i][j]!=grid[i+1][j]) continue;
                if (grid[i+2][j]==grid[i][j])
                    board[1]|=1ull<<((j<<3)+7);
                break;
            }
        }
}

void Board2Grid(BitBoard &board, Grid &grid){
    grid.clear();
    int num=0;
    for (int i=0;i<6;++i){
        switch (board.rowCnt[i]){
            case 2:
            case 3:{
                ++num;
                for (int j=0;j<6;++j)
                    if (board[0]&(1ull<<((i<<3)+j)))
                        grid[i][j]=num;
                break;
            }
            case 4:{
                ++num;
                int localCnt=0;
                for (int j=0;j<6;++j)
                    if (board[0]&(1ull<<((i<<3)+j))){
                        grid[i][j]=num+(localCnt>>1);
                        ++localCnt;
                    }
                ++num;
                break;
            }
            case 5:{
                ++num;
                int localCnt=0;
                int leftLength=(board[0]&(1ull<<((i<<3)+7)))?3:2;
                for (int j=0;j<6;++j)
                    if (board[0]&(1ull<<((i<<3)+j))){
                        grid[i][j]=num+(localCnt<leftLength?0:1);
                        ++localCnt;
                    }
                ++num;
                break;
            }
            default:
                break;
        }
    }
    for (int j=0;j<6;++j){
        switch (board.colCnt[j]){
            case 2:
            case 3:{
                ++num;
                for (int i=0;i<6;++i)
                    if (board[1]&(1ull<<((j<<3)+i)))
                        grid[i][j]=num;
                break;
            }
            case 4:{
                ++num;
                int localCnt=0;
                for (int i=0;i<6;++i)
                    if (board[1]&(1ull<<((j<<3)+i))){
                        grid[i][j]=num+(localCnt>>1);
                        ++localCnt;
                    }
                ++num;
                break;
            }
            case 5:{
                ++num;
                int localCnt=0;
                int upLength=(board[1]&(1ull<<((j<<3)+7)))?3:2;
                for (int i=0;i<6;++i)
                    if (board[1]&(1ull<<((j<<3)+i))){
                        grid[i][j]=num+(localCnt<upLength?0:1);
                        ++localCnt;
                    }
                ++num;
                break;
            }
            default:
                break;
        }
    }
}

}

#endif