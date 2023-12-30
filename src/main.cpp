#include "Solver.hpp"
#include "SolveGUI.hpp"

using namespace RUSH;

int main() {
    Solver s;
    std::vector<BitBoard> solution(s.solve());
    RushWindow window(1200, 800, "Rush Hour", solution);
    window.end();
    window.show();
    return Fl::run();
}