// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGSTREAM_H
#define GT_LOGSTREAM_H

#include "gt_loglevel.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

#ifdef __has_cpp_attribute 
  #if __has_cpp_attribute(nodiscard)
    #define GT_LOG_NODISCARD [[nodiscard]]
  #else
    #define GT_LOG_NODISCARD
  #endif
#else
  #define GT_LOG_NODISCARD
#endif

namespace gt
{

namespace log
{

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
    Stream(Stream&&) = default;
    Stream& operator=(Stream const&) = delete;
    Stream& operator=(Stream&&) = default;

    /// enables space flag, thus outputs a space after logging an element.
    /// Will only affect the next message.
    inline Stream& space()   { m_flags |=  LogSpace; return *this; }
    /// disables space flag, thus outputs no space after logging an element.
    /// Will only affect the next message.
    inline Stream& nospace() { m_flags &= ~LogSpace; return *this; }
    /// enables the quote flag, which adds quotes around the next string
    /// type to log.
    inline Stream& quote()   { m_flags |=  LogQuote; return *this; }
    /// disbales the quote flag, thus logging string type without decorations.
    inline Stream& noquote() { m_flags &= ~LogQuote; return *this; }

    Stream& medium() { return verbose(gt::log::Medium); }
    Stream& verbose(int level = gt::log::Everything)
    {
        m_vlevel = level;
        return *this;
    }

    std::string str() { return m_stream.str(); }

    GT_LOGGING_EXPORT static bool mayLog(int level);
    GT_LOG_NODISCARD bool mayLog() const { return mayLog(m_vlevel); }
    GT_LOG_NODISCARD bool mayLogSpace() const { return m_flags & LogSpace; }
    GT_LOG_NODISCARD bool mayLogQuote() const { return m_flags & LogQuote; }

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
            nospace().doLog("u'").doLog(t);
        }
        return doLog('\'');
    }
    inline Stream& operator<<(char32_t t)
    {
        { // block for state saver
            StreamStateSaver s{*this};
            nospace().doLog("U'").doLog(t);
        }
        return doLog('\'');
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

    template <typename... Ts>
    inline Stream& operator<<(std::basic_string<Ts...> const& t) { return doLogQuoted(t); }

    // ios flags, like std::hex
    inline Stream& operator<<(std::ios_base&(*t)(std::ios_base&))
    {
        if (mayLog()) m_stream << t;
        return *this;
    }
    // ios operators, like std::endl etc
    inline Stream& operator<<(std::ostream&(*f)(std::ostream&))
    {
        if (mayLog()) f(m_stream);
        return *this;
    }
    // ios modifers, like setw, setprecision...
    // we have to check each type here, as the standard does not define a
    // common return type for these modifieres
    template <typename MANIP,
              std::enable_if_t<
                  std::is_same<MANIP, decltype(std::setw(0))>::value ||
                  std::is_same<MANIP, decltype(std::setprecision(0))>::value ||
                  std::is_same<MANIP, decltype(std::setbase(0))>::value, bool> = true>
    inline Stream& operator<<(MANIP const& manip)
    {
        if (mayLog()) m_stream << manip;
        return *this;
    }

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

    template <typename Iter>
    inline Stream& doLogIter(Iter a, Iter b,
                             char const* pre = "(",
                             char const* suf = ")",
                             char const* sep = ", ") noexcept;

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

private:

    /// flags
    int m_flags{gt::log::LogSpace};
    /// verbosity level
    int m_vlevel{gt::log::Silent};
    /// ostream
    std::ostringstream m_stream;
};

// pair
template <typename T, typename U>
inline Stream&
operator<<(Stream& s, std::pair<T, U> const& t)
{
    if (s.mayLog())
    {
        gt::log::StreamStateSaver saver{s};
        s.nospace().quote() << '(' << t.first << ", " << t.second;
    }
    return s << ')';
}

template <typename Iter>
inline Stream& Stream::doLogIter(Iter a, Iter b,
                                 char const* pre,
                                 char const* suf,
                                 char const* sep) noexcept
{
    if (mayLog())
    {
        m_stream << pre;
        if (a != b)
        {
            StreamStateSaver s{*this};
            nospace().quote();
            // some iterators can only iterate forward
            Iter b_1 = std::next(a, std::distance(a, b)-1);
            // log until nth - 1 element
            std::for_each(a, b_1, [this, sep](auto const& val){
                *this << val;
                m_stream << sep;
            });
            // log nth element
            std::for_each(b_1, b, [this](auto const& val){
                *this << val;
            });
        }
        m_stream << suf;
        doLogSpace();
    }
    return *this;
};

inline Stream& nospace(Stream& s) { return s.nospace(); }
inline Stream& space(Stream& s) { return s.space(); }

inline Stream& noquote(Stream& s) { return s.noquote(); }
inline Stream& quote(Stream& s) { return s.quote(); }

inline gt::log::Stream&
operator<<(gt::log::Stream& s, gt::log::Stream&(*f)(gt::log::Stream&))
{
    return f(s);
}

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

#endif // GT_LOGSTREAM_H
