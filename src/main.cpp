#include "Solver.hpp"
#include "Display.hpp"

using namespace RUSH;

int main(){
    Solver s;
    std::vector<BitBoard> ans(s.solve());
    RushWindow window(600,600,"Rush Hour",ans);
    window.show();
    Fl::add_timeout(1.0/2.0, timer_callback, window.rushboard);
    return Fl::run();
}