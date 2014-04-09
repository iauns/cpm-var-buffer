#ifndef IAUNS_VARBUFFER_HPP
#define IAUNS_VARBUFFER_HPP

#include <cstdint>
#include <bserialize/BSerialize.hpp>

namespace CPM_VAR_BUFFER_NS {

/// This class is only for writing to a variable sized buffer. It is a thin
/// layer over the BSerialize code. To read data from a buffer, simply wrap
/// the buffer using BSerialize. There is no need to automatically extend
/// the buffer size when reading.
class VarBuffer
{
public:
  /// Initialize an empty buffer.
  VarBuffer();

  /// Initialize with pre-existing buffer. The buffers contents will be
  /// copied into a temporary buffer. Does not take ownership of 'buffer'.
  VarBuffer(char* buffer, int bufferSize);

  virtual ~VarBuffer();
  
  /// \todo Add flag which will swap bytes when reading out of or into buffer.

  /// Writes \p numBytes of \p bytes.
  void writeBytes(const char* bytes, size_t numBytes);

  /// Writes a null terminated string.
  void writeNullTermString(const char* str);

  template <typename T>
  void write(const T& val)
  {
    // Attempt to write the value to BSerialize. If a runtime exception is
    // caught, then we need to resize the buffer.
    while (mSerializer->write(val) == false)
    {
      resize();
    }
  }

  void clear();

  char* getBuffer()   {return mBuffer;}
  int getBufferSize() {return mSerializer->getOffset();}

private:

  void resize();

  static bool serializeFloat(char* msg, int msgLen, int* offset_out, float in);

  static bool serializeUInt16(char* msg, int msgLen, int* offset_out, uint16_t in);
  static uint16_t deserializeUInt16(const char* msg, int msgLen, int* offset_out);

  char*   mBuffer;      ///< Pointer to our allocated buffer.
  int     mBufferSize;  ///< Absolute size of mBuffer in bytes.

  std::unique_ptr<CPM_BSERIALIZE_NS::BSerialize> mSerializer;
};

} // namespace CPM_VAR_BUFFER_NS

#endif 
