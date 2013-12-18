#include <NaoFramework/Modules/DynamicModule.hpp>
#include <NaoFramework/Console/Console.hpp>
#include <boost/any.hpp>

#include <map>
#include <vector>
#include <string>
#include <iostream>

using std::cout;
namespace naoth = NaoFramework::Modules;

std::vector<naoth::DynamicModule> sharedLibs;
boost::any comm;

void addModule(std::vector<std::string>& inputs) {
    if ( inputs.size() < 3 ) {
        std::cout << "Command: " << inputs[0] << " " << "module_name module_filename\n";
        return;
    }
    try {
        sharedLibs.emplace_back(inputs[1], inputs[2], &comm);
        std::cout << "Successfully loaded module: " << inputs[1] << "\n";
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

int main() {
    NaoFramework::Console::Console c("NaoFramework> ");
    NaoFramework::Console::Console b("OtherConsole> ");
    c.registerCommand("run",printModules);
    c.registerCommand("add",addModule);

    while ( c.readLine() ) {}

    return 0;
} 
