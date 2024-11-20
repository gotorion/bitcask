#ifndef TYPE_H_
#define TYPE_H_
#include <absl/status/status.h>
#include <absl/status/statusor.h>

#include <cstdint>

namespace bitcask {

template <typename SuccessType>
using Result = absl::StatusOr<SuccessType>;

// max size of file
constexpr std::uint64_t MAX_FILE_SIZE = INT64_MAX;
using fsize_t = long;

};  // namespace bitcask
#endif
