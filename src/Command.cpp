#include "robot_movement/Command.hpp"

namespace robot_movement {

DimensionCommand::DimensionCommand(size_t dimension)
    : dimension_(dimension) {}

void DimensionCommand::execute(RobotController& controller) const {
    controller.setDimension(dimension_);
}

MoveToCommand::MoveToCommand(int x, int y)
    : x_(x), y_(y) {}

void MoveToCommand::execute(RobotController& controller) const {
    controller.moveTo(x_, y_);
}

LineToCommand::LineToCommand(int x, int y)
    : x_(x), y_(y) {}

void LineToCommand::execute(RobotController& controller) const {
    controller.lineTo(x_, y_);
}

} // namespace robot_movement
