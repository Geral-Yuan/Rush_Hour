#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
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

void timer_callback(void *widget);
void start_callback(Fl_Widget *widget, void *);

class RushBoard : public Fl_Widget {
    std::vector<BitBoard> boards;
    size_t idx;

   public:
    RushBoard(int X, int Y, int W, int H, const std::vector<BitBoard> &B = std::vector<BitBoard>()) : Fl_Widget(X, Y, W, H), boards(B) {
        idx = boards.size() - 1;
    }

    size_t get_idx() const {
        return idx;
    }

    void drawBoard(int startX, int startY, int side, int cellSize) const {
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID, 3);
        fl_line(startX, startY, startX, startY + side);
        fl_line(startX, startY, startX + side, startY);
        fl_line(startX, startY + side, startX + side, startY + side);
        fl_line(startX + side, startY, startX + side, startY + cellSize * 2);
        fl_line(startX + side, startY + cellSize * 3, startX + side, startY + cellSize * 6);
        fl_line_style(0);
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
        int side = std::min(width / 3, height / 2);
        side *= 2;
        int cellSize = side / 6;
        int startX = (width - side * 3 / 2) / 2;
        int startY = (height - side) / 2;
        drawBoard(startX, startY, side, cellSize);
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

    void update() {
        if (idx) --idx;
    }
};

class BackgroundBoard : public Fl_Widget {
   public:
    BackgroundBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}
    void draw() override {
        int winWidth = this->w();
        int winHeight = this->h();
        int side = std::min(winWidth / 3, winHeight / 2);
        int width = side * 3;
        int height = side * 2;
        int startX = (winWidth - width) / 2;
        int startY = (winHeight - height) / 2;
        fl_color(FL_WHITE);
        fl_rectf(startX, startY, width, height);
    }
};

class StartButton : public Fl_Button {
    bool called;

   public:
    StartButton(int X, int Y, int W, int H, const char *L) : Fl_Button(X, Y, W, H, L), called(false) {}
    void Call() {
        called = true;
    }
    bool isCalled() const {
        return called;
    }
    void draw() {
        int winWidth = this->w();
        int winHeight = this->h();
        int side = std::min(winWidth / 3, winHeight / 2);
        int startX = (winWidth - 3 * side) / 2 + side * 2 + side / 4;
        int startY = (winHeight - 2 * side) / 2 + side / 4;
        int width = side / 2;
        int height = side / 4;
        fl_color(FL_GRAY);
        fl_rectf(startX, startY, width, height);
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, side / 12);
        fl_draw(label(), startX, startY, width, height, FL_ALIGN_CENTER);
    }
};

class RushWindow : public Fl_Window {
   public:
    BackgroundBoard *backgroundBoard;
    RushBoard *rushBoard;
    StartButton *startButton;

    RushWindow(int W, int H, const char *L = 0, const std::vector<BitBoard> &B = std::vector<BitBoard>()) : Fl_Window(W, H, L) {
        backgroundBoard = new BackgroundBoard(0, 0, W, H);
        rushBoard = new RushBoard(0, 0, W, H, B);
        startButton = new StartButton(0, 0, W, H, "SOLVE");
        startButton->callback(start_callback);
        this->resizable(this);
    }
    ~RushWindow() {
        delete backgroundBoard;
        delete rushBoard;
    }
    void update() {
        rushBoard->update();
        this->redraw();
    }
    void draw() {
        backgroundBoard->draw();
        rushBoard->draw();
        startButton->draw();
    }
};

void timer_callback(void *widget) {
    RushWindow *window = (RushWindow *)widget;
    window->update();
    if (window->rushBoard->get_idx() > 0)
        Fl::repeat_timeout(1.0 / 5.0, timer_callback, widget);
    else
        window->startButton->label("Solved!");
}

void start_callback(Fl_Widget *widget, void *) {
    StartButton *button = (StartButton *)widget;
    if (button->isCalled()) return;
    button->Call();
    Fl::add_timeout(1.0 / 5.0, timer_callback, button->window());
    button->label("Solving...");
}

}  // namespace RUSH

#endif