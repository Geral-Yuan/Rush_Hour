#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <cstring>

namespace RUSH{

struct Grid{
    int grid[6][6];
    int *operator[](int idx){
        if (idx<0||idx>=6) {
            std::cerr<<"Invalid index access"<<std::endl;
            exit(1);
        }
        return (int*)grid[idx];
    }
    void getInput(){
        for (int i=0;i<6;++i)
            for (int j=0;j<6;++j)
                std::cin>>grid[i][j];
    }
    void clear(){
        memset(grid,0,sizeof(grid));
    }
    void print(){
        for (int i=0;i<6;++i){
            for (int j=0;j<6;++j){
                std::cout<<grid[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }
};

bool legalGrid(int i,int j){ return i>=0&&i<6&&j>=0&&j<6; }

bool isHorizontal(Grid &grid,int i,int j){
    return (legalGrid(i,j-1)&&grid[i][j-1]==grid[i][j])||(legalGrid(i,j+1)&&grid[i][j+1]==grid[i][j]);
}

}

#endif