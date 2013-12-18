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
                using CommandFunction = void(std::vector<std::string> &);

                Console(std::string greeting);
                ~Console();

                void registerCommand(std::string s, CommandFunction * f);
                bool readLine();

            private:
                using RegisteredCommands = std::unordered_map<std::string,CommandFunction*>;

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
