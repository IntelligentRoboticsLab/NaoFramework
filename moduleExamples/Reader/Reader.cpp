#include "../../include/NaoFramework/Modules/DynamicModuleInterface.hpp"

#include <iostream>
#include <vector>

class Reader : public NaoFramework::Modules::DynamicModuleInterface {
    public:
        using SomeData = std::vector<int>;

        Reader(boost::any * comm) : DynamicModuleInterface("Reader",comm) {}

        virtual ~Reader() { std::cout << "Reader out\n"; }

        virtual void print() { 
            // This will crash if comm is not set or data is weird!
            std::cout << "I'm Reader! From COM I read: " 
                      << boost::any_cast<SomeData>(*comm_).at(0) << "\n";
        }
};

MODULE_EXPORT(Reader)
