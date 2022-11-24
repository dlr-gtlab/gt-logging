// Copyright (c) 2013, Razvan Petru
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

#include "gt_logdestfile.h"

#include "gt_logging.h"

#include <iostream>

using namespace gt;

const size_t log::SizeRotationStrategy::MAX_BACKUP_COUNT = 10;

log::SizeRotationStrategy::~SizeRotationStrategy() = default;

log::SizeRotationStrategy::SizeRotationStrategy(FileSizeInBytes maxFileSize,
                                                BackupCount maxBackups)
    : m_maxFileSizeInBytes{maxFileSize}
    , m_maxBackupsCount{maxBackups}
{}

void
log::SizeRotationStrategy::setFileInfo(std::string const& filePath)
{
    m_fileName = filePath;
    // check temp
    std::ofstream temp(filePath, std::ofstream::ate | std::ofstream::binary);
    auto res = std::max(std::ios::pos_type{0}, temp.tellp());
    m_currentFileSizeInBytes = res;
}

void
log::SizeRotationStrategy::appendMessageSize(size_t size)
{
    m_currentFileSizeInBytes += size;
}

bool
log::SizeRotationStrategy::shouldRotate() const
{
    return m_currentFileSizeInBytes >= m_maxFileSizeInBytes;
}

// Algorithm assumes backups will be named filename.X, where 1 <= X <= mBackupsCount.
// All X's will be shifted up.
void
log::SizeRotationStrategy::rotate()
{
    // one file only
    if (!m_maxBackupsCount)
    {
        if (!removeFileAtPath(m_fileName))
        {
            std::cerr << "GtLogging: backup delete failed "
                      << m_fileName << '\n';
        }
        return;
    }

    const std::string logNamePattern{m_fileName + '.'};

     // 1. find the last existing backup that can be shifted up
     size_t lastExistingBackupIndex = 0;
     size_t fileNameLength = logNamePattern.size() +
                             m_maxBackupsCount / 10 + 1;

     std::string backupFileName;
     backupFileName.reserve(fileNameLength);

     for (size_t i = 1; i <= m_maxBackupsCount; ++i)
     {
         backupFileName = logNamePattern + std::to_string(i);
         if (fileExistsAtPath(backupFileName))
         {
             lastExistingBackupIndex = std::min(i, m_maxBackupsCount - 1);
         }
     }

     // 2. shift up
     std::string oldName;
     oldName.reserve(fileNameLength);
     std::string newName;
     newName.reserve(fileNameLength);

     for (size_t i = lastExistingBackupIndex; i >= 1; --i)
     {
         oldName = logNamePattern + std::to_string(i);
         newName = logNamePattern + std::to_string(i + 1);

         if (fileExistsAtPath(newName) && !removeFileAtPath(newName))
         {
             std::cerr << "GtLogging: could not remove backup file " << newName
                       << '\n';
             continue;
         }
         if (!renameFileFromTo(oldName, newName))
         {
             std::cerr << "GtLogging: could not rename backup file " << oldName
                       << " to " << newName << '\n';
         }
     }

     // 3. rename current log file
     newName = logNamePattern + std::to_string(1);
     if (fileExistsAtPath(newName) && !removeFileAtPath(newName))
     {
         std::cerr << "GtLogging: could not remove old log file "
                   << newName << '\n';
         return;
     }
     if (!renameFileFromTo(m_fileName, newName))
     {
         std::cerr << "GtLogging: could not rename log file " << m_fileName
                   << " to " << newName << '\n';
     }
}

void
log::SizeRotationStrategy::setMaximumSizeInBytes(uint64_t size)
{
    m_maxFileSizeInBytes = size;
}

void
log::SizeRotationStrategy::setBackupCount(size_t backups)
{
    m_maxBackupsCount = std::min(backups, MAX_BACKUP_COUNT);
}

bool
log::SizeRotationStrategy::removeFileAtPath(const std::string& path)
{
    return std::remove(path.c_str()) == 0;
}

bool
log::SizeRotationStrategy::fileExistsAtPath(const std::string& path)
{
    // try to open file to read
    std::ofstream file;
    file.open(path);
    return file.is_open();
}

bool
log::SizeRotationStrategy::renameFileFromTo(const std::string& from,
                                            const std::string& to)
{
    return std::rename(from.c_str(), to.c_str()) == 0;
}

log::FileDestination::~FileDestination() = default;

log::FileDestination::FileDestination(std::string filePath,
                                      RotationStrategyPtr rotationStrategy,
                                      Formatter formatter)

    : FormattedDestination{std::move(formatter)}
    , m_filePath{std::move(filePath)}
    , m_rotationStrategy{std::move(rotationStrategy)}
{
    assert(m_rotationStrategy);

    // open file
    m_fstream.open(m_filePath,
                        m_rotationStrategy->openMode());

    if (!m_fstream.is_open())
    {
        std::cerr << "GtLogging: could not open log file " << m_filePath << '\n';
    }

    // update file stats
    m_rotationStrategy->setFileInfo(m_filePath);
}

void
log::FileDestination::write(std::string const& message, Level /*level*/)
{
    std::string msg = message;

    if (m_rotationStrategy->shouldRotate())
    {
        // close and rotate
        m_fstream.close();
        m_rotationStrategy->rotate();

        // open file
        m_fstream.open(m_filePath,
                            m_rotationStrategy->openMode());

        if (!m_fstream.is_open())
        {
            std::cerr << "GtLogging: could not reopen log file "
                      << m_filePath << '\n';
            return;
        }
        // update file stats
        m_rotationStrategy->setFileInfo(m_filePath);
    }

    m_rotationStrategy->appendMessageSize(msg.size());

    m_fstream << std::move(msg);
    m_fstream.flush();
}

bool
log::FileDestination::isValid() const
{
    return m_fstream.is_open();
}
