#include "solve.h"

namespace RUSH {

void timer_callback(void *widget) {
    RushWindow *window = (RushWindow *)widget;
    if (!window->solving) return;
    window->StepForward();
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
    window->clearButton->hide();
    window->loadButton->hide();
    window->lastLevel->hide();
    window->nextLevel->hide();
    window->startButton->hide();
    window->rushBoard->show();
    window->solveButton->show();
    window->stepLabel->show();
    window->lastStep->show();
    window->nextStep->show();
    window->back2Design->show();
#ifdef LEVEL_DESIGN
    window->printDesign->hide();
#endif
}

void clear_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->vehicleBoard->reset();
    window->redraw();
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

void lastStep_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    if (!window->rushBoard->isSolvable()) return;
    window->solveButton->label("Solve");
    window->solving = false;
    window->StepBackward();
}

void nextStep_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    if (!window->rushBoard->isSolvable()) return;
    window->solveButton->label("Solve");
    window->solving = false;
    window->StepForward();
    if (window->rushBoard->get_idx() == 0) {
        window->solveButton->label("Solved!");
        window->solving = true;
    }
}

void design_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->reset();
    window->designBoard->show();
    window->vehicleBoard->show();
    window->ruleLabel->show();
    window->levelBoard->show();
    window->clearButton->show();
    window->loadButton->show();
    window->lastLevel->show();
    window->nextLevel->show();
    window->startButton->show();
    window->rushBoard->hide();
    window->solveButton->hide();
    window->stepLabel->hide();
    window->lastStep->hide();
    window->nextStep->hide();
    window->back2Design->hide();
#ifdef LEVEL_DESIGN
    window->printDesign->show();
#endif
}

#ifdef LEVEL_DESIGN
void printLevel_callback(Fl_Widget *, void *win) {
    RushWindow *window = (RushWindow *)win;
    window->vehicleBoard->printState();
}

void RushBoard::reset() {
    solvable = false;
    if (solution) {
        delete solution;
        solution = nullptr;
    }
    idx = 0;
}

void RushBoard::setSolution(const std::pair<bool, std::vector<BitBoard> *> sol) {
    solvable = sol.first;
    solution = sol.second;
    if (solvable) idx = solution->size() - 1;
}

size_t RushBoard::get_idx() const {
    return idx;
}

bool RushBoard::isSolvable() const {
    return solvable;
}

void RushBoard::drawVehicle(int m, int k, int l, int len, int &color_idx) const {
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

void RushBoard::draw() {
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

bool RushBoard::nextStep() {
    if (idx) {
        --idx;
        return true;
    }
    return false;
}

bool RushBoard::lastStep() {
    if (idx < solution->size() - 1) {
        ++idx;
        return true;
    }
    return false;
}

RushWindow::RushWindow(int W, int H, const char *L) : Fl_Window(W, H, L), solving(false) {
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
    clearButton = new Fl_Button(0, 0, W, H, "Clear");
    clearButton->labelfont(FL_HELVETICA_BOLD);
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
    lastStep = new Fl_Button(0, 0, W, H, "last step");
    lastStep->labelfont(FL_HELVETICA_BOLD);
    lastStep->hide();
    nextStep = new Fl_Button(0, 0, W, H, "next step");
    nextStep->labelfont(FL_HELVETICA_BOLD);
    nextStep->hide();
    back2Design = new Fl_Button(0, 0, W, H, "Desgin");
    back2Design->labelfont(FL_HELVETICA_BOLD);
    back2Design->hide();
    startButton->callback(start_callback, this);
    clearButton->callback(clear_callback, this);
    lastLevel->callback(lastLevel_callback, this);
    nextLevel->callback(nextLevel_callback, this);
    loadButton->callback(load_callback, this);
    solveButton->callback(solve_callback, this);
    lastStep->callback(lastStep_callback, this);
    nextStep->callback(nextStep_callback, this);
    back2Design->callback(design_callback, this);
#ifdef LEVEL_DESIGN
    printDesign = new Fl_Button(0, 0, W, H, "Print");
    printDesign->labelfont(FL_HELVETICA_BOLD);
    printDesign->callback(printLevel_callback, this);
#endif
}

RushWindow::~RushWindow() {
    delete solver;
    delete backgroundImage;
    delete background->image();
    delete background;
    delete startButton;
    delete designBoard;
    delete vehicleBoard;
    delete ruleLabel;
    delete levelBoard;
    delete clearButton;
    delete loadButton;
    delete lastLevel;
    delete nextLevel;
    delete rushBoard;
    delete solveButton;
    delete stepLabel;
    delete lastStep;
    delete nextStep;
    delete back2Design;
#ifdef LEVEL_DESIGN
    delete printDesign;
#endif
}

void RushWindow::initSolver() {
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

void RushWindow::StepForward() {
    if (rushBoard->nextStep())
        stepLabel->nextStep();
    this->redraw();
}

void RushWindow::StepBackward() {
    if (rushBoard->lastStep())
        stepLabel->lastStep();
    this->redraw();
}

void RushWindow::resize(int X, int Y, int W, int H) {
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
    clearButton->resize(canvas_X + side, canvas_Y + side * 31 / 18, side * 2 / 9, side / 9);
    clearButton->labelsize(side / 20);
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
    lastStep->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side * 5 / 6, side * 3 / 5, side / 6);
    lastStep->labelsize(side / 12);
    nextStep->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side, side * 3 / 5, side / 6);
    nextStep->labelsize(side / 12);
    back2Design->resize(canvas_X + canvas_W - side * 4 / 5, canvas_Y + side * 5 / 3, side * 3 / 5, side / 6);
    back2Design->labelsize(side / 12);
#ifdef LEVEL_DESIGN
    printDesign->resize(canvas_X + side, canvas_Y + side * 29 / 18, side * 2 / 9, side / 9);
    printDesign->labelsize(side / 20);
#endif
}

void RushWindow::reset() {
    solving = false;
    solveButton->label("Solve");
    vehicleBoard->reset();
    rushBoard->reset();
    stepLabel->reset();
}

#endif

};  // namespace RUSH