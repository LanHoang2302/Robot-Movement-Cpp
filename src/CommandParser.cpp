#include "robot_movement/CommandParser.hpp"
#include "robot_movement/Exceptions.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace robot_movement {

namespace {

std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

int parsePositiveOrZeroInt(const std::string& token, size_t line_num, const std::string& context) {
    std::string trimmed = trim(token);
    if (trimmed.empty()) {
        throw ParseException("Missing integer value for " + context, line_num);
    }
    for (char c : trimmed) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw ParseException("Invalid integer value '" + trimmed + "' for " + context, line_num);
        }
    }
    try {
        long val = std::stol(trimmed);
        if (val < 0) {
            throw ParseException("Negative integer value not allowed for " + context, line_num);
        }
        return static_cast<int>(val);
    } catch (const std::exception&) {
        throw ParseException("Integer overflow or conversion error for '" + trimmed + "'", line_num);
    }
}

std::pair<int, int> parseCoordinates(const std::string& args, size_t line_num, const std::string& cmd_name) {
    auto comma_pos = args.find(',');
    if (comma_pos == std::string::npos) {
        throw ParseException(cmd_name + " requires coordinates separated by a comma (x, y)", line_num);
    }
    std::string x_str = args.substr(0, comma_pos);
    std::string y_str = args.substr(comma_pos + 1);

    int x = parsePositiveOrZeroInt(x_str, line_num, cmd_name + " X coordinate");
    int y = parsePositiveOrZeroInt(y_str, line_num, cmd_name + " Y coordinate");
    return {x, y};
}

} // namespace

std::vector<std::unique_ptr<ICommand>> CommandParser::parseFile(const std::string& filepath) {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        throw ParseException("Unable to open input file: " + filepath);
    }
    return parseStream(ifs);
}

std::vector<std::unique_ptr<ICommand>> CommandParser::parseStream(std::istream& is) {
    std::vector<std::unique_ptr<ICommand>> commands;
    std::string line;
    size_t line_num = 0;
    while (std::getline(is, line)) {
        ++line_num;
        auto cmd = parseLine(line, line_num);
        if (cmd) {
            commands.push_back(std::move(cmd));
        }
    }
    return commands;
}

std::unique_ptr<ICommand> CommandParser::parseLine(const std::string& raw_line, size_t line_num) {
    std::string line = trim(raw_line);
    if (line.empty() || line[0] == '#' || (line.size() >= 2 && line[0] == '/' && line[1] == '/')) {
        return nullptr;
    }

    // Split command keyword and arguments
    auto space_pos = line.find_first_of(" \t");
    std::string keyword = line.substr(0, space_pos);
    std::string args = (space_pos != std::string::npos) ? trim(line.substr(space_pos + 1)) : "";

    if (keyword == "DIMENSION") {
        if (args.empty()) {
            throw ParseException("DIMENSION command requires a positive integer argument N", line_num);
        }
        int dim = parsePositiveOrZeroInt(args, line_num, "DIMENSION");
        if (dim <= 0) {
            throw ParseException("DIMENSION N must be greater than 0", line_num);
        }
        return std::make_unique<DimensionCommand>(static_cast<size_t>(dim));
    } else if (keyword == "MOVE_TO") {
        auto [x, y] = parseCoordinates(args, line_num, "MOVE_TO");
        return std::make_unique<MoveToCommand>(x, y);
    } else if (keyword == "LINE_TO") {
        auto [x, y] = parseCoordinates(args, line_num, "LINE_TO");
        return std::make_unique<LineToCommand>(x, y);
    } else {
        throw ParseException("Unknown command: '" + keyword + "'", line_num);
    }
}

} // namespace robot_movement
