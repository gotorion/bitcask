#ifndef WRITER_H_
#define WRITER_H_
#include <span>

#include "type.h"

namespace bitcask::fs {
class SeqWriter {
 public:
  virtual Result<fsize_t> Append(std::span<const std::byte> contents) = 0;
};
}  // namespace bitcask::fs
#endif
