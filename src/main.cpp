#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Console/Console.hpp>
#include <NaoFramework/Log/Frontend.hpp>
#include <boost/any.hpp>

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
    boost::any comm;

    void addModule(std::vector<std::string>& inputs) {
        sharedLibs.reserve(5);
        if ( inputs.size() < 2 ) {
            std::cout << "Command: " << inputs[0] << " " << "module_name module_filename\n";
            return;
        }
        try {
            sharedLibs.emplace_back(NaoFramework::Modules::makeDynamicModule(inputs[1], &comm));
            std::cout << "Successfully loaded module: " << sharedLibs.back().getName() << "\n";
        }
        catch ( std::runtime_error & e ) {
            std::cout << "Could not load module: " << e.what() << "\n";
        }
    }

    void printModules(std::vector<std::string>&) {
        for ( auto & module : sharedLibs ) {
            std::cout << "Main print:\n";
            module.print();
        }
    }
};

int main() {
    // Must call this! 
    NaoFramework::Log::init();

    NaoFramework::Console::Console c("NaoFramework> ");
    NaoFramework::Console::Console b("OtherConsole> ");

    FrameworkTest f;
    using namespace std::placeholders;
    c.registerCommand("run",std::bind(&FrameworkTest::printModules, &f, _1));
    c.registerCommand("add",std::bind(&FrameworkTest::addModule,    &f, _1));

    while ( c.readLine() ) {}

    return 0;
} 
