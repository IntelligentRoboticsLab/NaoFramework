#include <NaoFramework/Log/Frontend.hpp>

#include <ostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <mutex>

// Normally we would use std::shared_ptr
// But this library does not accept it
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/support/date_time.hpp>

// Some aliasing convenience
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace NaoFramework {
    namespace Log {
        // These are all the properties we want to be able to set/read about messages.
        BOOST_LOG_ATTRIBUTE_KEYWORD(clientAttribute, "Client", std::string)
        BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", MessagePriority)

        using TextSink = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;
        // Just as a warning, these two static globals, which are useful here to give a lightweight
        // log interface outside, will automatically die on program shutdown. This may cause
        // problems in case you are storing modules or logging stuff as globals. Don't do that!
        static std::unordered_map<std::string, boost::shared_ptr<TextSink>> availableSinks;

        static std::mutex availableSinksMutex;

        static std::string logFolder;

        void init(const std::string & logFolder) {
            try {
                boost::filesystem::create_directory(logFolder);
            }
            catch (const boost::filesystem::filesystem_error & e) {
                if (!boost::filesystem::is_directory(logFolder)) {
                    throw e;
                }
            }
            NaoFramework::Log::logFolder = logFolder;

            boost::shared_ptr< logging::core > core = logging::core::get();

            core->add_global_attribute("TimeStamp", attrs::local_clock());

            // We can avoid removing our sink because we're going to log during
            // the whole application anyway.
            makeSink("Log");
        }

        bool makeSink(const std::string & client) {
            bool result = false;
            availableSinksMutex.lock();
            auto it = availableSinks.find(client);

            if ( it == end(availableSinks) ) {
                // Construct the sink
                auto sink = boost::make_shared< TextSink >();

                // Add a stream to write log to
                sink->locked_backend()->add_stream(
                        boost::make_shared< std::ofstream >(logFolder+"/"+client));
                // Flush continuously, only in debug mode
                #if NAO_DEBUG
                sink->locked_backend()->auto_flush(true);
                #endif
                // Only get messages from caller 
                sink->set_filter(expr::has_attr(clientAttribute) && clientAttribute == client);

                sink->set_formatter
                (
                     expr::format("[%1%][%2%] %3%")
                         % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                         % logging::trivial::severity
                         % expr::smessage
                );

                logging::core::get()->add_sink(sink);

                log("Log", "Set sink for client: " + client );
                #if NAO_DEBUG
                log("Log", "DEBUG: Auto Flushing is true");
                #endif
                availableSinks[client] = sink;
                result = true;
            }

            availableSinksMutex.unlock();
            return result; // ONLY EXIT POINT HERE! UNLOCK MUTEX!
        }

        void removeSink(const std::string & client) {
            availableSinksMutex.lock();
            auto it = availableSinks.find(client);

            if ( it != end(availableSinks) ) {
                auto sink = it->second;
                log("Log", "Removing sink for client: " + client );
                sink->flush();
                logging::core::get()->remove_sink(sink);

                availableSinks.erase(it);
            }
            availableSinksMutex.unlock();
            return; // ONLY EXIT POINT HERE! UNLOCK MUTEX!
        }

        void log(const std::string & client, const std::string & message, MessagePriority priority) {
            using namespace logging::trivial;
            // Maybe this can improved by giving the loggers to the Modules directly.
            // Profiling first, though.
            src::severity_logger< severity_level > slg;
            slg.add_attribute("Client", attrs::constant< std::string >(client));
            BOOST_LOG_SEV(slg, static_cast<severity_level>(priority)) << message;
        }
    }
}
