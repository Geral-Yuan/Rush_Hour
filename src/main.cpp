#include "Solver.hpp"
#include "Display.hpp"

using namespace RUSH;

int main() {
    Solver s;
    std::vector<BitBoard> solution(s.solve());
    RushWindow window(1200, 800, "Rush Hour", solution);
    return Fl::run();
}