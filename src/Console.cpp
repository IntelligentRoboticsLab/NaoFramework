#include <NaoFramework/Console/Console.hpp>

#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>
#include <sstream>

#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

namespace NaoFramework {
    namespace Console {
        Console * Console::currentConsole_  = nullptr;
        void * Console::emptyHistory_       = static_cast<void*>(history_get_history_state());

        // Here we set default commands, they do nothing since we quit with them
        // Quitting behaviour is hardcoded in readLine()
        Console::Console(std::string greeting) : greeting_(greeting),
                                                 commands_({
                                                    {"quit",[](std::vector<std::string>&){}},
                                                    {"exit",[](std::vector<std::string>&){}}}),
                                                 history_(nullptr)
        {
            // Init readline basics
            rl_attempted_completion_function = &Console::getCommandCompletions;
        }

        Console::~Console() {
            free(history_);
        }

        void Console::registerCommand(std::string s, CommandFunction * f) {
            commands_[s] = f;
        }

        void Console::saveState() {
            free(history_);
            history_ = static_cast<void*>(history_get_history_state());
        }

        void Console::reserveConsole() {
            if ( currentConsole_ == this ) return;

            // Save state of other Console
            if ( currentConsole_ )
                currentConsole_->saveState();

            // Else we swap state
            if ( ! history_ )
                history_set_history_state(static_cast<HISTORY_STATE*>(emptyHistory_));
            else
                history_set_history_state(static_cast<HISTORY_STATE*>(history_));

            // Tell others we are using the console
            currentConsole_ = this;
        }

        bool Console::readLine() {
            reserveConsole();
            for ( auto & it : commands_ )
                std::cout << it.first << "\n";

            RegisteredCommands::iterator it;
            char * buffer = readline(greeting_.c_str());
            if ( !buffer ) {
                std::cout << '\n'; // EOF  doesn't put last endline so we put that.
                return false;
            }

            // Convert input to C++ <3
            std::vector<std::string> inputs;
            {
                std::istringstream iss(buffer);
                std::copy(std::istream_iterator<std::string>(iss),
                        std::istream_iterator<std::string>(),
                        std::back_inserter(inputs));
            }
            // TODO: Maybe add commands to history only if succeeded?
            if ( buffer[0] != '\0' )
                add_history(buffer);

            free(buffer);

            if ( inputs.size() == 0 ) return true;
            if ( inputs[0] == "quit" || inputs[0] == "exit" ) return false;

            if ( ( it = commands_.find(inputs[0]) ) != end(commands_) ) {
                (it->second)(inputs);
            }
            else {
                std::cout << "Command '" << inputs[0] << "' not found.\n";
            }
            return true;
        }

        char ** Console::getCommandCompletions(const char * text, int start, int) {
            char ** completionList = nullptr;

            if ( start == 0 )
                completionList = rl_completion_matches(text, &Console::commandIterator);

            return completionList;
        }

        char * Console::commandIterator(const char * text, int state) {
            static RegisteredCommands::iterator it;
            auto & commands_ = currentConsole_->commands_;

            if ( state == 0 ) it = begin(commands_);

            while ( it != end(commands_ ) ) {
                auto & command = it->first;
                ++it;
                if ( command.find(text) != std::string::npos ) {
                    char * completion = new char[command.size()];
                    strcpy(completion, command.c_str());
                    return completion;
                }
            }
            return nullptr;
        }
    }
}
