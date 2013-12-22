#include <NaoFramework/Modules/Brain.hpp>
#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Console/Console.hpp>
#include <NaoFramework/Log/Frontend.hpp>
#include <NaoFramework/Comm/Blackboard.hpp>
#include <NaoFramework/Comm/ExternalBlackboardAdapter.hpp>
#include <NaoFramework/Comm/LocalBlackboardAdapter.hpp>

#include <map>
#include <vector>
#include <string>
#include <iostream>

using std::cout;

int main(int argc, const char * argv[]) {
    // Must call this!
    NaoFramework::Log::init();

    namespace pl = std::placeholders;
    using namespace NaoFramework::Console;
    using namespace NaoFramework::Modules;

    Console c("NaoFramework> ");
    Console b("OtherConsole> ");

    Brain brain;
    c.registerCommand("test",std::bind(&Brain::printModules, &brain, pl::_1));
    c.registerCommand("add", std::bind(&Brain::addModule,    &brain, pl::_1));

    cout << "\nWelcome to the NaoFramework command line interface!\n";
    // Default running script
    if ( argc > 1 ) {
        cout << "It looks like you have a script to run. Let's get to it.\n\n";
        if ( c.executeFile(argv[1]) == Console::ReturnCode::Quit ) return 0;
    }

    c.executeCommand("help");
    while ( c.readLine() != Console::ReturnCode::Quit ) {}

    return 0;
}
