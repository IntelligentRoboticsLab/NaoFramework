#include <NaoFramework/Modules/ModuleInterface.hpp>

#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

// Some aliasing convenience
namespace logging = boost::log;

namespace NaoFramework {
    namespace Modules {
        ModuleInterface::ModuleInterface(std::string moduleName, boost::any * comm) : name_(moduleName), comm_(comm) {
            // Construct the sink
            sink_ = boost::make_shared< TextSink >();

            // Add a stream to write log to
            sink_->locked_backend()->add_stream(
                    boost::make_shared< std::ofstream >("ss"));

            // Register the sink in the logging core
            logging::core::get()->add_sink(sink_);
        }

        ModuleInterface::~ModuleInterface() {
            logging::core::get()->remove_sink(sink_);
        }

        std::string ModuleInterface::getName() const {
            return name_;
        }
    } // Modules
} //NaoFramework
