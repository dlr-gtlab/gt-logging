# GTlab Logging

Basic C++14 logging library. 

## Logging Macros:

To log a message use the following macros. They correspond to the six loglevels:

```cpp
gtTrace()   << "..."; // TraceLevel 
gtDebug()   << "..."; // DebugLevel 
gtInfo()    << "..."; // InfoLevel
gtWarning() << "..."; // WarningLevel 
gtError()   << "..."; // ErrorLevel 
gtFatal()   << "..."; // FatalLevel
```

Additionally there are six macros, which may be used to set a custom logging id. The id will be logged together with the content:

```cpp
gtTraceId("...")   << "..."; // TraceLevel 
gtDebugId("...")   << "..."; // DebugLevel 
gtInfoId("...")    << "..."; // InfoLevel
gtWarningId("...") << "..."; // WarningLevel 
gtErrorId("...")   << "..."; // ErrorLevel 
gtFatalId("...")   << "..."; // FatalLevel
```

In general the output signature looks something like this:

```cpp
// out: ([ID]) [TIME] <content>
```

## Global Config:

### Logging ID:

One may set a global logging by defining the macro `GT_MODULE_ID` globally.

### Qt Support:

The library adds optionally support for Qt types. This must be enabled globally using the define `GT_LOG_USE_QT_BINDINGS`. Alternatively one may include `gt_logging_qt_bindings.h` together with `gt_logging.h`.

## Local Config:

The logging behaviour may be altered for each file individually:

### Log Line Numbers:

One may log line numbers by defining `GT_LOG_LINE_NUMBERS` before including `gt_logging.h`. This may be set globally as well.

```cpp
// out: ([ID]) [TIME] <file>@<lineno>: <content>
```

### Log No Space:

By default this library will log every argument with an trailing space. This can be disabled by defining `GT_LOG_NOSPACE` before including `gt_logging.h`. This may be set globally as well.

### Log Quotes:

This library can log Qt-String types with quotes. This can be enabled by defining `GT_LOG_QUOTE` before including `gt_logging.h`. This may be set globally as well.

## Adding an Output Destination:

> Note: No output destination is registered by default

Optionally one may use a unique id when adding a destination. Destination with the same id will be skipped.

### Stdout:

Will log everything to stdout or stderr (only if loglevel >= `ErrorLevel`).

```cpp
gt::log::Logger& logger = gt::log::Logger::instance();
logger.addDestination("default", gt::log::makeDebugDestination());
```
	
Output:

```cpp
gtWarning() << "Test";
// out: WARN ([ID]) [10:02:33] Test;
```

### File:

Will log message to the filepath specified. A rotation strategy can be specified.


### Functor:

Can be used to forward the output to a function-like object (Functor). The logging behaviour is up to the developer:

```cpp
static void my_functor(std::string const& message, gt::log::Level level);

// ...

gt::log::Logger& logger = gt::log::Logger::instance();
logger.addDestination("default", gt::log::makeFunctorDestination(my_functor));
```

## Adding an Output Destination at startup:

One may register an logging destination at startup to not miss out on any logging output by invoking a lambda:

```cpp
static auto init_logger_once = [](){
    gt::log::Logger& logger = gt::log::Logger::instance();
    logger.addDestination("default", gt::log::makeDebugDestination());
    return 0;
}();
```

## Removing an Output Destination:

Output destinations may be removed at any time. An unnamed destionation may only be removed using its pointer:

```cpp
gt::log::DestinationPtr my_destination;
gt::log::Logger& logger = gt::log::Logger::instance();
logger.removeDestination(my_destination);
```

Alternatively one may remove a destination using the unique id used, when registering a destination:

```cpp
gt::log::DestinationPtr my_destination;
gt::log::Logger& logger = gt::log::Logger::instance();
logger.removeDestination(my_destination);
```

One may check if a destination exists using `gt::log::Logger::hasDestination` or `gt::log::Logger::hasDestinationOfType`

    