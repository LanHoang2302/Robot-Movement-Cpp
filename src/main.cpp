#include "robot_movement/CommandParser.hpp"
#include "robot_movement/Exceptions.hpp"
#include "robot_movement/Grid.hpp"
#include "robot_movement/RobotController.hpp"
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        std::vector<std::unique_ptr<robot_movement::ICommand>> commands;

        if (argc >= 2) {
            std::string filepath = argv[1];
            commands = robot_movement::CommandParser::parseFile(filepath);
        } else {
            std::cerr << "Usage: " << argv[0] << " <input_commands_file.txt>\n";
            std::cerr << "Reading from standard input (stdin)...\n";
            commands = robot_movement::CommandParser::parseStream(std::cin);
        }

        auto grid = std::make_shared<robot_movement::TextGrid>();
        robot_movement::RobotController controller(grid);

        for (const auto& cmd : commands) {
            cmd->execute(controller);
        }

        grid->render(std::cout);
        return 0;

    } catch (const robot_movement::RobotMovementException& e) {
        std::cerr << "Robot Movement Error: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << '\n';
        return 2;
    }
}
