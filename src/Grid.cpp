#include "robot_movement/Grid.hpp"
#include "robot_movement/Exceptions.hpp"
#include <iostream>
#include <stdexcept>

namespace robot_movement {

TextGrid::TextGrid(size_t dimension) {
    setDimension(dimension);
}

void TextGrid::setDimension(size_t dimension) {
    if (dimension == 0) {
        throw RobotMovementException("Grid dimension N must be a positive integer > 0.");
    }
    dimension_ = dimension;
    initialized_ = true;
    cells_.assign(dimension_ * dimension_, UNVISITED_SYMBOL);
}

void TextGrid::validateBounds(int x, int y) const {
    if (!initialized_) {
        throw GridNotInitializedException();
    }
    if (x < 0 || static_cast<size_t>(x) >= dimension_ ||
        y < 0 || static_cast<size_t>(y) >= dimension_) {
        throw OutOfGridException(x, y, static_cast<int>(dimension_));
    }
}

void TextGrid::markCell(int x, int y) {
    validateBounds(x, y);
    cells_[static_cast<size_t>(y) * dimension_ + static_cast<size_t>(x)] = VISITED_SYMBOL;
}

void TextGrid::markCell(const Point& p) {
    markCell(p.x, p.y);
}

char TextGrid::getCell(int x, int y) const {
    validateBounds(x, y);
    return cells_[static_cast<size_t>(y) * dimension_ + static_cast<size_t>(x)];
}

void TextGrid::render(std::ostream& os) const {
    if (!initialized_) {
        os << "Grid not initialized.\n";
        return;
    }
    for (size_t y = 0; y < dimension_; ++y) {
        for (size_t x = 0; x < dimension_; ++x) {
            os << cells_[y * dimension_ + x];
            if (x + 1 < dimension_) {
                os << ' ';
            }
        }
        os << '\n';
    }
}

size_t TextGrid::getDimension() const noexcept {
    return dimension_;
}

bool TextGrid::isInitialized() const noexcept {
    return initialized_;
}

} // namespace robot_movement
