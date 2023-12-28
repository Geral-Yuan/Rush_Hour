#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <vector>
#include "BitBoard.hpp"

namespace RUSH {

Fl_Color colors[] = {
    FL_RED,
    FL_GREEN,
    FL_YELLOW,
    FL_BLUE,
    FL_MAGENTA,
    FL_CYAN,
    FL_BLACK,
    FL_DARK_RED,
    FL_DARK_GREEN,
    FL_DARK_YELLOW,
    FL_DARK_BLUE,
    FL_DARK_MAGENTA,
    FL_DARK_CYAN,
    FL_LIGHT1,
    FL_DARK2,
    FL_GRAY0};

class RushBoard : public Fl_Widget {
    std::vector<BitBoard> boards;
    size_t idx;

   public:
    RushBoard(int X, int Y, int W, int H, const std::vector<BitBoard> &B=std::vector<BitBoard>()) : Fl_Widget(X, Y, W, H), boards(B) {
        idx=boards.size()-1;
    }
    void drawBoard(int X, int Y, int size) const {
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j) {
                int x = X + j * size;
                int y = Y + i * size;
                fl_color(FL_WHITE);
                fl_rectf(x, y, size, size);
                fl_color(FL_BLACK);
                fl_rect(x, y, size, size);
            }
    }
    void drawVehicle(int X, int Y, int size, int m, int k, int l, int len, int idx) const {
        int width = 0.8 * size;
        int x = l * size + (size - width) / 2;
        int y = k * size + (size - width) / 2;
        int w = size * len - (size - width);
        int h = width;
        fl_color(colors[idx]);
        if (m == 0)
            fl_rectf(X + x, Y + y, w, h);
        else
            fl_rectf(X + y, Y + x, h, w);
    }

    void draw() override {
        int width = this->w();
        int height = this->h();
        int side = std::min(width, height);
        int cellSize = side / 6;
        int startX = (width - side) / 2;
        int startY = (height - side) / 2;
        drawBoard(startX, startY, cellSize);
        int colorIDX = 0;
        for (int m = 0; m < 2; ++m)
            for (int k = 0; k < 6; ++k) {
                if (m == 0 && k == 2) continue;
                int bitCnt = m == 0 ? boards[idx].rowCnt[k] : boards[idx].colCnt[k];
                switch (bitCnt) {
                    case 2:
                    case 3: {
                        drawVehicle(startX, startY, cellSize, m, k, boards[idx].first_bit(m, k), bitCnt, ++colorIDX);
                        break;
                    }
                    case 4: {
                        drawVehicle(startX, startY, cellSize, m, k, boards[idx].first_bit(m, k), 2, ++colorIDX);
                        drawVehicle(startX, startY, cellSize, m, k, boards[idx].last_bit(m, k) - 1, 2, ++colorIDX);
                        break;
                    }
                    case 5: {
                        int leftLength = (boards[idx][m][k] & (1 << 7)) ? 3 : 2;
                        drawVehicle(startX, startY, cellSize, m, k, boards[idx].first_bit(m, k), leftLength, ++colorIDX);
                        drawVehicle(startX, startY, cellSize, m, k, boards[idx].last_bit(m, k) + leftLength - 4, 5 - leftLength, ++colorIDX);
                        break;
                    }
                    default:
                        break;
                }
            }
        drawVehicle(startX, startY, cellSize, 0, 2, boards[idx].first_bit(0, 2), 2, 0);
    }
    
    void update(){
        if (idx) --idx;
        this->redraw();
    }
};

class RushWindow : public Fl_Window {
   public:
    RushBoard *rushboard;

    RushWindow(int W, int H, const char *L = 0, const std::vector<BitBoard> &B = std::vector<BitBoard>()) : Fl_Window(W, H, L) {
        rushboard = new RushBoard(0, 0, W, H, B);
        this->resizable(this);
    }
};

void timer_callback(void *widget) {
    ((RushBoard*)widget)->update(); // update
    Fl::repeat_timeout(1.0/2.0, timer_callback, widget); // recall each 0.5s
}

}  // namespace RUSH

#endif