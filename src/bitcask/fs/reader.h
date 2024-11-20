#ifndef READER_H_
#define READER_H_
#include "type.h"

namespace bitcask::fs {
class SeqReader {
 public:
  virtual Result<fsize_t> Read(std::span<std::byte> dst) noexcept = 0;
};

class RandomReader {
 public:
  virtual Result<fsize_t> Read(fsize_t offset, std::span<std::byte> dst) noexcept = 0;
};

}  // namespace bitcask::fs
#endif
