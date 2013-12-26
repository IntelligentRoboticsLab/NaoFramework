#include "../../include/NaoFramework/Modules/DynamicModuleInterface.hpp"

#include <iostream>
#include <vector>

using namespace NaoFramework::Comm;

class Writer : public NaoFramework::Modules::DynamicModuleInterface {
    public:
        Writer(LocalBlackboardAdapter & c, ExternalBlackboardAdapterMap & m) : DynamicModuleInterface("Writer") {
            RegistrationError e = RegistrationError::None;
            f_ = c.registerProvide<int>("test", &e);
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
            int testData = 5;
            
            log("I'm Writer! I'm setting comm to: " + std::to_string(testData));
            f_(testData);
        }
    private:
        NaoFramework::Comm::ProvideFunction<int> f_;
};

MODULE_EXPORT(Writer)
