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
namespace naoth = NaoFramework::Modules;

struct FrameworkTest {
    //FrameworkTest() {}
    //FrameworkTest(const FrameworkTest &) = delete;
    //FrameworkTest & operator=(const FrameworkTest &) = delete;
    std::vector<naoth::DynamicModule> sharedLibs;
    NaoFramework::Comm::Blackboard b;

    FrameworkTest() : b("BBoard") {}

    unsigned addModule(std::vector<std::string>& inputs) {
        sharedLibs.reserve(5);
        if ( inputs.size() < 2 ) {
            std::cout << "Usage: " << inputs[0] << " module_filename\n";
            return 1;
        }

        unsigned loaded = 1; // 1 = Error!
        try {
            auto adapter = NaoFramework::Comm::LocalBlackboardAdapter(b);
            sharedLibs.emplace_back(NaoFramework::Modules::makeDynamicModule(inputs[1], adapter));
            std::cout << "Successfully loaded module: " << sharedLibs.back().getName() << "\n";
            loaded = 0;
        }
        catch ( std::runtime_error & e ) {
            std::cout << "Could not load module: " << e.what() << "\n";
        }
        return loaded;
    }

    unsigned printModules(std::vector<std::string>&) {
        if ( ! b.validateGlobals() ) {
            std::cout << "Dependencies are not met!\n";
            return 1;
        }
        for ( auto & module : sharedLibs ) {
            module.execute();
        }
        return 0;
    }
};

int main(int argc, const char * argv[]) {
    // Must call this!
    NaoFramework::Log::init();

    namespace pl = std::placeholders;
    using namespace NaoFramework::Console;

    Console c("NaoFramework> ");
    Console b("OtherConsole> ");

    FrameworkTest f;
    c.registerCommand("test",std::bind(&FrameworkTest::printModules, &f, pl::_1));
    c.registerCommand("add", std::bind(&FrameworkTest::addModule,    &f, pl::_1));

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
