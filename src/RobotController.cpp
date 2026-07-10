#include "robot_movement/RobotController.hpp"
#include "robot_movement/Bresenham.hpp"
#include "robot_movement/Exceptions.hpp"
#include <stdexcept>

namespace robot_movement {

RobotController::RobotController(std::shared_ptr<IGrid> grid)
    : grid_(std::move(grid)) {
    if (!grid_) {
        throw std::invalid_argument("RobotController requires a non-null IGrid instance.");
    }
}

void RobotController::ensureGridInitialized(int x, int y) const {
    if (!grid_->isInitialized()) {
        throw GridNotInitializedException();
    }
    if (x < 0 || static_cast<size_t>(x) >= grid_->getDimension() ||
        y < 0 || static_cast<size_t>(y) >= grid_->getDimension()) {
        throw OutOfGridException(x, y, static_cast<int>(grid_->getDimension()));
    }
}

void RobotController::setDimension(size_t dimension) {
    grid_->setDimension(dimension);
    current_position_ = {0, 0};
}

void RobotController::moveTo(int x, int y) {
    ensureGridInitialized(x, y);
    current_position_ = {x, y};
}

void RobotController::lineTo(int x, int y) {
    ensureGridInitialized(x, y);
    Point target{x, y};
    auto points = Bresenham::getLinePoints(current_position_, target);
    for (const auto& pt : points) {
        grid_->markCell(pt);
    }
    current_position_ = target;
}

Point RobotController::getPosition() const noexcept {
    return current_position_;
}

IGrid& RobotController::getGrid() const {
    return *grid_;
}

} // namespace robot_movement
