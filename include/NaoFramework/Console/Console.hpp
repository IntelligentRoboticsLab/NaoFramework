#ifndef NAO_FRAMEWORK_CONSOLE_CONSOLE_HEADER_FILE
#define NAO_FRAMEWORK_CONSOLE_CONSOLE_HEADER_FILE

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

namespace NaoFramework {
    namespace Console {
        class Console {
            public:
                using CommandFunction = std::function<unsigned(std::vector<std::string> &)>;

                enum ReturnCode {
                    Quit = -1,
                    Ok = 0,
                    Error = 1 // Or greater!
                };

                Console(std::string greeting);
                ~Console();

                void registerCommand(const std::string & s, CommandFunction f);
                std::vector<std::string> getRegisteredCommands() const;

                int executeCommand(const std::string & command);
                int executeFile(const std::string & filename);

                int readLine();
            private:
                using RegisteredCommands = std::unordered_map<std::string,CommandFunction>;

                std::string greeting_;
                RegisteredCommands commands_;
                // This is just to avoid importing library names in here
                void * history_;

                void saveState();
                void reserveConsole();
                static Console * currentConsole_;
                static void * emptyHistory_;

                // GNU newline interface to our commands.
                using commandCompleterFunction = char**(const char * text, int start, int end);
                using commandIteratorFunction = char*(const char * text, int state);

                static commandCompleterFunction getCommandCompletions;
                static commandIteratorFunction commandIterator;
        };
    }
}

#endif
