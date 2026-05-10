#include "cli/CommandLineParser.h"
#include "cli/commands/ComputeCommand.h"
#include "cli/commands/OptimizeCommand.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    CommandLineParser parser;
    ProgramOptions    opts = parser.parse(argc, argv);

    // Показать справку, если запрошено
    if (opts.showHelp) {
        parser.printUsage(argv[0]);
        return 0;
    }

    // Выполнить соответствующую команду
    try {
        std::unique_ptr<Command> command;

        switch (opts.command) {
            case CommandEnum::COMPUTE:
                command = std::make_unique<ComputeCommand>();
                break;

            case CommandEnum::OPTIMIZE:
                command = std::make_unique<OptimizeCommand>();
                break;

            default:
                std::cerr << "Error: Unknown command\n";
                parser.printUsage(argv[0]);
                return 1;
        }

        command->execute(opts);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
