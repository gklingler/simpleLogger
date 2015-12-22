#include "simpleLogger.h"

#include <boost/log/core/core.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <ostream>


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", boost::log::aux::thread::id)

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt) {
    src::severity_logger_mt<boost::log::trivial::severity_level> logger;

    // add attributes
    logger.add_attribute("LineID", attrs::counter<unsigned int>(0));     // lines are sequentially numbered
    logger.add_attribute("TimeStamp", attrs::local_clock());             // each log line gets a timestamp
    logger.add_attribute("ThreadID", attrs::current_thread_id());


    // text backend
    boost::shared_ptr<sinks::text_file_backend> t_backend = boost::make_shared<sinks::text_file_backend>(
        keywords::file_name = LOGFILE"_%Y-%m-%d.%N.log",
        keywords::open_mode = std::ios::app,
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::min_free_space = 30 * 1024 * 1024);

    // add a text sink
    typedef sinks::synchronous_sink<sinks::text_file_backend> text_file_sink;
    boost::shared_ptr<text_file_sink> t_sink = boost::make_shared<text_file_sink>(t_backend);

    // auto_flush
    t_sink->locked_backend()->auto_flush(true);

    // specify the format of the log message
    logging::formatter t_formatter = expr::format("[%1%](%2%): [%3%]%4%")
        % thread_id
        % expr::format_date_time(timestamp, "%H:%M:%S.%f")
        % logging::trivial::severity
        % expr::smessage;
    t_sink->set_formatter(t_formatter);


    // only messages with severity >= SEVERITY_THRESHOLD are written
    t_sink->set_filter(severity >= SEVERITY_THRESHOLD_FILE);

    // "register" our sink
    logging::core::get()->add_sink(t_sink);

#ifdef CONSOLE_LOG

    // add a text_ostream sink
    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

    // add "console" output stream to our sink
    sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

    // specify the format of the log message
    logging::formatter formatter = expr::stream
        << std::setw(7) << std::setfill('0') << line_id << std::setfill(' ') << " | "
        << expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f") << " "
        << "[" << logging::trivial::severity << "]"
        << " - " << expr::smessage;
    sink->set_formatter(formatter);

    // only messages with severity >= SEVERITY_THRESHOLD are written
    sink->set_filter(severity >= SEVERITY_THRESHOLD_CONSOLE);

    // "register" our sink
    logging::core::get()->add_sink(sink);
#endif

    return logger;
}
