#include <gtest/gtest.h>

#include "absl/status/status.h"
#include "file.h"

using namespace bitcask::fs;

TEST(FSTest, OpenFileNoException) { ASSERT_NO_THROW(File("test_file")); }

TEST(FSTest, WriteFile) {
  File file("test_file");
  std::vector<std::byte> str(10, std::byte{0});
  std::span<std::byte> span{str.begin(), 10};
  EXPECT_TRUE(file.Write(0, span).ok());
}

TEST(FSTest, ReadFileFromStart) {
  File file("test_read_file");
  std::vector<std::byte> content{10, std::byte{1}};
  EXPECT_TRUE(file.Write(0, std::span<std::byte>{content.begin(), 10}).ok());

  // read it
  std::vector<std::byte> readContent(10, std::byte{0});
  EXPECT_TRUE(file.Read(0, std::span<std::byte>{readContent.begin(), 10}).ok());

  EXPECT_EQ(content, readContent);
}

TEST(FSTest, ReadFileFromOffset) {
  File file("test_read_from_offset_file");
  std::vector<std::byte> content{10, std::byte{1}};
  EXPECT_TRUE(file.Write(0, std::span<std::byte>{content.begin(), 10}).ok());

  std::vector<std::byte> contentSeg2{10, std::byte{2}};
  EXPECT_TRUE(file.Append(std::span<std::byte>{contentSeg2.begin(), 10}).ok());

  // read it
  std::vector<std::byte> readContent(10, std::byte{0});
  EXPECT_TRUE(file.Read(10, std::span<std::byte>{readContent.begin(), 10}).ok());

  EXPECT_EQ(contentSeg2, readContent);
}