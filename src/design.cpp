#include "design.h"

namespace RUSH {

Fl_Color vehicle_colors[16] = {
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
    fl_rgb_color(255, 102, 178),
    fl_rgb_color(153, 153, 255),
    fl_rgb_color(255, 204, 153)};

size_t mapped_idx[16] = {};

size_t IDs[6][6] = {};

const int LEVELS = 5;

size_t preDefined_levels[LEVELS][6][6] = {
    {{5, 5, 8, 0, 0, 0},
     {3, 0, 8, 15, 15, 15},
     {3, 0, 16, 16, 4, 0},
     {0, 0, 0, 0, 4, 0},
     {2, 2, 14, 14, 14, 9},
     {0, 0, 0, 0, 0, 9}},
    {{0, 0, 0, 0, 0, 0},
     {0, 0, 5, 0, 15, 0},
     {16, 16, 5, 0, 15, 4},
     {14, 14, 14, 0, 15, 4},
     {1, 2, 11, 0, 8, 8},
     {1, 2, 11, 0, 6, 6}},
    {{1, 1, 0, 14, 2, 8},
     {0, 0, 0, 14, 2, 8},
     {3, 16, 16, 14, 0, 13},
     {3, 0, 12, 6, 6, 13},
     {4, 4, 12, 0, 0, 13},
     {5, 5, 12, 0, 0, 0}},
    {{13, 3, 3, 0, 5, 15},
     {13, 2, 1, 0, 5, 15},
     {13, 2, 1, 16, 16, 15},
     {14, 14, 14, 8, 0, 0},
     {0, 0, 4, 8, 11, 11},
     {6, 6, 4, 10, 10, 0}},
    {{15, 1, 2, 2, 3, 0},
     {15, 1, 4, 0, 3, 13},
     {15, 0, 4, 16, 16, 13},
     {14, 14, 14, 11, 0, 13},
     {0, 0, 8, 11, 9, 9},
     {6, 6, 8, 5, 5, 0}}};

struct vehicleState {
    bool settle;
    bool horizontal;
    int row, col;
} v_states[LEVELS][16] = {
    {{1, 1, 2, 2},
     {0, 1, 0, 0},
     {1, 1, 4, 0},
     {1, 0, 1, 0},
     {1, 0, 2, 4},
     {1, 1, 0, 0},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 0, 0, 2},
     {1, 0, 4, 5},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 1, 4, 2},
     {1, 1, 1, 3}},
    {{1, 1, 2, 0},
     {1, 0, 4, 0},
     {1, 0, 4, 1},
     {0, 1, 0, 0},
     {1, 0, 2, 5},
     {1, 0, 1, 2},
     {1, 1, 5, 4},
     {0, 1, 0, 0},
     {1, 1, 4, 4},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 0, 4, 2},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 1, 3, 0},
     {1, 0, 1, 4}},
    {{1, 1, 2, 1},
     {1, 1, 0, 0},
     {1, 0, 0, 4},
     {1, 0, 2, 0},
     {1, 1, 4, 0},
     {1, 1, 5, 0},
     {1, 1, 3, 3},
     {0, 1, 0, 0},
     {1, 0, 0, 5},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 0, 3, 2},
     {1, 0, 2, 5},
     {1, 0, 0, 3},
     {0, 1, 0, 0}},
    {{1, 1, 2, 3},
     {1, 0, 1, 2},
     {1, 0, 1, 1},
     {1, 1, 0, 1},
     {1, 0, 4, 2},
     {1, 0, 0, 4},
     {1, 1, 5, 0},
     {0, 1, 0, 0},
     {1, 0, 3, 3},
     {0, 1, 0, 0},
     {1, 1, 5, 3},
     {1, 1, 4, 4},
     {0, 1, 0, 0},
     {1, 0, 0, 0},
     {1, 1, 3, 0},
     {1, 0, 0, 5}},
    {{1, 1, 2, 3},
     {1, 0, 0, 1},
     {1, 1, 0, 2},
     {1, 0, 0, 4},
     {1, 0, 1, 2},
     {1, 1, 5, 3},
     {1, 1, 5, 0},
     {0, 1, 0, 0},
     {1, 0, 4, 2},
     {1, 1, 4, 4},
     {0, 1, 0, 0},
     {1, 0, 3, 3},
     {0, 1, 0, 0},
     {1, 0, 1, 5},
     {1, 1, 3, 0},
     {1, 0, 0, 0}}};

bool isLegal(int i, int j) { return i >= 0 && i < 6 && j >= 0 && j < 6; }

bool isHorizontal(int i, int j) {
    return (isLegal(i, j - 1) && IDs[i][j - 1] == IDs[i][j]) || (isLegal(i, j + 1) && IDs[i][j + 1] == IDs[i][j]);
}

void drawBoard(int startX, int startY, int cellSize) {
    int side = cellSize * 6;
    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID, 1 + cellSize / 30);
    fl_line(startX, startY, startX, startY + side);
    fl_line(startX, startY, startX + side, startY);
    fl_line(startX, startY + side, startX + side, startY + side);
    fl_line(startX + side, startY, startX + side, startY + cellSize * 2);
    fl_line(startX + side, startY + cellSize * 3, startX + side, startY + side);
    fl_line_style(0);
}

void Vehicle::draw() {
    fl_color(vehicle_colors[color_idx]);
    fl_rectf(drawX, drawY, w(), h());
}

void Vehicle::Settle(bool h) {
    settle = cover = true;
    horizontal = h;
}
void Vehicle::Stay() {
    settle = cover = false;
    horizontal = true;
}
void Vehicle::setPos(int row, int col) { pos = std::make_pair(row, col); }
void Vehicle::reset() {
    horizontal = true;
    dragging = cover = settle = false;
}

void Vehicle::resize(int X, int Y, int W, int H) {
    Fl_Widget::resize(X, Y, W, H);
    if (!dragging) {
        drawX = X;
        drawY = Y;
    }
}

bool Vehicle::inSideVehicle(int x, int y) const { return x >= drawX && x <= drawX + w() && y >= drawY && y <= drawY + h(); }

void Vehicle::changeDirection() {
    horizontal = !horizontal;
    int delta = w() / 2 - h() / 2;
    drawX += delta;
    drawY -= delta;
    dx -= delta;
    dy += delta;
    this->size(h(), w());
}

void Vehicle::checkFullRow(int &coverCnt) {
    for (int i = 0; i < 6; ++i) {
        if (i == 2) continue;
        int carCnt = 0, truckCnt = 0;
        for (int j = 0; j < 6; j += 2)
            if (IDs[i][j] > 0 && IDs[i][j] == IDs[i][j + 1])
                ++carCnt;
            else
                break;
        if (carCnt == 3) {
            coverCnt = 0;
            break;
        }
        for (int j = 0; j < 6; j += 3)
            if (IDs[i][j] > 0 && IDs[i][j] == IDs[i][j + 1] && IDs[i][j] == IDs[i][j + 2])
                ++truckCnt;
            else
                break;
        if (truckCnt == 2) {
            coverCnt = 0;
            break;
        }
    }
}

void Vehicle::checkFullCol(int &coverCnt) {
    for (int j = 0; j < 6; ++j) {
        int carCnt = 0, truckCnt = 0;
        for (int i = 0; i < 6; i += 2)
            if (IDs[i][j] > 0 && IDs[i][j] == IDs[i + 1][j])
                ++carCnt;
            else
                break;
        if (carCnt == 3) {
            coverCnt = 0;
            break;
        }
        for (int i = 0; i < 6; i += 3)
            if (IDs[i][j] > 0 && IDs[i][j] == IDs[i + 1][j] && IDs[i][j] == IDs[i + 2][j])
                ++truckCnt;
            else
                break;
        if (truckCnt == 2) {
            coverCnt = 0;
            break;
        }
    }
}

void Vehicle::check() {
    int coverCnt = 0;
    bool firstCover = true;
    std::pair<int, int> localPos;
    size_t id = color_idx == 0 ? 16 : color_idx;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            if (inSideVehicle(board_X + j * cellSize + cellSize / 2, board_Y + i * cellSize + cellSize / 2)) {
                if ((IDs[i][j] == 0 || IDs[i][j] == id)) {
                    if (id == 16 && i != 2) break;
                    ++coverCnt;
                    IDs[i][j] = id;
                    if (firstCover) {
                        if (id == 16 && i == 2 && j == 4) break;
                        if (id != 16 && horizontal && i == 2) break;
                        localPos = std::make_pair(i, j);
                        firstCover = false;
                    }
                }
            } else if (IDs[i][j] == id) {
                IDs[i][j] = 0;
            }
    checkFullRow(coverCnt);
    checkFullCol(coverCnt);

    if ((isCar && coverCnt == 2) || (!isCar && coverCnt == 3)) {
        cover = true;
        pos = localPos;
    } else {
        cover = false;
        settle = false;
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 6; ++j)
                if (IDs[i][j] == id)
                    IDs[i][j] = 0;
    }
}

int Vehicle::handle(int event) {
    int event_x = Fl::event_x();
    int event_y = Fl::event_y();

    switch (event) {
        case FL_FOCUS:
        case FL_UNFOCUS:
            return 1;
        case FL_KEYBOARD: {
            if (dragging) {
                changeDirection();
                window()->redraw();
                return 1;
            }
            break;
        }
        case FL_PUSH: {
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                if (inSideVehicle(event_x, event_y)) {
                    dragging = true;
                    Fl_Widget::take_focus();
                    dx = event_x - drawX;
                    dy = event_y - drawY;
                    return 1;
                }
            }
            break;
        }
        case FL_DRAG: {
            if (dragging) {
                drawX = event_x - dx;
                drawY = event_y - dy;
                check();
                window()->redraw();
                return 1;
            }
            break;
        }
        case FL_RELEASE: {
            if (dragging) {
                dragging = false;
                if (cover) {
                    settle = true;
                    drawX = board_X + cellSize * pos.second + cellSize * 0.1;
                    drawY = board_Y + cellSize * pos.first + cellSize * 0.1;
                } else {
                    if (!horizontal) changeDirection();
                    drawX = x();
                    drawY = y();
                }
                window()->redraw();
                return 1;
            }
            break;
        }
    }
    return 0;
}

void DesignBoard::drawGrids() const {
    int X = x(), Y = y();
    int size = w() / 6;
    int cellSize = 0.8 * size, gap = (size - cellSize) / 2;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j) {
            if (IDs[i][j])
                fl_color(FL_WHITE);
            else
                fl_color(fl_rgb_color(224, 224, 224));
            fl_rectf(X + j * size + gap, Y + i * size + gap, cellSize, cellSize);
        }
}

void DesignBoard::draw() {
    int startX = x(), startY = y();
    int side = w(), cellSize = side / 6;
    drawBoard(startX, startY, cellSize);
    drawGrids();
}

VehicleBoard::VehicleBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {
    for (int i = 0; i < 12; ++i)
        vehicles[i] = new Vehicle(0, 0, W, H, true, i);
    for (int i = 12; i < 16; ++i)
        vehicles[i] = new Vehicle(0, 0, W, H, false, i);
}
VehicleBoard::~VehicleBoard() {
    for (int i = 0; i < 16; ++i)
        delete vehicles[i];
}
void VehicleBoard::draw() {
    int X = x(), Y = y();
    int size = w() / 6;
    int width = 0.8 * size, carLength = 1.8 * size, truckLength = 2.8 * size;
    int gap = (size - width) / 2;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j) {
            Vehicle *v = vehicles[i * 3 + j];
            if (v->stay())
                v->resize(X + j * 2 * size + gap, Y + i * size + gap, carLength, width);
            else if (v->settled()) {
                int w = v->isHorizontal() ? carLength : width;
                int h = v->isHorizontal() ? width : carLength;
                v->resize(board_X + cellSize * v->posCol() + gap, board_Y + cellSize * v->posRow() + gap, w, h);
            }
        }
    for (int i = 4; i < 6; ++i)
        for (int j = 0; j < 2; ++j) {
            Vehicle *v = vehicles[12 + (i - 4) * 2 + j];
            if (v->stay())
                v->resize(X + j * 3 * size + gap, Y + i * size + gap, truckLength, width);
            else if (v->settled()) {
                int w = v->isHorizontal() ? truckLength : width;
                int h = v->isHorizontal() ? width : truckLength;
                v->resize(board_X + cellSize * v->posCol() + gap, board_Y + cellSize * v->posRow() + gap, w, h);
            }
        }
}
void VehicleBoard::resize(int X, int Y, int W, int H) {
    Fl_Widget::resize(X, Y, W, H);
    board_X = X - W * 4 / 3;
    board_Y = Y;
    cellSize = W / 6;
    for (int i = 0; i < 16; ++i) {
        vehicles[i]->board_X = board_X;
        vehicles[i]->board_Y = board_Y;
        vehicles[i]->cellSize = cellSize;
    }
}
void VehicleBoard::show() {
    Fl_Widget::show();
    for (int i = 0; i < 16; ++i)
        vehicles[i]->show();
}
void VehicleBoard::hide() {
    Fl_Widget::hide();
    for (int i = 0; i < 16; ++i)
        vehicles[i]->hide();
}
void VehicleBoard::set2level(int level) {
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            IDs[i][j] = preDefined_levels[level][i][j];
    for (int i = 0; i < 16; ++i) {
        vehicleState &vs = v_states[level][i];
        if (vs.settle) {
            vehicles[i]->Settle(vs.horizontal);
            vehicles[i]->setPos(vs.row, vs.col);
        } else {
            vehicles[i]->Stay();
        }
    }
}
void VehicleBoard::reset() {
    memset(IDs, 0, sizeof(IDs));
    memset(mapped_idx, 0, sizeof(mapped_idx));
    for (int i = 0; i < 16; ++i) vehicles[i]->reset();
}

#ifdef LEVEL_DESIGN
void VehicleBoard::printState() const {
    std::cout << "IDs:\n";
    for (int i = 0; i < 6; ++i) {
        std::cout << "{";
        for (int j = 0; j < 6; ++j) {
            std::cout << IDs[i][j];
            if (j < 5)
                std::cout << ",";
            else if (i < 5)
                std::cout << "},\n";
            else
                std::cout << "}\n";
        }
    }
    std::cout << "Vehicles' States:\n";
    for (int i = 0; i < 16; ++i) {
        Vehicle *v = vehicles[i];
        std::cout << "{" << v->settled() << "," << v->isHorizontal() << ",";
        if (v->settled())
            std::cout << v->posRow() << "," << v->posCol();
        else
            std::cout << "0,0";
        std::cout << "}";
        if (i < 15)
            std::cout << ",\n";
        else
            std::cout << "\n";
    }
}
#endif

void LevelBoard::lastLevel() { level = (level + LEVELS - 1) % LEVELS; }

void LevelBoard::nextLevel() { level = (level + 1) % LEVELS; }

void LevelBoard::draw() {
    int X = x(), Y = y();
    int size = w() / 6;
    int width = 0.8 * size, carLength = 1.8 * size, truckLength = 2.8 * size;
    int gap = (size - width) / 2;
    drawBoard(X, Y, size);
    for (int i = 0; i < 16; ++i) {
        vehicleState &vs = v_states[level][i];
        int length = i < 12 ? carLength : truckLength;
        if (vs.settle) {
            int w = vs.horizontal ? length : width;
            int h = vs.horizontal ? width : length;
            fl_color(vehicle_colors[i]);
            fl_rectf(X + size * vs.col + gap, Y + size * vs.row + gap, w, h);
        }
    }
}

};  // namespace RUSH