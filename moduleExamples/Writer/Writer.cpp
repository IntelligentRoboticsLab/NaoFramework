#include "../../include/NaoFramework/Modules/DynamicModuleInterface.hpp"

#include <iostream>
#include <vector>

class Writer : public NaoFramework::Modules::DynamicModuleInterface {
    public:
        // This can be anything!
        using MyData = std::vector<int>;

        Writer(boost::any * comm) : DynamicModuleInterface("Writer",comm) {}

        virtual ~Writer() { std::cout << "Writer died\n"; }

        virtual void print() { 
            MyData data;
            if ( comm_->empty() ) {
                data.push_back(5);

                std::cout << "I'm Writer! I'm setting comm to: " 
                          << data[0] << "\n";
                *comm_ = data;
            }
            else {
                auto data = boost::any_cast<MyData>(*comm_);

                ++(data[0]);
                std::cout << "I'm Writer! I'm upping comm to: " 
                          << data[0] << "\n";
                *comm_ = data;
            }
        }
};

MODULE_EXPORT(Writer)
