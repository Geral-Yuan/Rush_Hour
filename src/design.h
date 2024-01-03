#ifndef DESIGN_H
#define DESIGN_H

// #define LEVEL_DESIGN

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <Fl/Fl_Box.H>
#include <FL/fl_draw.H>
#include <utility>
#include <algorithm>
#include <iostream>

namespace RUSH {

extern Fl_Color vehicle_colors[16];
extern size_t mapped_idx[16];
extern size_t IDs[6][6];

bool isLegal(int i, int j);

bool isHorizontal(int i, int j);

class Vehicle : public Fl_Widget {
    bool horizontal;
    bool isCar;
    bool dragging;
    bool cover;
    bool settle;
    std::pair<int, int> pos;
    int dx, dy;
    int drawX, drawY;
    size_t color_idx;

   public:
    int board_X, board_Y, cellSize;

    Vehicle(int X, int Y, int W, int H, bool car, size_t idx) : Fl_Widget(X, Y, W, H), horizontal(true), isCar(car), dragging(false), cover(false), settle(false), drawX(X), drawY(Y), color_idx(idx) {}
    void draw() override;

    void Settle(bool h);
    void Stay();
    void setPos(int row, int col);
    void reset();

    bool isHorizontal() const { return horizontal; }
    bool settled() const { return settle; }
    bool stay() const { return !(dragging || settle); }
    int posRow() const { return pos.first; }
    int posCol() const { return pos.second; }

    void resize(int X, int Y, int W, int H);
    bool inSideVehicle(int x, int y) const;
    void changeDirection();
    void checkFullRow(int &coverCnt);
    void checkFullCol(int &coverCnt);
    void check();
    int handle(int event) override;
};

void drawBoard(int startX, int startY, int cellSize);

class DesignBoard : public Fl_Widget {
   public:
    DesignBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}
    void drawGrids() const;
    void draw() override;
};

class VehicleBoard : public Fl_Widget {
    Vehicle *vehicles[16];

   public:
    int board_X, board_Y, cellSize;
    VehicleBoard(int X, int Y, int W, int H);
    ~VehicleBoard();
    void draw() override;
    void resize(int X, int Y, int W, int H) override;
    void show() override;
    void hide() override;
    void set2level(int level);
    void reset();

#ifdef LEVEL_DESIGN
    void printState() const;
#endif
};

class RuleLabel : public Fl_Box {
   public:
    RuleLabel(int X, int Y, int W, int H, const char *L = 0) : Fl_Box(X, Y, W, H, L) {}
    void draw() override {
        int startX = x(), startY = y();
        int width = w(), height = h();
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, height / 10);
        fl_draw(label(), startX, startY, width, height, FL_ALIGN_CENTER);
    };
};

class LevelBoard : public Fl_Widget {
    int level;

   public:
    LevelBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H), level(0) {}
    int getLevel() const { return level; }
    void lastLevel();
    void nextLevel();
    void draw() override;
};

}  // namespace RUSH

#endif