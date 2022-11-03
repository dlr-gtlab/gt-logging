# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] 
### Added
### Fixed
### Changed
### Deprecated
### Removed
### Security 

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
