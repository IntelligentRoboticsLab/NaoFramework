#ifndef NAO_FRAMEWORK_CONSOLE_CONSOLE_HEADER_FILE
#define NAO_FRAMEWORK_CONSOLE_CONSOLE_HEADER_FILE

#include <string>
#include <vector>
#include <unordered_map>

namespace NaoFramework {
    namespace Console {
        class Console {
            public:
                using CommandFunction = void(std::vector<std::string> &);

                Console(std::string greeting);

                void registerCommand(std::string s, CommandFunction * f);
                void readLine();

                struct History;
            private:
                using RegisteredCommands = std::unordered_map<std::string,CommandFunction*>;

                std::string greeting_;
                RegisteredCommands commands_;
                History * history_;

                void saveState();
                void reserveConsole();
                static Console * currentConsole_;

                // GNU newline interface to our commands.
                using commandCompleterFunction = char**(const char * text, int start, int end);
                using commandIteratorFunction = char*(const char * text, int state);
                
                commandCompleterFunction getCommandCompletions;
                commandIteratorFunction commandIterator;
                // These two we keep to avoid calling std::bind over and over
                commandCompleterFunction * bindedCompleterFunction_;
                commandIteratorFunction * bindedIteratorFunction_;
        };
    }
}

#endif
