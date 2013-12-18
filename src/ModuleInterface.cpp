#include <NaoFramework/Modules/ModuleInterface.hpp>

#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

// Some aliasing convenience
namespace logging = boost::log;
namespace src = boost::log::sources;

namespace NaoFramework {
    namespace Modules {
        ModuleInterface::ModuleInterface(std::string moduleName, boost::any * comm) : name_(moduleName), comm_(comm) {
            // Construct the sink
            sink_ = boost::make_shared< TextSink >();

            // Add a stream to write log to
            sink_->locked_backend()->add_stream(
                    boost::make_shared< std::ofstream >(name_));
            sink_->locked_backend()->auto_flush(true);
            std::cout << name_ << " Opened sink with name: " << name_ << '\n';

            // Register the sink in the logging core
            logging::core::get()->add_sink(sink_);

            src::logger lg;
            BOOST_LOG(lg) << "Hello world!" << name_;
        }

        ModuleInterface::ModuleInterface(ModuleInterface&& other) : name_(other.name_), comm_(other.comm_), sink_(other.sink_) {
            other.sink_.reset();  // So that it does not remove any sink when it dies
        }

        const ModuleInterface & ModuleInterface::operator=(ModuleInterface&& other) {
            name_ = other.name_;
            comm_ = other.comm_;

            std::cout << name_ << " move-assignment Dropping sink\n";
            sink_->flush();
            std::cout << name_ << " Flushed.\n";
            logging::core::get()->remove_sink(sink_);
            std::cout << name_ << " Done.\n";

            sink_ = other.sink_;
            other.sink_.reset();
        }

        ModuleInterface::~ModuleInterface() {
            // Sink can be empty in moved copies!
            if ( sink_ ) {
                std::cout << name_ << " Dropping sink\n";
                sink_->flush();
                std::cout << name_ << " Flushed.\n";
                logging::core::get()->remove_sink(sink_);
                std::cout << name_ << " Done.\n";
            }
        }

        std::string ModuleInterface::getName() const {
            return name_;
        }

        void ModuleInterface::log(const std::string & text) {
            src::logger lg;
            BOOST_LOG(lg) << text;
        }
    } // Modules
} //NaoFramework
