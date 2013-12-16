#include <NaoFramework/Console/Console.hpp>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>

#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

namespace NaoFramework {
    namespace Console {
        using Console::History = HISTORY_STATE;

        Console * Console::currentConsole_ = nullptr;

        // Here we set default commands, they do nothing since we quit with them
        // Quitting behaviour is hardcoded in run()
        Console::Console(std::string greeting) : greeting_(greeting), 
                                                 commands_({
                            {"quit",[](std::vector<std::string>&){}},
                            {"exit",[](std::vector<std::string>&){}}    })
        {
            using namespace std::placeholders;
            bindedCompleterFunction_ = 
                std::bind(&Console::getCommandCompletions,this, _2, _3, _4);
            bindedIteratorFunction_  =
                std::bind(&Console::commandIterator,      this, _2, _3);
        }

        void Console::registerCommand(std::string s, CommandFunction * f) {
            commands_[s] = f;
        }

        void Console::saveState() {
            history_ = history_get_history_state();
        }

        void Console::reserveConsole() {
            if ( currentConsole_ == this ) return;

            // Save state of other Console
            if ( currentConsole_ )
                currentConsole_->saveState();

            // Else we swap state
            history_set_history_state(history_);
            free(history_);

            rl_attempted_completion_function = bindedCompleterFunction_;
            // Tell others we are using the console
            currentConsole_ = this;
        }

        void Console::readline() {
            reserveConsole();

            RegisteredCommands::iterator it; 
            char * buffer = readline(greeting_.c_str());
            // Convert to C++ <3
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

            if ( inputs.size() == 0 ) continue;
            if ( inputs[0] == "quit" || inputs[0] == "exit" )
                break;

            if ( ( it = commands_.find(inputs[0]) ) != end(commands_) ) {
                (it->second)(inputs);
            }
            else {
                std::cout << "Command '" << inputs[0] << "' not found.\n";
            }
        }
        if ( !buffer )
            std::cout << "\n";  // EOF  doesn't put last endline so we put that.
        else
            free(buffer);
    }

    char ** Console::getCommandCompletions(const char * text, int start, int end) {
        char ** completionList = nullptr;

        if ( start == 0 )
            completionList = rl_completion_matches(text, bindedIteratorFunction_);

        return completionList;
    }

    char * Console::commandIterator(const char * text, int state) {
        static RegisteredCommands::iterator it;

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
