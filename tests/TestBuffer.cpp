
#include <var-buffer/VarBuffer.hpp>
#include <bserialize/BSerialize.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <cstring>

namespace varbuff = CPM_VAR_BUFFER_NS;

namespace {

TEST(VarBuffer, BasicSerialization)
{
  varbuff::VarBuffer vb;

  // Proceed close to 1024 limit.
  const size_t emptySize = 1020;
  char empty[emptySize];
  std::memset(empty, 0, emptySize);

  vb.writeBytes(empty, emptySize);

  // After this write, we are at 1024.
  uint32_t v1 = 3;
  double v2 = 5.483;
  float v3 = 8293.09;
  int32_t v4 = -323;
  uint16_t v5 = 8924;
  std::string v6 = "This is a test string.";
  uint8_t v7 = 98;

  vb.write(v1);

  // Continue writing.
  vb.write(v2);
  vb.write(v3);
  vb.write(v4);
  vb.writeNullTermString("Blah!");
  vb.write(v5);
  vb.write(v6);
  vb.write(v7);

  // Now take the buffer and stuff it into BSerialize
  CPM_BSERIALIZE_NS::BSerialize sin(vb.getBuffer(), vb.getBufferSize());  
  sin.readBytes(emptySize);
  EXPECT_EQ(v1, sin.read<uint32_t>());
  EXPECT_EQ(v2, sin.read<double>());
  EXPECT_EQ(v3, sin.read<float>());
  EXPECT_EQ(v4, sin.read<int32_t>());
  EXPECT_EQ(std::string("Blah!"), std::string(sin.readNullTermString()));
  EXPECT_EQ(v5, sin.read<uint16_t>());
  EXPECT_EQ(v6, sin.read<std::string>());
  EXPECT_EQ(v7, sin.read<uint8_t>());
}

}

