#include "solve.h"

using namespace RUSH;

int main() {
    RushWindow window(1200, 800, "Rush Hour");
    window.end();
    window.show();
    return Fl::run();
}