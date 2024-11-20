#include "file.h"

#include "error.h"
namespace bitcask::fs {

File::File(std::filesystem::path &&path) : path_(std::move(path)) {
  fs_.open(path_, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
}

File::~File() {
  if (fs_.is_open()) {
    fs_.close();
  }
}

Result<fsize_t> File::Write(fsize_t offset, std::span<const std::byte> contents) {
  if (!fs_.is_open()) {
    return absl::FailedPreconditionError(kFileStatusError);
  }
  fs_.seekp(offset);
  if (fs_.fail()) {
    return absl::InternalError("Seek file failed");
  }
  fs_.write(reinterpret_cast<const char *>(contents.data()), contents.size());
  if (fs_.fail()) {
    return absl::InternalError("Write file failed");
  }
  return contents.size();
}

Result<fsize_t> File::Append(std::span<std::byte const> contents) {
  if (!fs_.is_open()) {
    return absl::FailedPreconditionError(kFileStatusError);
  }
  fs_.write(reinterpret_cast<char const *>(contents.data()), contents.size());
  if (fs_.fail()) {
    return absl::InternalError("Write file failed");
  }
  return contents.size();
}

absl::Status File::Read(fsize_t offset, std::span<std::byte> dst) {
  if (!fs_.is_open()) {
    return absl::FailedPreconditionError(kFileStatusError);
  }
  fs_.seekg(offset);
  if (fs_.fail()) {
    return absl::InternalError("Seek failed");
  }
  fs_.read(reinterpret_cast<char *>(dst.data()), dst.size());
  if (fs_.fail() && !fs_.eof()) {
    return absl::InternalError("Read failed");
  }
  return absl::OkStatus();
}

}  // namespace bitcask::fs
