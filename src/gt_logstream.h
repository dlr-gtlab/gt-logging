#ifndef GT_LOGSTREAM_H
#define GT_LOGSTREAM_H

#include <QDebug>

#include <QsLogDest.h>

namespace gt
{

namespace log
{

enum Verbosity
{
    Silent = 0,
    Medium = 5,
    Everything = 9
};

namespace detail
{

/// Maps all types to void
template<class... _Ts>
using void_t = void;


/// Uses SFINAE to check for a global operator<<(QDebug&)
template <typename T, class = void>
struct has_global_qdebug_shiftop
        : std::false_type{};

template <typename T>
struct has_global_qdebug_shiftop<T,
        void_t<decltype(::operator<<(std::declval<QDebug&>(),
                                     std::declval<T>()))>>
        : std::true_type{};


/// Uses SFINAE to check for a operator<<(T) in QDebug
template <typename T, class = void>
struct has_dedicated_qdebug_shiftop
        : std::false_type{};

template <typename T>
struct has_dedicated_qdebug_shiftop<T,
        void_t<decltype(std::declval<QDebug>().operator<<(std::declval<T>()))>>
        : std::true_type{};


/// Aliases for enables ifs
template <typename T>
using if_has_global_qdebug_shiftop =
        std::enable_if_t<
                has_global_qdebug_shiftop<std::decay_t<T>>::value, bool>;
template <typename T>
using if_has_no_global_qdebug_shiftop =
        std::enable_if_t<
                !has_global_qdebug_shiftop<std::decay_t<T>>::value, bool>;

template <typename T>
using if_has_dedicated_qdebug_shiftop =
        std::enable_if_t<
                has_dedicated_qdebug_shiftop<std::decay_t<T>>::value, bool>;
template <typename T>
using if_has_no_dedicated_qdebug_shiftop =
        std::enable_if_t<
                !has_dedicated_qdebug_shiftop<std::decay_t<T>>::value, bool>;

/// Alias for no pointer or integral type
template <typename T>
using if_not_integral_and_pointer =
        std::enable_if_t<
                !std::is_pointer<T>::value &&
                !std::is_integral<T>::value, bool>;

} // namsepace detail

class Stream : private QDebug
{
public:
    using QDebug::QDebug;

    inline Stream &quote() { return static_cast<Stream&>(QDebug::quote());}
    inline Stream &space() { return static_cast<Stream&>(QDebug::space());}
    inline Stream &noquote() { return static_cast<Stream&>(QDebug::noquote());}
    inline Stream &nospace() { return static_cast<Stream&>(QDebug::nospace());}

    Stream& verbose(int level = gt::log::Everything)
    {
        msgLevel = level;
        return *this;
    }

    Stream& medium()
    {
        return verbose(gt::log::Medium);
    }

    QSLOG_SHARED_OBJECT
    static bool mayLog(int v);
    bool mayLog() const { return mayLog(msgLevel); }

    // pod
    inline Stream &operator<<(std::nullptr_t) {return logMember("(nullptr)");}
    inline Stream &operator<<(void const* t) {return logMember(t);}
    inline Stream &operator<<(bool t) {return logMember(t);}

    // chars
    inline Stream &operator<<(char t) {return logMember(t);}
#ifdef Q_COMPILER_UNICODE_STRINGS
    inline Stream &operator<<(char16_t t) {return logMember(t);}
    inline Stream &operator<<(char32_t t) {return logMember(t);}
#endif
    inline Stream &operator<<(QChar t) {return logMember(t);}

    // ints
    inline Stream &operator<<(signed short t) {return logMember(t);}
    inline Stream &operator<<(unsigned short t) {return logMember(t);}
    inline Stream &operator<<(signed int t) {return logMember(t);}
    inline Stream &operator<<(unsigned int  t) {return logMember(t);}
    inline Stream &operator<<(signed long t) {return logMember(t);}
    inline Stream &operator<<(unsigned long t) {return logMember(t);}
    inline Stream &operator<<(qlonglong t) {return logMember(t);}
    inline Stream &operator<<(qulonglong t) {return logMember(t);}

    // floats
    inline Stream &operator<<(float t) {return logMember(t);}
    inline Stream &operator<<(double t) {return logMember(t);}

    // strings
    inline Stream &operator<<(const char* t) {return logMember(t);}
    inline Stream &operator<<(std::string const& t) {return logMember(t.c_str());}
    inline Stream &operator<<(const QString& t) {return logMember(t);}
    inline Stream &operator<<(QLatin1String t) {return logMember(t);}
    inline Stream &operator<<(const QByteArray& t) {return logMember(t);}
    inline Stream &operator<<(QTextStreamFunction t) {return t ? logMember(t) : *this;}

    // other
    inline Stream &operator<<(const QVariant& t) {return logOp(t);}

    // template based operators: QObject*
    template <typename T,
              std::enable_if_t<std::is_base_of<QObject, T>::value, bool> = true>
    inline Stream& operator<<(T const* t) {return logOp(t);}

    // template based operators: non QObject*
    template <typename T,
              std::enable_if_t<!std::is_base_of<QObject, T>::value, bool> = true>
    inline Stream& operator<<(T const* t) {return logMember(static_cast<void const*>(t));}

    // check for global qdebug support<<
    template<typename T,
             detail::if_not_integral_and_pointer<T> = true,
             detail::if_has_global_qdebug_shiftop<T> = true>
    inline Stream& operator<<(T const& t) {return logOp(t);}

    // check for dedicated qdebug operator<<
    template<typename T,
             detail::if_not_integral_and_pointer<T> = true,
             detail::if_has_no_global_qdebug_shiftop<T> = true,
             detail::if_has_dedicated_qdebug_shiftop<T> = true>
    inline Stream& operator<<(T const& t) {return logMember(t);}

private:

    using QDebug::operator=;

    template <typename T>
    Stream& logOp(T&& t)
    {
        if (mayLog(msgLevel))
        {
            *this = ::operator<<(static_cast<QDebug&>(*this),
                                 std::forward<T>(t));
        }
        return *this;
    }

    template <typename T>
    inline Stream & logMember(T&& t) {
        if (mayLog(msgLevel))
        {
            *this = QDebug::operator<<(t);
        }
        return *this;
    }

    int msgLevel{gt::log::Silent};
};

} // namespace log

} // namespace gt

#endif // GT_LOGSTREAM_H
