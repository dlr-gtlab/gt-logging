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

    inline Stream &operator<<(QChar t) {return logMember(t);}
    inline Stream &operator<<(bool t) {return logMember(t);}
    inline Stream &operator<<(char t) {return logMember(t);}
    inline Stream &operator<<(signed short t) {return logMember(t);}
    inline Stream &operator<<(unsigned short t) {return logMember(t);}
#ifdef Q_COMPILER_UNICODE_STRINGS
    inline Stream &operator<<(char16_t t) {return logMember(t);}
    inline Stream &operator<<(char32_t t) {return logMember(t);}
#endif
    inline Stream &operator<<(signed int t) {return logMember(t);}
    inline Stream &operator<<(unsigned int  t) {return logMember(t);}
    inline Stream &operator<<(signed long t) {return logMember(t);}
    inline Stream &operator<<(unsigned long t) {return logMember(t);}
    inline Stream &operator<<(qint64 t) {return logMember(t);}
    inline Stream &operator<<(quint64 t) {return logMember(t);}
    inline Stream &operator<<(float t) {return logMember(t);}
    inline Stream &operator<<(double t) {return logMember(t);}
    inline Stream &operator<<(const char* t) {return logMember(t);}
#if QT_STRINGVIEW_LEVEL < 2
    inline Stream &operator<<(const QString& t) {return logMember(t);}
    inline Stream &operator<<(const QStringRef& t) {return logMember(t);}
#endif
    inline Stream &operator<<(QStringView t) {return logMember(t);}
    inline Stream &operator<<(QLatin1String t) {return logMember(t);}
    inline Stream &operator<<(const QByteArray & t) {return logMember(t);}
    inline Stream &operator<<(const void * t) {return logMember(t);}
    inline Stream &operator<<(std::nullptr_t) {return logMember("(nullptr)");}
    inline Stream &operator<<(QTextStreamFunction t) {return logMember(t);}
    inline Stream &operator<<(const QVariant& t) {return logOp(t);}
    inline Stream &operator<<(const QModelIndex& t) {return logOp(t);}

    template <class T>
    inline Stream& operator<<(const QList<T> &t) {return logOp(t);}

    template <class T>
    inline Stream& operator<<(const QVector<T> &t) {return logOp(t);}

    template <typename T, typename Alloc>
    inline Stream& operator<<(const std::vector<T, Alloc> &t) {return logOp(t);}

    template <typename T, typename Alloc>
    inline Stream& operator<<(const std::list<T, Alloc> &t) {return logOp(t);}

    template <typename Key, typename T, typename Compare, typename Alloc>
    inline Stream& operator<<(const std::map<Key, T, Compare, Alloc>& t) {return logOp(t);}

    template <typename Key, typename T, typename Compare, typename Alloc>
    inline Stream& operator<<(const std::multimap<Key, T, Compare, Alloc>& t) {return logOp(t);}

    template <class Key, class T>
    inline Stream& operator<<(const QMap<Key, T> &t) {return logOp(t);}

    template <class Key, class T>
    inline Stream& operator<<(const QHash<Key, T> &t) {return logOp(t);}

    template <class T1, class T2>
    inline Stream& operator<<(const QPair<T1, T2> &t) {return logOp(t);}

    template <class T1, class T2>
    inline Stream& operator<<(const std::pair<T1, T2> &t) {return logOp(t);}

    template <typename T>
    inline Stream& operator<<(const QSet<T> &t) {return logOp(t);}

    template <typename T>
    inline Stream& operator<<(const QContiguousCache<T> &t) {return logOp(t);}

    template <typename T>
    inline Stream& operator<<(const QSharedPointer<T> &t) {return logOp(t);}

    template<typename T,
             typename A = typename std::enable_if<std::is_enum<T>::value, void>::type,
             typename B = typename std::enable_if<sizeof(T) <= sizeof(int), void>::type,
             typename C = typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value, void>::type,
             typename D = typename std::enable_if<QtPrivate::IsQEnumHelper<QFlags<T>>::Value, void>::type>
    inline Stream& operator<<(T t) {return logOp(std::move(t));}

    template<typename T>
    typename std::enable_if<QtPrivate::IsQEnumHelper<T>::Value, Stream&>::type
    operator<<(T value) {return logOp(std::move(value));}

    template<typename T>
    inline Stream& operator<<(const QFlags<T> &t) {return logOp(t);}
private:

    using QDebug::operator=;


    template <typename T>
    Stream& logOp(T&& t)
    {
        if (mayLog(msgLevel))
        {
            *this = ::operator<<(static_cast<QDebug&>(*this), std::forward<T>(t));
        }

        return *this;
    }

    template <typename T>
    inline Stream & logMember(T&& t) {
        if (mayLog(msgLevel))
        {
            *this =  QDebug::operator<<(t);
        }
        return *this;

    }

    int msgLevel{gt::log::Silent};
};
} // namespace log
} // namespace gt

#endif // GT_LOGSTREAM_H
