#ifndef BITCASK_H_
#define BITCASK_H_
namespace bitcask {

enum class OpenOpts {
  READ_WRITE,
  SYNC_ON_PUT,
};

template <typename K, typename V>
class BitCaskHandle {};

template <typename K, typename V>
class Bitcask {
  // public API
 public:
 public:
  Bitcask(Bitcask &&) = delete;

 private:
  BitCaskHandle<K, V> handle_;
};
}  // namespace bitcask
#endif
