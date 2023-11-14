// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_QT_BINDINGS_H
#define GT_LOGGING_QT_BINDINGS_H

#include "gt_logging.h"
#include "gt_logging/stl_bindings.h"

#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include <QVector>
#include <QList>

namespace gt
{

namespace log
{

namespace detail
{

/// Maps all type to void
template <typename... Ts>
using void_t = void;

/// Uses SFINAE to check for a QDebug operator<<(T)
template <typename T, class = void>
struct has_qdebug_shiftop
            : std::false_type{};

template <typename T, class = void>
struct has_global_qdebug_shiftop
        : std::false_type{};

template <typename T>
struct has_qdebug_shiftop<T,
        void_t<decltype(std::declval<QDebug&>().operator<<(std::declval<T const&>()))>>
            : std::true_type{};

template <typename T>
struct has_global_qdebug_shiftop<T,
        void_t<decltype(::operator<<(std::declval<QDebug&>(),
                                     std::declval<T>()))>>
        : std::true_type{};

template <typename T>
using if_has_qdebug_shiftop =
        std::enable_if_t<
                has_qdebug_shiftop<std::decay_t<T>>::value, bool>;

template <typename T>
using if_has_global_qdebug_shiftop =
        std::enable_if_t<
                has_global_qdebug_shiftop<std::decay_t<T>>::value, bool>;

template <typename T>
using if_has_no_global_qdebug_shiftop =
        std::enable_if_t<
                !has_global_qdebug_shiftop<std::decay_t<T>>::value, bool>;

//! Alias for no pointer nor integral type
template <typename T>
using if_not_integral_and_pointer =
        std::enable_if_t<
                !std::is_pointer<T>::value &&
                !std::is_integral<T>::value, bool>;

template <typename T>
using if_base_of_qobject =
        std::enable_if_t<std::is_base_of<QObject, T>::value, bool>;

template <typename T>
using if_not_base_of_qobject =
        std::enable_if_t<!std::is_base_of<QObject, T>::value, bool>;

//! Logging helper for Qt types
template <typename T>
inline Stream& doLogQt(Stream& s, T const& t)
{
    if (s.mayLog())
    {
        // setup debug stream
        QString data;
        QDebug debug(&data);
        QDebug& d = debug.nospace();
        // apply flags
        d = (s.mayLogQuote()) ? d.quote() : d.noquote();
        // log t
        d << t;
        // append to stream
        const QByteArray buffer = data.toUtf8();
        return s << buffer.data();
    }
    return s;
}

} // namespace detail

/** Stream operator<< **/
inline Stream& operator<<(Stream& s, QChar t) { return s.doLogQuoted(char(t.unicode())); }
inline Stream& operator<<(Stream& s, const QByteArray& t) { return s.doLogQuoted(t.data()); }
inline Stream& operator<<(Stream& s, const QLatin1String& t) { return s.doLogQuoted(t.data()); }
inline Stream& operator<<(Stream& s, const QString& t) { return s << t.toUtf8(); }
inline Stream& operator<<(Stream& s, QTextStreamFunction t) { return t ? detail::doLogQt(s, t) : s; }
inline Stream& operator<<(Stream& s, const QVariant& t) { return detail::doLogQt(s, t); }

// template based operators: QObject*
template <typename T, detail::if_base_of_qobject<T> = true>
inline Stream& operator<<(Stream& s, T const* t) { return detail::doLogQt(s, t); }

// template based operators: non QObject*
template <typename T, detail::if_not_base_of_qobject<T> = true>
inline Stream& operator<<(Stream& s, T const* t) { return s << static_cast<void const*>(t); }

// template based operators: non QObject*
template <typename T>
inline Stream& operator<<(Stream& s, QVector<T> const& t) { return s.doLogIter(t.begin(), t.end()); }

// template based operators: non QObject*
template <typename T>
inline Stream& operator<<(Stream& s, QList<T> const& t) { return s.doLogIter(t.begin(), t.end()); }

// check for ::operator<<(QDebug, T)
template<typename T,
         detail::if_not_integral_and_pointer<T> = true,
         detail::if_has_global_qdebug_shiftop<T> = true>
inline Stream& operator<<(Stream& s, T const& t) { return detail::doLogQt(s, t); }

// check for QDebug::operator<<(T)
template<typename T,
         detail::if_not_integral_and_pointer<T> = true,
         detail::if_has_no_global_qdebug_shiftop<T> = true,
         detail::if_has_qdebug_shiftop<T> = true>
inline Stream& operator<<(Stream& s, T const& t) { return detail::doLogQt(s, t); }

} // namespace log

} // namespace gt

#endif // GT_LOGGING_QT_BINDINGS_H
