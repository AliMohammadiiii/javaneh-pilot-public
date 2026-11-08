#include <ostream>
#include <streambuf>
#include <string>

#include <log.h>




syslog_streambuf::syslog_streambuf(const std::string& name, log::type type):
    std::basic_streambuf<char>()
{
    openlog(name.size() ? name.data() : nullptr, LOG_PID, type);
}
syslog_streambuf::~syslog_streambuf() { closelog(); }

auto syslog_streambuf::overflow(auto c = traits_type::eof()) 
{
    if(traits_type::eq_int_type(c, traits_type::eof()))
        sync();
    else buffer += traits_type::to_char_type(c);

    return c;
}

int syslog_streambuf::sync() 
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
