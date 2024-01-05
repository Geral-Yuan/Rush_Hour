#ifndef SOLVE_H
#define SOLVE_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <vector>
#include "Solver.h"
#include "design.h"

namespace RUSH {

class RushBoard : public Fl_Widget {
    bool solvable;
    const std::vector<BitBoard> *solution;
    size_t idx;

   public:
    RushBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H), solvable(false), solution(nullptr), idx(0) {}
    ~RushBoard() {
        if (solution)
            delete solution;
    }
    void reset();
    void setSolution(const std::pair<bool, std::vector<BitBoard> *> sol);
    size_t get_idx() const;
    bool isSolvable() const;
    void drawVehicle(int m, int k, int l, int len, int &color_idx) const;
    void draw() override;
    bool nextStep();
    bool lastStep();
};

class StepLabel : public Fl_Box {
    size_t step;
    std::string labelContent;

   public:
    StepLabel(int X, int Y, int W, int H, const char *L = 0, size_t _step = 0) : Fl_Box(X, Y, W, H, L), step(_step) {}

    void draw() override {
        int startX = x(), startY = y();
        int width = w(), height = h();
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, width / 6);
        labelContent = std::to_string(step) + " steps";
        label(labelContent.c_str());
        fl_draw(label(), startX, startY, width, height, FL_ALIGN_CENTER);
    };
    void nextStep() { ++step; }
    void lastStep() { --step; }
    void reset() { step = 0; }
};

class RushWindow : public Fl_Double_Window {
    Solver *solver;
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;
    Fl_Button *startButton;
    DesignBoard *designBoard;
    VehicleBoard *vehicleBoard;
    RuleLabel *ruleLabel;
    LevelBoard *levelBoard;
    Fl_Button *clearButton;
    Fl_Button *loadButton;
    Fl_Button *lastLevel;
    Fl_Button *nextLevel;
    RushBoard *rushBoard;
    Fl_Button *solveButton;
    StepLabel *stepLabel;
    Fl_Button *lastStep;
    Fl_Button *nextStep;
    Fl_Button *back2Design;
    bool solving;
#ifdef LEVEL_DESIGN
    Fl_Button *printDesign;
#endif

    friend void timer_callback(void *);
    friend void start_callback(Fl_Widget *, void *);
    friend void clear_callback(Fl_Widget *, void *);
    friend void load_callback(Fl_Widget *, void *);
    friend void lastLevel_callback(Fl_Widget *, void *);
    friend void nextLevel_callback(Fl_Widget *, void *);
    friend void solve_callback(Fl_Widget *, void *);
    friend void lastStep_callback(Fl_Widget *, void *);
    friend void nextStep_callback(Fl_Widget *, void *);
    friend void design_callback(Fl_Widget *, void *);
#ifdef LEVEL_DESIGN
    friend void printLevel_callback(Fl_Widget *, void *);
#endif

   public:
    RushWindow(int W, int H, const char *L = 0);
    ~RushWindow();
    void initSolver();
    void StepForward();
    void StepBackward();
    void resize(int X, int Y, int W, int H) override;
    void reset();
};

}  // namespace RUSH

#endif