//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#if defined(__ANDROID__)

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

#include <android/log.h>

#include <mutex>
#include <string>

namespace spdlog {
namespace sinks {

/*
 * Android sink (logging using __android_log_write)
 */
template<class Mutex>
class base_android_sink : public base_sink<Mutex>
{
public:
    explicit base_android_sink(const std::string& tag = "spdlog"): _tag(tag)
    {
    }

    void flush() override
    {
    }

protected:
    void _sink_it(const details::log_msg& msg) override
    {
        const android_LogPriority priority = convert_to_android(msg.level);
        __android_log_write(priority, _tag.c_str(), msg.formatted.c_str());
    }

private:
    static android_LogPriority convert_to_android(spdlog::level::level_enum level)
    {
        switch(level)
        {
        case spdlog::level::trace:
            return ANDROID_LOG_VERBOSE;
        case spdlog::level::debug:
            return ANDROID_LOG_DEBUG;
        case spdlog::level::info:
            return ANDROID_LOG_INFO;
        case spdlog::level::warn:
            return ANDROID_LOG_WARN;
        case spdlog::level::err:
            return ANDROID_LOG_ERROR;
        case spdlog::level::critical:
            return ANDROID_LOG_FATAL;
        default:
            return ANDROID_LOG_INFO;
        }
    }

    std::string _tag;
};

typedef base_android_sink<std::mutex> android_sink_mt;
typedef base_android_sink<details::null_mutex> android_sink_st;

} // namespace sinks
} // namespace spdlog

#endif
