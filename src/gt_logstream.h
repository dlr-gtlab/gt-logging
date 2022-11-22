#ifndef GT_LOGSTREAM_H
#define GT_LOGSTREAM_H

#include "gt_logging_exports.h"

#include <algorithm>
#include <sstream>
#include <memory>

namespace gt
{

namespace log
{

//! Enum for verbosity log levels
enum Verbosity
{
    Silent = 0,
    Medium = 5,
    Everything = 9
};


//! Enum for setting certain flags of a stream
enum StreamFlag
{
    LogSpace = 2, // 0b0010 else no space
    LogQuote = 4  // 0b0100 else no quote
};

class Stream;
//! Helper class to restore state of a stream object once destroyed.
//! Stream object may not go out of scope before state saver does
class StreamStateSaver
{
public:

    //! ctor (copies states)
    explicit StreamStateSaver(Stream& s);
    //! dtor (reapplies states)
    ~StreamStateSaver();

private:
    Stream* stream;
    std::ios_base::fmtflags iosflags;
    int flags;
    int vlevel;
};

//! Main stream class for logging most types. May work with Qt symbols as well.
class Stream
{
    friend class StreamStateSaver;

public:

    Stream() {
        // manipulators
        m_stream << std::boolalpha;
    };
    ~Stream() = default;

    Stream(Stream const&) = delete;
    Stream(Stream&&) = delete;
    Stream& operator=(Stream const&) = delete;
    Stream& operator=(Stream&&) = delete;

    inline Stream& space()   { m_flags |=  LogSpace; return *this; }
    inline Stream& nospace() { m_flags &= ~LogSpace; return *this; }
    inline Stream& quote()   { m_flags |=  LogQuote; return *this; }
    inline Stream& noquote() { m_flags &= ~LogQuote; return *this; }

    Stream& medium() { return verbose(gt::log::Medium); }
    Stream& verbose(int level = gt::log::Everything)
    {
        m_vlevel = level;
        return *this;
    }

    std::string str() { return m_stream.str(); }

    GT_LOGGING_EXPORT static bool mayLog(int level);
    bool mayLog() const { return mayLog(m_vlevel); }
    bool mayLogSpace() const { return m_flags & LogSpace; }
    bool mayLogQuote() const { return m_flags & LogQuote; }

    // pod
    inline Stream& operator<<(std::nullptr_t) { return doLog("(nullptr)"); }
    inline Stream& operator<<(void const* t)
    {
        { // block for state saver
            StreamStateSaver s{*this};
            // format pointers
            nospace().doLog(std::hex)
                     .doLog("0x")
                     .doLog(reinterpret_cast<std::uintptr_t>(t));
        }
        return doLogSpace();
    }

    inline Stream& operator<<(bool t) { return doLog(t); }

    // chars
    inline Stream& operator<<(char t) { return doLog(t); }
    // indicate wide chars
    inline Stream& operator<<(char16_t t)
    {
        { // block for state saver
            StreamStateSaver s{*this};
            nospace().operator<<("u'").doLog(t);
        }
        return operator<<('\'');
    }
    inline Stream& operator<<(char32_t t)
    {
        { // block for state saver
            StreamStateSaver s{*this};
            nospace().operator<<("U'").doLog(t);
        }
        return operator<<('\'');
    }

    // ints
    inline Stream& operator<<(short t) { return doLog(t); }
    inline Stream& operator<<(unsigned short t) { return doLog(t); }
    inline Stream& operator<<(int t) { return doLog(t); }
    inline Stream& operator<<(unsigned int  t) { return doLog(t); }
    inline Stream& operator<<(long t) { return doLog(t); }
    inline Stream& operator<<(unsigned long t) { return doLog(t); }
    inline Stream& operator<<(long long t) { return doLog(t); }
    inline Stream& operator<<(unsigned long long t) { return doLog(t); }

    // floats
    inline Stream& operator<<(float t) { return doLog(t); }
    inline Stream& operator<<(double t) { return doLog(t); }

    // strings
    inline Stream& operator<<(const char* t) { return doLog(t); }
    inline Stream& operator<<(std::string const& t) { return doLog(t); }

    // memory
    template <typename T, typename U>
    inline Stream& operator<<(std::unique_ptr<T, U> const& t) { return operator<<(t.get()); }
    template <typename T>
    inline Stream& operator<<(std::shared_ptr<T> const& t) { return operator<<(t.get()); }

    // ios flags
    inline Stream& operator<<(std::ios_base&(*t)(std::ios_base&))
    {
        m_stream << t;
        return *this;
    }

#if 0
    template <typename Vec,
              typename Iter = decltype(std::begin(std::declval<Vec const&>()))>
    inline Stream& operator<<(Vec const& t)
    {
        return doLogIter(std::begin(t), std::end(t));
    }

    template <typename T, typename U>
    inline Stream& operator<<(std::pair<T, U> const& t)
    {
        if (mayLog())
        {
            m_stream << '(';
            {
                StreamStateSaver s{*this};
                nospace();
                *this << t.first;
                m_stream << ", ";
                *this << t.second;
            }
            m_stream << ')';
            mayLogSpace();
        }
        return *this;
    }
#endif

    //! Default logging method
    template <typename T>
    inline Stream& doLog(T const& t)
    {
        if (mayLog())
        {
            m_stream << t;
            doLogSpace();
        }
        return *this;
    }

    //! Logging with quotes (if enabled)
    template <typename T>
    inline Stream& doLogQuoted(T const& t)
    {
        if (mayLog())
        {
            doLogQuote();
            m_stream << t;
            doLogQuote().doLogSpace();
        }
        return *this;
    }

#if 0
    template <typename Iter>
    inline Stream& doLogIter(Iter a, Iter b) noexcept
    {
        if (mayLog())
        {
            m_stream << '(';
            if (a != b)
            {
                StreamStateSaver s{*this};
                nospace();
                Iter llast = --b;
                // log until nth - 1 element
                std::for_each(a, llast, [this](auto const& val){
                    *this << val;
                    m_stream << ", ";
                });
                // log last lement
                *this << *b;
            }
            m_stream << ')';
            mayLogSpace();
        }
        return *this;
    };
#endif

private:

    /// flags
    int m_flags{gt::log::LogSpace};
    /// verbosity level
    int m_vlevel{gt::log::Silent};
    /// ostream
    std::ostringstream m_stream;

    //! Helper function to log ' '
    inline Stream& doLogSpace() noexcept
    {
        if (mayLogSpace()) m_stream << ' ';
        return *this;
    };

    //! Helper function to log '"'
    inline Stream& doLogQuote() noexcept
    {
        if (mayLogQuote()) m_stream << '"';
        return *this;
    };
};

} // namespace log

} // namespace gt

inline gt::log::StreamStateSaver::StreamStateSaver(Stream& s)
    : stream{&s}
    , iosflags{s.m_stream.flags()}
    , flags{s.m_flags}
    , vlevel{s.m_vlevel}
{}

inline gt::log::StreamStateSaver::~StreamStateSaver()
{
    stream->m_stream.setf(iosflags);
    stream->m_flags = flags;
    stream->m_vlevel = vlevel;
}

//template <typename Vec,
//          typename Iter = decltype(std::begin(std::declval<Vec const&>()))>
//inline Stream& operator<<(Vec const& t)
//{
//    return doLogIter(std::begin(t), std::end(t));
//}

// Qt support
#ifdef GT_LOG_USE_QT_BINDINGS
#include "gt_logbindings_qt.h"
#endif

#endif // GT_LOGSTREAM_H
