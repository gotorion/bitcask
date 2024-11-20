#ifndef FILE_H_
#define FILE_H_
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <shared_mutex>

#include "reader.h"
#include "type.h"

namespace bitcask::fs {

using fid_t = std::uint32_t;

class File {
 public:
  explicit File(std::filesystem::path&& path);
  ~File();

  File(File&&) = delete;
  File& operator=(File&&) = delete;
  friend class SeqWriter;
  friend class SeqReader;
  friend class RandomReader;

 public:
  fid_t id() const;
  void id(fid_t);

  // return the size successfully wrote in
  Result<fsize_t> Write(fsize_t offset, std::span<const std::byte> contents);
  Result<fsize_t> Append(std::span<std::byte const> contents);
  // read contents to dst from offset
  absl::Status Read(fsize_t offset, std::span<std::byte> dst);

 private:
  mutable std::shared_mutex lock_{};
  std::filesystem::path path_{};
  std::fstream fs_{};
  fid_t id_{};
};

}  // namespace bitcask::fs
#endif
