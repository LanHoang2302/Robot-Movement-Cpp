#include "robot_movement/Bresenham.hpp"
#include "robot_movement/CommandParser.hpp"
#include "robot_movement/Exceptions.hpp"
#include "robot_movement/Grid.hpp"
#include "robot_movement/RobotController.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace robot_movement;

void testBresenhamSinglePoint() {
    auto pts = Bresenham::getLinePoints({2, 2}, {2, 2});
    assert(pts.size() == 1);
    assert(pts[0] == Point(2, 2));
    std::cout << "[PASS] testBresenhamSinglePoint\n";
}

void testBresenhamDiagonal() {
    auto pts = Bresenham::getLinePoints({1, 1}, {3, 3});
    assert(pts.size() == 3);
    assert(pts[0] == Point(1, 1));
    assert(pts[1] == Point(2, 2));
    assert(pts[2] == Point(3, 3));
    std::cout << "[PASS] testBresenhamDiagonal\n";
}

void testBresenhamSteepLine() {
    auto pts = Bresenham::getLinePoints({3, 3}, {3, 2});
    assert(pts.size() == 2);
    assert(pts[0] == Point(3, 3));
    assert(pts[1] == Point(3, 2));
    std::cout << "[PASS] testBresenhamSteepLine\n";
}

void testPdfExampleScenario() {
    auto grid = std::make_shared<TextGrid>();
    RobotController controller(grid);

    std::string input =
        "DIMENSION 5\n"
        "MOVE_TO 1,1\n"
        "LINE_TO 3,3\n"
        "LINE_TO 3,2\n";

    std::istringstream iss(input);
    auto commands = CommandParser::parseStream(iss);
    assert(commands.size() == 4);

    for (const auto& cmd : commands) {
        cmd->execute(controller);
    }

    // Check cells based on PDF explanation:
    // (1,1), (2,2), (3,3), (3,2) should be '+'
    assert(grid->getCell(1, 1) == '+');
    assert(grid->getCell(2, 2) == '+');
    assert(grid->getCell(3, 3) == '+');
    assert(grid->getCell(3, 2) == '+');

    // Some unvisited checks
    assert(grid->getCell(0, 0) == '.');
    assert(grid->getCell(4, 4) == '.');
    assert(grid->getCell(1, 0) == '.');

    // Verify rendered output matches expected format
    std::ostringstream oss;
    grid->render(oss);
    std::string expected =
        ". . . . .\n"
        ". + . . .\n"
        ". . + + .\n"
        ". . . + .\n"
        ". . . . .\n";
    assert(oss.str() == expected);

    std::cout << "[PASS] testPdfExampleScenario\n";
}

void testErrorHandlingOutOfBounds() {
    auto grid = std::make_shared<TextGrid>();
    RobotController controller(grid);

    DimensionCommand(5).execute(controller);

    bool caught = false;
    try {
        MoveToCommand(5, 0).execute(controller);
    } catch (const OutOfGridException&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[PASS] testErrorHandlingOutOfBounds\n";
}

void testErrorHandlingInvalidCommand() {
    bool caught = false;
    try {
        CommandParser::parseLine("UNKNOWN_CMD 1, 2", 1);
    } catch (const ParseException& e) {
        caught = true;
        assert(e.line_number() == 1);
    }
    assert(caught);
    std::cout << "[PASS] testErrorHandlingInvalidCommand\n";
}

void testErrorHandlingMissingDimension() {
    auto grid = std::make_shared<TextGrid>();
    RobotController controller(grid);

    bool caught = false;
    try {
        MoveToCommand(1, 1).execute(controller);
    } catch (const GridNotInitializedException&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[PASS] testErrorHandlingMissingDimension\n";
}

int main() {
    std::cout << "Running Robot Movement Unit Tests...\n";
    testBresenhamSinglePoint();
    testBresenhamDiagonal();
    testBresenhamSteepLine();
    testPdfExampleScenario();
    testErrorHandlingOutOfBounds();
    testErrorHandlingInvalidCommand();
    testErrorHandlingMissingDimension();
    std::cout << "All tests passed successfully!\n";
    return 0;
}
