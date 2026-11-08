#ifndef SYSLOG_HPP
#define SYSLOG_HPP
#endif // SYSLOG_HPP

#include <ostream>
#include <streambuf>
#include <string>

#include <log.h>




explicit syslog_streambuf::syslog_streambuf(const std::string& name, log::type type):
    std::basic_streambuf<char>()
{
    openlog(name.size() ? name.data() : nullptr, LOG_PID, type);
}
syslog_streambuf::~syslog_streambuf() override { closelog(); }

int_type syslog_streambuf::overflow(int_type c = traits_type::eof()) override
{
    if(traits_type::eq_int_type(c, traits_type::eof()))
        sync();
    else buffer += traits_type::to_char_type(c);

    return c;
}

int syslog_streambuf::sync() override
{
    if(buffer.size())
    {
        syslog(level, "%s", buffer.data());

        buffer.clear();
        level = ini_level;
    }
    return 0;
}

void syslog_streambuf::set_level(log::level new_level) noexcept { level = new_level; }


explicit syslog_stream::syslog_stream(const std::string& name = std::string(), log::type type = log::user):
    std::basic_ostream<char>(&streambuf),
    streambuf(name, type)
{ }

syslog_stream& syslog_stream::operator<<(log::level level) noexcept
{
    streambuf.set_level(level);
    return (*this);
}



