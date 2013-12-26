#include "../../include/NaoFramework/Modules/DynamicModuleInterface.hpp"

#include <iostream>
#include <vector>

using namespace NaoFramework::Comm;

struct Test {
    ~Test() { std::cerr << "Bleah\n"; }
    int x;
    int y;
};

class Writer : public NaoFramework::Modules::DynamicModuleInterface {
    public:
        Writer(LocalBlackboardAdapter & c, ExternalBlackboardAdapterMap & m) : DynamicModuleInterface("Writer") {
            RegistrationError e = RegistrationError::None;
            f_ = c.registerProvide<Test>("qqq", &e);
            if ( e != RegistrationError::None ) {
                log("Mistake..");
                throw e;
            }
            else {
                log("OK!");
            }
        }

        virtual ~Writer() { 
            log("Writer out!");
        }

        virtual void execute() { 
            Test t;
            int testData = 5;
            
            log("I'm Writer! I'm setting comm");// to: " + std::to_string(testData));
            f_(t);
        }
    private:
        NaoFramework::Comm::ProvideFunction<Test> f_;
};

MODULE_EXPORT(Writer)
