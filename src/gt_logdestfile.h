// SPDX-FileCopyrightText: 2013, Razvan Petru
// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
//
// SPDX-License-Identifier: BSD-3-Clause
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef GT_LOGDESTFILE_H
#define GT_LOGDESTFILE_H

#include "gt_logdest.h"

#include <stdint.h>
#include <fstream>

namespace gt
{
namespace log
{

//! Rotation Option
enum RotationOption
{
    DisableLogRotation = 0,
    EnableLogRotation  = 1
};

//! Typesafe wrapper for file size in bytes
struct FileSizeInBytes
{
    constexpr FileSizeInBytes() : value(0) {}
    constexpr explicit FileSizeInBytes(uint64_t size_) : value(size_) {}

    constexpr operator uint64_t() noexcept { return value; }

    uint64_t value;
};

//! Typesafe wrapper for backups to keep
struct BackupCount
{
    constexpr BackupCount() : value(0) {}
    constexpr explicit BackupCount(size_t count_) : value(count_) {}

    constexpr operator size_t() noexcept { return value; }

    size_t value;
};

struct RotationStrategy
{
    virtual ~RotationStrategy() = default;

    virtual void setFileInfo(std::string const& filePath) = 0;
    virtual void appendMessageSize(size_t size) = 0;
    virtual bool shouldRotate() const = 0;
    virtual void rotate() = 0;
    virtual std::ios_base::openmode openMode() const = 0;
};

// Never rotates file, overwrites existing file.
struct NullRotationStrategy : public RotationStrategy
{
    void setFileInfo(std::string const&) override {}
    void appendMessageSize(size_t) override {}
    bool shouldRotate() const override { return false; }
    void rotate() override {}

    std::ios_base::openmode openMode() const override
    {
        return std::ios::out | std::ios::trunc;
    }
};

//! Rotates after a size is reached, keeps a number of <= MAX_BACKUP_COUNT
//! backups, appends to existing file.
class SizeRotationStrategy : public RotationStrategy
{
public:
    static const size_t MAX_BACKUP_COUNT;

    GT_LOGGING_EXPORT SizeRotationStrategy(FileSizeInBytes maxFileSize = {},
                         BackupCount maxBackups = {});
    GT_LOGGING_EXPORT ~SizeRotationStrategy();

    GT_LOGGING_EXPORT
    void setFileInfo(std::string const& filePath) override;

    GT_LOGGING_EXPORT
    void appendMessageSize(size_t size) override;

    GT_LOGGING_EXPORT
    bool shouldRotate() const override;

    GT_LOGGING_EXPORT
    void rotate() override;

    std::ios_base::openmode openMode() const override
    {
        return std::ios::out | std::ios::app;
    }

    GT_LOGGING_EXPORT
    void setMaximumSizeInBytes(uint64_t size);

    GT_LOGGING_EXPORT
    void setBackupCount(size_t backups);

protected:
    // can be overridden for testing
    static bool removeFileAtPath(const std::string&path);
    static bool fileExistsAtPath(const std::string& path);
    static bool renameFileFromTo(const std::string& from,
                                 const std::string& to);

private:

    std::string m_fileName;
    uint64_t m_currentFileSizeInBytes{0};
    uint64_t m_maxFileSizeInBytes{0};
    size_t m_maxBackupsCount{0};
};

using RotationStrategyPtr = std::shared_ptr<RotationStrategy>;

inline RotationStrategyPtr
makeSizeRotationStrategy(FileSizeInBytes maxFileSize = {},
                         BackupCount maxBackups = {})
{
    return std::make_shared<SizeRotationStrategy>(maxFileSize, maxBackups);
}

//! File destination sink
class FileDestination : public FormattedDestination
{
public:

    //! ctor
    GT_LOGGING_EXPORT FileDestination(std::string filePath,
                                      RotationStrategyPtr rotationStrategy,
                                      Formatter formatter = {});

    //! dtor
    GT_LOGGING_EXPORT ~FileDestination();

    //! Logs formatted text
    GT_LOGGING_EXPORT
    void write(std::string const& message, Level level) override;

    //! is valid
    GT_LOGGING_EXPORT
    bool isValid() const override;


private:

    std::ofstream m_fstream;
    std::string m_filePath;
    RotationStrategyPtr m_rotationStrategy;
};

inline std::unique_ptr<FileDestination>
makeFileDestination(std::string filePath, Formatter formatter = {})
{
    return std::make_unique<FileDestination>(
                std::move(filePath), std::make_shared<NullRotationStrategy>(),
                std::move(formatter));
}

inline std::unique_ptr<FileDestination>
makeFileDestination(std::string filePath,
                    std::shared_ptr<RotationStrategy> rotation,
                    Formatter formatter = {})
{
    if (!rotation)
    {
        rotation = std::make_unique<NullRotationStrategy>();
    }
    return std::make_unique<FileDestination>(
                std::move(filePath), std::move(rotation), std::move(formatter));
}

} // namespace log

} // namespace gt

#endif // GT_LOGDESTFILE_H
