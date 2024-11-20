#ifndef FILE_FACTORY_H_
#define FILE_FACTORY_H_
#include <filesystem>

#include "file.h"
namespace bitcask::fs {

class FileFactory {
 public:
 static Result<File> Open(std::filesystem::path&& path) noexcept;
};

};  // namespace bitcask::fs
#endif
