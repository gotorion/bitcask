#ifndef ENTRY_H_
#define ENTRY_H_

#include <cstdint>

namespace bitcask {

class Entry {
 public:
 private:
  std::int32_t crc32_{};
};

}  // namespace bitcask
#endif
