#include "../../include/NaoFramework/Modules/DynamicModuleInterface.hpp"

#include <iostream>
#include <vector>

class Reader : public NaoFramework::Modules::DynamicModuleInterface {
    public:
        Reader(NaoFramework::Comm::LocalBlackboardAdapter & comm) : DynamicModuleInterface("Reader") {
            using namespace NaoFramework::Comm;
            RegistrationError e = RegistrationError::None;
            f_ = comm.registerRequire<int>("test", &e);
            if ( e != RegistrationError::None ) {
                log("Mistake..");
                throw e;
            }
            else {
                log("OK!");
            }
        }

        virtual ~Reader() { 
            log("Reader out");
        }

        virtual void execute() { 
            int data = f_();
            log("I'm Reader! From COM I read: "  + std::to_string(data));
        }
    private:
        NaoFramework::Comm::RequireFunction<int> f_;
};

MODULE_EXPORT(Reader)
