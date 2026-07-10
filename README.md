# Robot Movement (C++17 Console Program)

A production-grade, object-oriented C++17 implementation of the **Robot Movement** simulation on an $N \times N$ square grid. The robot reads commands from a text file, moves, and draws lines using **Bresenham's line algorithm**.

---

## 🏛 Architecture & OOP Design

The project is structured around clean object-oriented principles to ensure scalability, maintainability, and safety:

```
+----------------+      parses       +-------------------+
| CommandParser  | ----------------> |     ICommand      |
+----------------+                   +-------------------+
                                       ^       ^       ^
               +-----------------------+       |       +-----------------------+
               |                               |                               |
       +------------------+         +------------------+             +------------------+
       | DimensionCommand |         |  MoveToCommand   |             |  LineToCommand   |
       +------------------+         +------------------+             +------------------+
               |                               |                               |
               +-----------------------+       |       +-----------------------+
                                       v       v       v
                             +---------------------------+
                             |      RobotController      |
                             +---------------------------+
                                           |
                                           v
                             +---------------------------+
                             |          IGrid            |
                             +---------------------------+
                                           ^
                                           |
                             +---------------------------+
                             |         TextGrid          |
                             +---------------------------+
```

### Key Design Highlights:
1. **Extensible Commands (Command Pattern)**:
   - `ICommand` defines an abstract interface for executing commands against `RobotController`.
   - Adding new commands (e.g., `CIRCLE_TO`, `ARC_TO`) requires implementing a subclass of `ICommand` without modifying existing command classes or grid rendering logic.

2. **Scalable Grid Rendering Interface (`IGrid`)**:
   - `IGrid` decouples the robot's movement logic from how the grid is stored or rendered.
   - Currently implemented via `TextGrid` for console rendering (`.` and `+`).
   - Can easily be extended to `ImageGrid` (e.g., exporting PNG/BMP) or UI canvas without modifying the controller.

3. **Bresenham's Line Algorithm (`Bresenham`)**:
   - Accurately computes integer grid cell coordinates for lines in all 8 octants with zero floating-point inaccuracies.

4. **Safety & Memory Management**:
   - Standard C++17 RAII (`std::shared_ptr`, `std::unique_ptr`, `std::vector`) ensures zero memory leaks.
   - Comprehensive domain-specific exception handling (`RobotMovementException`, `ParseException`, `OutOfGridException`).

5. **Performance Considerations**:
   - `TextGrid` stores the 2D grid in a single contiguous 1D `std::vector<char>` (`cells[y * dimension + x]`), providing $O(1)$ cache-friendly access and minimal memory overhead ($1 \text{ byte/cell}$).

---

## 🚀 Build Instructions (CMake)

### Requirements
- **C++17 Compiler** (GCC 7+, Clang 6+, MSVC 2017+)
- **CMake 3.14+**

### Build Steps

```bash
# 1. Create a build directory
mkdir -p build && cd build

# 2. Configure project with CMake
cmake ..

# 3. Compile executable and tests
cmake --build .
```

---

## 🧪 Running Unit & Integration Tests

The repository includes a comprehensive automated test suite verifying Bresenham line generation, error handling, out-of-bounds checks, and the PDF specification scenario.

```bash
# Run tests using ctest inside build directory:
ctest --output-on-failure

# Or execute test binary directly:
./robot_movement_tests
```

---

## 💻 Usage

Run the program by passing a command text file path:

```bash
./robot_movement ../examples/example1.txt
```

Or pipe commands via standard input (`stdin`):

```bash
cat ../examples/example1.txt | ./robot_movement
```

### Command Format
- **`DIMENSION N`**: Sets grid size to $N \times N$ ($N > 0$).
- **`MOVE_TO x, y`**: Moves robot to $(x, y)$ without drawing.
- **`LINE_TO x, y`**: Moves robot to $(x, y)$ while drawing a Bresenham line (`+`).

---

## 📋 Example Output

Input (`examples/example1.txt`):
```text
DIMENSION 5
MOVE_TO 1,1
LINE_TO 3,3
LINE_TO 3,2
```

Output:
```text
. . . . .
. + . . .
. . + + .
. . . + .
. . . . .
```
