# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.1.1] - 2023-05-05 
### Added
 - Added support for the cmake build system
 - Added support to build with conan
 - Added support to create a conan recipe
### Fixed
- Fixed misleading output for unique and shared ptr - #42

### Changed
### Deprecated
### Removed
### Security 

## [4.1.0] - 2023-02-02 
### Added
- Added support for `std::tuple` types

## [4.0.0] - 2022-11-24 
### Changed
- __Highlight__: GtLogging does not require Qt anymore. Optional Qt bindings may still be renabled using `GT_LOG_USE_QT_BINDINGS` - #33
- `DestinationPtr` is now a `std::unique_ptr`. Destionations must be added/removed using a destionation id - #34
- Changed `Destination::write` function signature. Now accepts another parameter containing details of the logged message. The message is now unformatted. - #34
- Functor Destination to accepts all function-like objects (using `std::function`) - #33

### Added
- a `format` function and a `Fromatter` helper class for indivdual formatting - #34

## [3.0.0 ] - 2022-11-03
### Changed
- Renamed QsLogging namespace to gt::log

## [2.2.0] - 2022-11-02
### Fixed
- Ambiguous overloads for `operator<<`

## [2.1.0] - 2022-10-25
### Added
- The logging system can now log an identification string, which may be specified by `GT_MODULE_ID` globally at compile time - #28
- Dedicated macros for logging a custom id - #28
- Handling of verbose logging messages - #29

### Fixed
- A reference to the `QDebug` object will now be returned when logging line numbers 

### Changed
- using  `noquote` by default - #25
- Renamed the `QS_LOG_LINE_NUMBERS` macro to `GT_LOG_LINE_NUMBERS` for logging file and line numbers

## [2.0.0] - 2022-07-15
### Changed
- Switched to Qt 5.15 as a foundation for further development
- Switched to Visual Studio 2019 as a foundation for further development

## [1.0.1] - 2021-03-03
### Fixed
- R-Path link for linux - # 38


## [1.0.0] - 2021-03-02
### Added
- CHANGELOG as initial commit
