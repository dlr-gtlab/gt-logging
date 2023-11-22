# GTlab Logging

Just a test... don't merge...

Basic C++14 logging library based on [QsLog](https://github.com/victronenergy/QsLog).

## Differences to QsLog

 - Removed Qt depdendency. Logging Qt types is still supported, see below.
 - Logging of STL types.
 - Verbosity levels.
 - Custom log destinations using lambdas and functors.
 - Full CMake support.

## License
 
GTlab logging is licensed under the __3-Clause BSD License__, see LICENSES folder.

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

The library adds optionally support for Qt types. This must be enabled globally using the define `GT_LOG_USE_QT_BINDINGS` or by including `gt_logging/qt_bindings.h` instead.

### Additional STL Bindings:

The library adds dedicated logging operators for STL classes like vectors, lists, maps, unique_ptrs etc. 
These can be included individually e.g. using `gt_logging/vector.h`.
Alternatively they may be included automatically by defining `GT_LOG_USE_EXTENDED_STL_BINDINGS` globally or by using `gt_logging/stl_bindings.h` instead.

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

> Note: No output destination is registered by default!

Each destination must be registered using a unqiue id. Destination with the same id will be skipped.

### Stdout:

Will log everything to stdout or stderr (only if loglevel >= `ErrorLevel`).

```cpp
gt::log::Logger& logger = gt::log::Logger::instance();
logger.addDestination("default", gt::log::makeDebugOutputDestination());
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
    logger.addDestination("default", gt::log::makeDebugOutputDestination());
    return 0;
}();
```

## Removing an Output Destination:

Output destinations may be removed at any time. A destionation may only be removed by its unique id:

```cpp
gt::log::Logger& logger = gt::log::Logger::instance();
logger.removeDestination("my_destination");
```

One may check if a destination exists using `gt::log::Logger::hasDestination`.

    
