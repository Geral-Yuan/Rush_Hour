#include <utility>
#include <vector>
#include <iostream>
#include <cstring>

class Solver {
    struct bitBoard{
        unsigned long long rowMat;
        unsigned long long colMat;
        int rowCnt[6];
        int colCnt[6];
        bitBoard():rowMat(0),colMat(0){
            memset(rowCnt,0,sizeof(rowCnt));
            memset(colCnt,0,sizeof(colCnt));
        }
        unsigned long long &operator[](int idx){
            if (idx<0||idx>1) {
                std::cerr<<"Invalid index access"<<std::endl;
                exit(1);
            }
            if (idx==0) return rowMat;
            return colMat;
        }
        bool operator==(const bitBoard &rhs) const{
            return rowMat==rhs.rowMat&&colMat==rhs.colMat;
        }
    };
private:
    bitBoard initBoard;
    std::vector<bitBoard> BFS_Q;

public:
    Solver(){
        int grid[6][6];
        for (int i=0;i<6;++i)
            for (int j=0;j<6;++j)
                std::cin>>grid[i][j];
        Grid2Board(grid, initBoard);
    }

    bool legalGrid(int i,int j){ return i>=0&&i<6&&j>=0&&j<6; }

    bool isHorizontal(const int grid[6][6],int i,int j){
        return legalGrid(i,j-1)&&grid[i][j-1]==grid[i][j]||legalGrid(i,j+1)&&grid[i][j+1]==grid[i][j];
    }

    void Grid2Board(const int grid[6][6], bitBoard &board){
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
        for (int i=0;i<6;++i)
            if (board.rowCnt[i]==5){
                // board[0]|=1ull<<((i<<3)+6);
                for (int j=0;j<6;++j){
                    if (grid[i][j]==0) continue;
                    if (grid[i][j+2]==grid[i][j])
                        board[0]|=1ull<<((i<<3)+7);
                    break;
                }
            }
        for (int j=0;j<6;++j)
            if (board.colCnt[j]==5){
                // board[1]|=1ull<<((j<<3)+6);
                for (int i=0;i<6;++i){
                    if (grid[i][j]==0) continue;
                    if (grid[i+2][j]==grid[i][j])
                        board[1]|=1ull<<((j<<3)+7);
                    break;
                }
            }
    }

    void Board2Grid(bitBoard &board, int grid[6][6]){
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

    void Print(int grid[6][6]){
        for (int i=0;i<6;++i){
            for (int j=0;j<6;++j){
                std::cout<<grid[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }

    void Print_initBoard(){
        int grid[6][6];
        memset(grid,0,sizeof(grid));
        Board2Grid(initBoard,grid);
        Print(grid);
    }

};