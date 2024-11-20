#ifndef DB_H_
#define DB_H_

#include "file.h"
#include "reader.h"
#include "writer.h"

namespace bitcask::db {

struct Position {
  fs::fid_t fileId_;
  std::uint64_t offset;
};

class Store {
 public:
  Store(std::filesystem::path&& path, fs::fid_t latestFileId, std::uint64_t threshold);
  ~Store();
  using callback_t = std::function<void(Position)>;
  absl::StatusOr<std::size_t> ReadAt(Position const& pos, std::span<std::uint8_t> dst) noexcept;
  absl::Status Append(absl::Span<const std::uint8_t> src, callback_t callback) noexcept;

  absl::Status Sync() noexcept;

 private:
  const std::size_t threshold_{};
  std::filesystem::path path_{};
  // for latest file
  fs::fid_t activeFileId_{};
  std::unique_ptr<fs::SeqWriter> activeFileWriter_{};
  std::unique_ptr<fs::RandomReader> activeFileReader_{};
  // all readers
  std::unordered_map<fs::fid_t, std::unique_ptr<fs::RandomReader>> readers_{};
  std::mutex readersLock_{};
};

}  // namespace bitcask::db

#endif
