#ifndef SOLVEGUI_HPP
#define SOLVEGUI_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <vector>
#include "Solver.hpp"
#include "DesignGUI.hpp"

namespace RUSH {

void timer_callback(void *);
void start_callback(Fl_Widget *, void *);
void load_callback(Fl_Widget *, void *);
void lastLevel_callback(Fl_Widget *, void *);
void nextLevel_callback(Fl_Widget *, void *);
void solve_callback(Fl_Widget *, void *);
void design_callback(Fl_Widget *, void *);

#ifdef LEVEL_DESIGN
void printLevel_callback(Fl_Widget *, void *);
#endif

class RushBoard : public Fl_Widget {
    bool solvable;
    const std::vector<BitBoard> *solution;
    size_t idx;

   public:
    RushBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H), solvable(false), solution(nullptr), idx(0) {}

    ~RushBoard(){
        if (solution)
            delete solution;
    }

    void reset() {
        solvable = false;
        if (solution) {
            delete solution;
            solution = nullptr;
        }
        idx = 0;
    }

    void setSolution(const std::pair<bool, std::vector<BitBoard> *> sol) {
        solvable = sol.first;
        solution = sol.second;
        if (solvable) idx = solution->size() - 1;
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
    void update() { ++step; }
    void reset() { step = 0; }
};

class RushWindow : public Fl_Window {
   public:
    Solver *solver;
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;
    Fl_Button *startButton;
    DesignBoard *designBoard;
    VehicleBoard *vehicleBoard;
    RuleLabel *ruleLabel;
    LevelBoard *levelBoard;
    Fl_Button *loadButton;
    Fl_Button *lastLevel;
    Fl_Button *nextLevel;
    RushBoard *rushBoard;
    Fl_Button *solveButton;
    StepLabel *stepLabel;
    Fl_Button *back2Design;
    bool solving;
#ifdef LEVEL_DESIGN
    Fl_Button *printDesign;
#endif

    RushWindow(int W, int H, const char *L = 0) : Fl_Window(W, H, L), solving(false) {
        solver = new Solver;
        backgroundImage = new Fl_PNG_Image("./assets/Background.png");
        background = new Fl_Box(0, 0, W, H);
        background->image(backgroundImage->copy(W, H));
        startButton = new Fl_Button(0, 0, W, H, "Start");
        startButton->labelfont(FL_HELVETICA_BOLD);
        designBoard = new DesignBoard(0, 0, W, H);
        vehicleBoard = new VehicleBoard(0, 0, W, H);
        ruleLabel = new RuleLabel(0, 0, W, H, "Design the puzzle by yourself. You can drag cars and trucks onto the board by mouse\nand rotate them by pushing any key on the keyboard. The goal is to move the red car to the exit,\nso only the red can be put on the exit lane except at the exit. Click \"Start\" to confirm your design.");
        levelBoard = new LevelBoard(0, 0, W, H);
        loadButton = new Fl_Button(0, 0, W, H, "Load");
        loadButton->labelfont(FL_HELVETICA_BOLD);
        lastLevel = new Fl_Button(0, 0, W, H, "<");
        lastLevel->labelfont(FL_HELVETICA_BOLD);
        nextLevel = new Fl_Button(0, 0, W, H, ">");
        nextLevel->labelfont(FL_HELVETICA_BOLD);
        rushBoard = new RushBoard(0, 0, W, H);
        rushBoard->hide();
        solveButton = new Fl_Button(0, 0, W, H, "Solve");
        solveButton->labelfont(FL_HELVETICA_BOLD);
        solveButton->hide();
        stepLabel = new StepLabel(0, 0, W, H, "0 steps");
        stepLabel->hide();
        back2Design = new Fl_Button(0, 0, W, H, "Desgin");
        back2Design->labelfont(FL_HELVETICA_BOLD);
        back2Design->hide();
        startButton->callback(start_callback, this);
        lastLevel->callback(lastLevel_callback, this);
        nextLevel->callback(nextLevel_callback, this);
        loadButton->callback(load_callback, this);
        solveButton->callback(solve_callback, this);
        back2Design->callback(design_callback, this);
#ifdef LEVEL_DESIGN
        printDesign = new Fl_Button(0, 0, W, H, "Print");
        printDesign->labelfont(FL_HELVETICA_BOLD);
        printDesign->callback(printLevel_callback, this);
#endif
    }

    ~RushWindow() {
        delete solver;
        delete backgroundImage;
        delete background->image();
        delete background;
        delete startButton;
        delete designBoard;
        delete vehicleBoard;
        delete ruleLabel;
        delete levelBoard;
        delete loadButton;
        delete lastLevel;
        delete nextLevel;
        delete rushBoard;
        delete solveButton;
        delete stepLabel;
        delete back2Design;
#ifdef LEVEL_DESIGN
        delete printDesign;
#endif
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
        startButton->resize(canvas_X + side * 2, canvas_Y + side * 5 / 3, side / 2, side / 6);
        startButton->labelsize(side / 12);
        designBoard->resize(canvas_X + side / 3, canvas_Y + side / 2, side, side);
        vehicleBoard->resize(canvas_X + side * 5 / 3, canvas_Y + side / 2, side, side);
        ruleLabel->resize(canvas_X, canvas_Y, canvas_W, side / 2);
        levelBoard->resize(canvas_X + side * 7 / 18, canvas_Y + side * 14 / 9, side * 4 / 9, side * 4 / 9);
        loadButton->resize(canvas_X + side, canvas_Y + side * 11 / 6, side * 2 / 9, side / 9);
        loadButton->labelsize(side / 20);
        lastLevel->resize(canvas_X + side * 8 / 9, canvas_Y + side * 11 / 6, side / 9, side / 9);
        lastLevel->labelsize(side / 20);
        nextLevel->resize(canvas_X + side * 11 / 9, canvas_Y + side * 11 / 6, side / 9, side / 9);
        nextLevel->labelsize(side / 20);
        rushBoard->resize(canvas_X + side / 3, canvas_Y + side / 3, side * 4 / 3, side * 4 / 3);
        solveButton->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side / 2, side * 3 / 5, side / 6);
        solveButton->labelsize(side / 12);
        stepLabel->resize(canvas_X + canvas_W - side * 3 / 4, canvas_Y + side * 4 / 3, side / 2, side / 6);
        back2Design->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side * 5 / 3, side * 3 / 5, side / 6);
        back2Design->labelsize(side / 12);
#ifdef LEVEL_DESIGN
        printDesign->resize(canvas_X + side, canvas_Y + side * 10 / 6, side * 2 / 9, side / 9);
        printDesign->labelsize(side / 20);
#endif
    }

    void reset() {
        solving = false;
        solveButton->label("Solve");
        vehicleBoard->reset();
        rushBoard->reset();
        stepLabel->reset();
    }
};

void timer_callback(void *widget) {
    RushWindow *window = (RushWindow *)widget;
    if (!window->solving) return;
    window->update();
    if (window->rushBoard->get_idx() > 0)
        Fl::repeat_timeout(1.0 / 5.0, timer_callback, widget);
    else
        window->solveButton->label("Solved!");
}

void start_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->initSolver();
    window->rushBoard->setSolution(window->solver->solve());
    window->designBoard->hide();
    window->vehicleBoard->hide();
    window->ruleLabel->hide();
    window->levelBoard->hide();
    window->loadButton->hide();
    window->lastLevel->hide();
    window->nextLevel->hide();
    window->startButton->hide();
    window->rushBoard->show();
    window->solveButton->show();
    window->stepLabel->show();
    window->back2Design->show();
#ifdef LEVEL_DESIGN
    window->printDesign->hide();
#endif
}

void load_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    int level = window->levelBoard->getLevel();
    window->vehicleBoard->set2level(level);
    window->redraw();
}

void lastLevel_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->levelBoard->lastLevel();
    window->redraw();
}

void nextLevel_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->levelBoard->nextLevel();
    window->redraw();
}

void solve_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    if (window->solving) return;
    window->solving = true;
    if (window->rushBoard->isSolvable()) {
        Fl::add_timeout(1.0 / 5.0, timer_callback, window->solveButton->window());
        window->solveButton->label("Solving...");
    } else
        window->solveButton->label("Unsolvable");
}

void design_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->reset();
    window->designBoard->show();
    window->vehicleBoard->show();
    window->ruleLabel->show();
    window->levelBoard->show();
    window->loadButton->show();
    window->lastLevel->show();
    window->nextLevel->show();
    window->startButton->show();
    window->rushBoard->hide();
    window->solveButton->hide();
    window->stepLabel->hide();
    window->back2Design->hide();
}

#ifdef LEVEL_DESIGN
void printLevel_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->vehicleBoard->printState();
}
#endif

}  // namespace RUSH

#endif