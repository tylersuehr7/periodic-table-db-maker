#include "args.hpp"
#include "generator.hpp"

/// @brief Main application entry-point.
///
/// @param argc number of command-line arguments
/// @param argv values of command-line arguments
int main(const int argc, const char **argv) {
    Args args(argc, argv);
    Generator generator(args);

    generator.generateNewDatabase();

    return 0;
}
