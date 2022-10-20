#ifndef GT_LOGSTREAM_H
#define GT_LOGSTREAM_H

#include <QDebug>

#include <QsLogDest.h>

namespace gt
{

enum Verbosity
{
    SILENT,
    MEDIUM,
    EVERYTHING
};

class LogStream : private QDebug
{
public:
    using QDebug::QDebug;

    inline LogStream &quote() { return static_cast<LogStream&>(QDebug::quote());}
    inline LogStream &noquote() { return static_cast<LogStream&>(QDebug::noquote());}
    inline LogStream &nospace() { return static_cast<LogStream&>(QDebug::nospace());}

    LogStream& verbose(gt::Verbosity level = gt::EVERYTHING)
    {
        msgLevel = level;
        return *this;
    }

    QSLOG_SHARED_OBJECT
    static bool mayLog(gt::Verbosity v);

    inline LogStream &operator<<(QChar t) {return logMember(t);}
    inline LogStream &operator<<(bool t) {return logMember(t);}
    inline LogStream &operator<<(char t) {return logMember(t);}
    inline LogStream &operator<<(signed short t) {return logMember(t);}
    inline LogStream &operator<<(unsigned short t) {return logMember(t);}
#ifdef Q_COMPILER_UNICODE_STRINGS
    inline LogStream &operator<<(char16_t t) {return logMember(t);}
    inline LogStream &operator<<(char32_t t) {return logMember(t);}
#endif
    inline LogStream &operator<<(signed int t) {return logMember(t);}
    inline LogStream &operator<<(unsigned int  t) {return logMember(t);}
    inline LogStream &operator<<(signed long t) {return logMember(t);}
    inline LogStream &operator<<(unsigned long t) {return logMember(t);}
    inline LogStream &operator<<(qint64 t) {return logMember(t);}
    inline LogStream &operator<<(quint64 t) {return logMember(t);}
    inline LogStream &operator<<(float t) {return logMember(t);}
    inline LogStream &operator<<(double t) {return logMember(t);}
    inline LogStream &operator<<(const char* t) {return logMember(t);}
#if QT_STRINGVIEW_LEVEL < 2
    inline LogStream &operator<<(const QString& t) {return logMember(t);}
    inline LogStream &operator<<(const QStringRef& t) {return logMember(t);}
#endif
    inline LogStream &operator<<(QStringView t) {return logMember(t);}
    inline LogStream &operator<<(QLatin1String t) {return logMember(t);}
    inline LogStream &operator<<(const QByteArray & t) {return logMember(t);}
    inline LogStream &operator<<(const void * t) {return logMember(t);}
    inline LogStream &operator<<(std::nullptr_t) {return logMember("(nullptr)");}
    inline LogStream &operator<<(QTextStreamFunction t) {return logMember(t);}
    inline LogStream &operator<<(const QVariant& t) {return logOp(t);}
    inline LogStream &operator<<(const QModelIndex& t) {return logOp(t);}

    template <class T>
    inline LogStream& operator<<(const QList<T> &t) {return logOp(t);}

    template <class T>
    inline LogStream& operator<<(const QVector<T> &t) {return logOp(t);}

    template <typename T, typename Alloc>
    inline LogStream& operator<<(const std::vector<T, Alloc> &t) {return logOp(t);}

    template <typename T, typename Alloc>
    inline LogStream& operator<<(const std::list<T, Alloc> &t) {return logOp(t);}

    template <typename Key, typename T, typename Compare, typename Alloc>
    inline LogStream& operator<<(const std::map<Key, T, Compare, Alloc>& t) {return logOp(t);}

    template <typename Key, typename T, typename Compare, typename Alloc>
    inline LogStream& operator<<(const std::multimap<Key, T, Compare, Alloc>& t) {return logOp(t);}

    template <class Key, class T>
    inline LogStream& operator<<(const QMap<Key, T> &t) {return logOp(t);}

    template <class Key, class T>
    inline LogStream& operator<<(const QHash<Key, T> &t) {return logOp(t);}

    template <class T1, class T2>
    inline LogStream& operator<<(const QPair<T1, T2> &t) {return logOp(t);}

    template <class T1, class T2>
    inline LogStream& operator<<(const std::pair<T1, T2> &t) {return logOp(t);}

    template <typename T>
    inline LogStream& operator<<(const QSet<T> &t) {return logOp(t);}

    template <typename T>
    inline LogStream& operator<<(const QContiguousCache<T> &t) {return logOp(t);}

    template <typename T>
    inline LogStream& operator<<(const QSharedPointer<T> &t) {return logOp(t);}

    template<typename T,
             typename A = typename std::enable_if<std::is_enum<T>::value, void>::type,
             typename B = typename std::enable_if<sizeof(T) <= sizeof(int), void>::type,
             typename C = typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value, void>::type,
             typename D = typename std::enable_if<QtPrivate::IsQEnumHelper<QFlags<T>>::Value, void>::type>
    inline LogStream& operator<<(T t) {return logOp(std::move(t));}

    template<typename T>
    typename std::enable_if<QtPrivate::IsQEnumHelper<T>::Value, LogStream&>::type
    operator<<(T value) {return logOp(std::move(value));}

    template<typename T>
    inline LogStream& operator<<(const QFlags<T> &t) {return logOp(t);}
private:

    using QDebug::operator=;


    template <typename T>
    LogStream& logOp(T&& t)
    {
        if (mayLog(msgLevel))
        {
            *this = ::operator<<(static_cast<QDebug&>(*this), std::forward<T>(t));
        }

        return *this;
    }

    template <typename T>
    inline LogStream & logMember(T&& t) {
        if (mayLog(msgLevel))
        {
            *this =  QDebug::operator<<(t);
        }
        return *this;

    }

    gt::Verbosity msgLevel{gt::SILENT};
};
}

#endif // GT_LOGSTREAM_H
