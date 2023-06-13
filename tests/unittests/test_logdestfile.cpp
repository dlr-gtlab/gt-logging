/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 11.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include <gtest/gtest.h>
#include "gt_logging.h"
#include "test_helper.h"

#include <QFileInfo>

class LogFileDest :  public testing::Test
{
public:

    LogFileDest() : logger(gt::log::Logger::instance())
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
        logger.removeDestination(noRotId);
        logger.removeDestination(rotId);
        ASSERT_FALSE(logger.hasDestination(rotId));
        ASSERT_FALSE(logger.hasDestination(noRotId));
    }

    gt::log::Logger& logger;
    std::string noRotId = "file_no_rot";
    std::string rotId = "file_rot";
};

TEST_F(LogFileDest, noRotation)
{
    // file path should not exist
    std::string filePath = TestHelper::instance().tempPath().toStdString() +
                           "no_rot_file.txt";
    QString qFilePath = QString::fromStdString(filePath);
    ASSERT_FALSE(QFileInfo::exists(qFilePath));

    // destination will create file
    auto dest = gt::log::makeFileDestination(filePath);
    EXPECT_TRUE(QFileInfo::exists(qFilePath));

    // register destination
    ASSERT_FALSE(logger.hasDestination(noRotId));
    EXPECT_TRUE(logger.addDestination(noRotId, std::move(dest)));
    EXPECT_TRUE(logger.hasDestination(noRotId));

    // log messages
    char boring[] = "boring";
    gtWarning() << 'm' << 'y' << boring << "warning!";
    gtError() << "my" << std::string{"not-so-fancy"} << "error no" << 42;

    // open file and read all data
    QFile file{qFilePath};
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));
    auto data = file.readAll();

    // check warning message:
    // level
    EXPECT_TRUE(data.contains(gt::log::levelToString(gt::log::WarnLevel).c_str()));
    // message
    EXPECT_TRUE(data.contains("m y boring warning!"));

    // check error message:
    // level
    EXPECT_TRUE(data.contains(gt::log::levelToString(gt::log::ErrorLevel).c_str()));
    // message
    EXPECT_TRUE(data.contains("my not-so-fancy error no 42"));
}

TEST_F(LogFileDest, withRotation)
{
    // size of new line character
    constexpr uint newLineSize = 1
#ifdef WIN32
        + 1 // for CR LF on windows
#endif
        ;
    constexpr uint digitSize = 2; // two digits -> two characters
    constexpr uint fileSize = (digitSize + newLineSize) * 20;
    constexpr uint backups = 2;
    std::string filePath = TestHelper::instance().tempPath().toStdString() +
                           "rot_file.txt";

    // convert path to QString
    QString qFilePath = QString::fromStdString(filePath);
    // file path of "next" file
    QString qFilePath1 = qFilePath + '.' + QString::number(1);

    // file paths should not exist
    ASSERT_FALSE(QFileInfo::exists(qFilePath));
    ASSERT_FALSE(QFileInfo::exists(qFilePath1));

    // destination will create first file
    auto rot = gt::log::makeSizeRotationStrategy(gt::log::FileSizeInBytes{fileSize},
                                                 gt::log::BackupCount{backups});
    auto dest = gt::log::makeFileDestination(filePath, rot);
    dest->setFormatter(gt::log::Formatter{
        [](std::string const& msg, auto, auto){
            return msg;
        }
    });
    EXPECT_TRUE(QFileInfo::exists(qFilePath));
    EXPECT_FALSE(QFileInfo::exists(qFilePath1));

    // register destination
    ASSERT_FALSE(logger.hasDestination(rotId));
    EXPECT_TRUE(logger.addDestination(rotId, std::move(dest)));
    EXPECT_TRUE(logger.hasDestination(rotId));

    QFileInfo fileInfo{qFilePath};

    // write data until we have reached max file size
    // starting at 10 here, so that each entry has 2 digits thus 2 characters
    for (uint i = 10, lastSize = 0; (lastSize < fileSize);
         ++i, fileInfo.refresh(), lastSize = fileInfo.size())
    {
        gtDebug().nospace() << i;
    }

    // first file is "full" now
    fileInfo.refresh();
    EXPECT_GE(static_cast<uint64_t>(fileInfo.size()), fileSize);

    // new file should have been written
    QByteArray msg = "This should trigger the file creation!";
    gtDebug() << msg.data();
    EXPECT_TRUE(QFileInfo::exists(qFilePath));  // <- this is the new file
    EXPECT_TRUE(QFileInfo::exists(qFilePath1)); // <- this is the old file

    // check content of new file
    QFile file{qFilePath};
    file.open(QIODevice::ReadOnly);
    EXPECT_TRUE(file.readAll().contains(msg));

}
