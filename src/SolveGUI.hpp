#ifndef SOLVEGUI_HPP
#define SOLVEGUI_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <vector>
#include "Solver.hpp"
#include "DesignGUI.hpp"

namespace RUSH {

void timer_callback(void *widget);
void start_callback(Fl_Widget *widget, void *);
void solve_callback(Fl_Widget *widget, void *);

class RushBoard : public Fl_Widget {
    bool solvable;
    const std::vector<BitBoard> *solution;
    size_t idx;

   public:
    RushBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H), solvable(false), solution(nullptr), idx(0) {
    }

    void setSolution(const std::pair<bool, std::vector<BitBoard>> &sol) {
        solution = &sol.second;
        solvable = sol.first;
        if (solvable) idx=solution->size()-1;
    }

    size_t get_idx() const {
        return idx;
    }

    bool isSolvable() const {
        return solvable;
    }

    void drawVehicle(int m, int k, int l, int len, int &color_idx) const {
        int X = x(), Y = y(), size = w() / 6;
        int width = 0.8 * size;
        int x = l * size + (size - width) / 2;
        int y = k * size + (size - width) / 2;
        int w = size * len - (size - width);
        int h = width;
        fl_color(vehicle_colors[mapped_idx[color_idx++] % 16]);
        if (m == 0)
            fl_rectf(X + x, Y + y, w, h);
        else
            fl_rectf(X + y, Y + x, h, w);
    }

    void draw() override {
        int startX = x(), startY = y();
        int side = w(), cellSize = side / 6;
        drawBoard(startX, startY, cellSize);
        int colorIDX = 0;
        for (int m = 0; m < 2; ++m)
            for (int k = 0; k < 6; ++k) {
                int bitCnt = m == 0 ? (*solution)[idx].rowCnt[k] : (*solution)[idx].colCnt[k];
                switch (bitCnt) {
                    case 2:
                    case 3: {
                        drawVehicle(m, k, (*solution)[idx].first_bit(m, k), bitCnt, colorIDX);
                        break;
                    }
                    case 4: {
                        drawVehicle(m, k, (*solution)[idx].first_bit(m, k), 2, colorIDX);
                        drawVehicle(m, k, (*solution)[idx].last_bit(m, k) - 1, 2, colorIDX);
                        break;
                    }
                    case 5: {
                        int leftLength = ((*solution)[idx][m][k] & (1 << 7)) ? 3 : 2;
                        drawVehicle(m, k, (*solution)[idx].first_bit(m, k), leftLength, colorIDX);
                        drawVehicle(m, k, (*solution)[idx].last_bit(m, k) + leftLength - 4, 5 - leftLength, colorIDX);
                        break;
                    }
                    default:
                        break;
                }
            }
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

class StartButton : public Fl_Button {
   public:
    StartButton(int X, int Y, int W, int H, const char *L = 0) : Fl_Button(X, Y, W, H, L) {}
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
    Solver *solver;
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;
    StartButton *startButton;
    DesignBoard *designBoard;
    VehicleBoard *vehicleBoard;
    RushBoard *rushBoard;
    SolveButton *solveButton;
    StepLabel *stepLabel;

    RushWindow(int W, int H, const char *L = 0) : Fl_Window(W, H, L) {
        solver = new Solver;
        backgroundImage = new Fl_PNG_Image("./assets/Background.png");
        background = new Fl_Box(0, 0, W, H);
        background->image(backgroundImage->copy(W, H));
        startButton = new StartButton(0, 0, W, H, "Start");
        designBoard = new DesignBoard(0, 0, W, H);
        vehicleBoard = new VehicleBoard(0, 0, W, H);
        rushBoard = new RushBoard(0, 0, W, H);
        rushBoard->hide();
        solveButton = new SolveButton(0, 0, W, H, "Solve");
        solveButton->hide();
        stepLabel = new StepLabel(0, 0, W, H, "0 steps");
        stepLabel->hide();
        startButton->callback(start_callback, this);
        solveButton->callback(solve_callback, this->rushBoard);
    }

    ~RushWindow() {
        delete solver;
        delete backgroundImage;
        delete background->image();
        delete background;
        delete startButton;
        delete designBoard;
        delete vehicleBoard;
        delete rushBoard;
        delete solveButton;
        delete stepLabel;
    }

    void initSolver() {
        BitBoard board;
        size_t last_id = 0;
        int num = 0;
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                if (IDs[i][j])
                    if (isHorizontal(i, j)) {
                        board[0][i] |= 1 << j;
                        ++board.rowCnt[i];
                        if (IDs[i][j] != last_id)
                            last_id = mapped_idx[num++] = IDs[i][j];
                    }

        for (int j = 0; j < 6; ++j)
            for (int i = 0; i < 6; ++i)
                if (IDs[i][j])
                    if (!isHorizontal(i, j)) {
                        board[1][j] |= 1 << i;
                        ++board.colCnt[j];
                        if (IDs[i][j] != last_id)
                            last_id = mapped_idx[num++] = IDs[i][j];
                    }

        for (int i = 0; i < 6; ++i)
            if (board.rowCnt[i] == 6 || board.colCnt[i] == 6) {
                std::cerr << "Invalid board with a stuck row or column" << std::endl;
                exit(1);
            }

        if (board.rowCnt[2] != 2) {
            std::cerr << "Invalid board as not exactly a car in exit row" << std::endl;
            exit(1);
        }

        for (int i = 0; i < 6; ++i)
            if (board.rowCnt[i] == 5) {
                for (int j = 0; j < 6; ++j) {
                    if (IDs[i][j] == 0 || IDs[i][j] != IDs[i][j + 1]) continue;
                    if (IDs[i][j + 2] == IDs[i][j])
                        board[0][i] |= 1 << 7;
                    break;
                }
            }
        for (int j = 0; j < 6; ++j)
            if (board.colCnt[j] == 5) {
                for (int i = 0; i < 6; ++i) {
                    if (IDs[i][j] == 0 || IDs[i][j] != IDs[i + 1][j]) continue;
                    if (IDs[i + 2][j] == IDs[i][j])
                        board[1][j] |= 1 << 7;
                    break;
                }
            }
        solver->setBoard(board);
    }

    void update() {
        rushBoard->update();
        stepLabel->update();
        this->redraw();
    }

    void resize(int X, int Y, int W, int H) override {
        Fl_Window::resize(X, Y, W, H);
        int win_W = W, win_H = H;
        int side = std::min(win_W / 3, win_H / 2);
        int canvas_X = (win_W - 3 * side) / 2, canvas_Y = (win_H - 2 * side) / 2;
        int canvas_W = side * 3, canvas_H = side * 2;
        delete background->image();
        background->resize(canvas_X, canvas_Y, canvas_W, canvas_H);
        background->image(backgroundImage->copy(canvas_W, canvas_H));
        startButton->resize(canvas_X + side * 5 / 4, canvas_Y + side * 5 / 3, side / 2, side / 6);
        designBoard->resize(canvas_X + side / 3, canvas_Y + side / 2, side, side);
        vehicleBoard->resize(canvas_X + side * 5 / 3, canvas_Y + side / 2, side, side);
        rushBoard->resize(canvas_X + side / 3, canvas_Y + side / 3, side * 4 / 3, side * 4 / 3);
        solveButton->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side / 2, side * 3 / 5, side / 6);
        stepLabel->resize(canvas_X + canvas_W - side * 3 / 4, canvas_Y + side * 4 / 3, side / 2, side / 6);
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

void start_callback(Fl_Widget *widget, void *win) {
    StartButton *button = (StartButton *)widget;
    RushWindow *window = (RushWindow *)win;
    window->initSolver();
    window->rushBoard->setSolution(window->solver->solve());
    window->designBoard->hide();
    window->vehicleBoard->hide();
    button->hide();
    window->rushBoard->show();
    window->solveButton->show();
    window->stepLabel->show();
}

void solve_callback(Fl_Widget *widget, void *board) {
    SolveButton *button = (SolveButton *)widget;
    RushBoard *rushBoard = (RushBoard *)board;
    if (button->isCalled()) return;
    button->Call();
    if (rushBoard->isSolvable()) {
        Fl::add_timeout(1.0 / 5.0, timer_callback, button->window());
        button->label("Solving...");
    } else
        button->label("Unsolvable");
}

}  // namespace RUSH

#endif