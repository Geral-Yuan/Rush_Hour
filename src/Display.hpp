#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
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
        int startX = x(), startY = y();
        int side = w(), cellSize = side / 6;
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

class SolveButton : public Fl_Button {
    bool called;

   public:
    SolveButton(int X, int Y, int W, int H, const char *L) : Fl_Button(X, Y, W, H, L), called(false) {}
    void Call() {
        called = true;
    }
    bool isCalled() const {
        return called;
    }
    void draw() override {
        int startX = x(), startY = y();
        int width = w(), height = h();
        fl_color(FL_WHITE);
        fl_rectf(startX, startY, width, height);
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, width / 6);
        fl_draw(label(), startX, startY, width, height, FL_ALIGN_CENTER);
    }
};

class StepLabel : public Fl_Box {
    size_t step;

   public:
    StepLabel(int X, int Y, int W, int H, const char *L = 0, size_t _step = 0) : Fl_Box(X, Y, W, H, L), step(_step) {}

    void draw() override {
        int startX = x(), startY = y();
        int width = w(), height = h();
        fl_color(FL_WHITE);
        fl_rectf(startX, startY, width, height);
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, width / 6);
        label((std::to_string(step) + " steps").c_str());
        fl_draw(label(), startX, startY, width, height, FL_ALIGN_CENTER);
    };

    void update() {
        ++step;
    }
};

class RushWindow : public Fl_Window {
   public:
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;
    RushBoard *rushBoard;
    SolveButton *solveButton;
    StepLabel *stepLabel;

    RushWindow(int W, int H, const char *L = 0, const std::vector<BitBoard> &B = std::vector<BitBoard>()) : Fl_Window(W, H, L) {
        backgroundImage = new Fl_PNG_Image("./assets/Background.png");
        background = new Fl_Box(0, 0, W, H);
        background->image(backgroundImage->copy(W, H));
        rushBoard = new RushBoard(0, 0, W, H, B);
        solveButton = new SolveButton(0, 0, W, H, "Solve");
        stepLabel = new StepLabel(0, 0, W, H, "0 steps");
        stepLabel->labelfont(FL_BOLD);
        stepLabel->labelcolor(FL_BLUE);
        stepLabel->color(FL_WHITE);
        solveButton->callback(start_callback);
        this->end();
        this->show();
    }

    ~RushWindow() {
        delete backgroundImage;
        delete background->image();
        delete background;
        delete rushBoard;
        delete solveButton;
        delete stepLabel;
    }

    void update() {
        rushBoard->update();
        stepLabel->update();
        this->redraw();
    }

    void resize(int X, int Y, int W, int H) override {
        Fl_Window::resize(X, Y, W, H);
        int win_W = w(), win_H = h();
        int side = std::min(win_W / 3, win_H / 2);
        int canvas_X = (win_W - 3 * side) / 2, canvas_Y = (win_H - 2 * side) / 2;
        int canvas_W = side * 3, canvas_H = side * 2;
        int boardSide = canvas_H;
        delete background->image();
        background->resize(canvas_X, canvas_Y, canvas_W, canvas_H);
        background->image(backgroundImage->copy(canvas_W, canvas_H));
        rushBoard->resize(canvas_X, canvas_Y, boardSide, boardSide);
        solveButton->resize(canvas_X + canvas_W - side * 3 / 4, canvas_Y + side / 4, side / 2, side / 4);
        stepLabel->resize(canvas_X + canvas_W - side * 3 / 4, canvas_Y + side * 3 / 2, side / 2, side / 4);
    }
};

void timer_callback(void *widget) {
    RushWindow *window = (RushWindow *)widget;
    window->update();
    if (window->rushBoard->get_idx() > 0)
        Fl::repeat_timeout(1.0 / 5.0, timer_callback, widget);
    else
        window->solveButton->label("Solved!");
}

void start_callback(Fl_Widget *widget, void *) {
    SolveButton *button = (SolveButton *)widget;
    if (button->isCalled()) return;
    button->Call();
    Fl::add_timeout(1.0 / 5.0, timer_callback, button->window());
    button->label("Solving...");
}

}  // namespace RUSH

#endif