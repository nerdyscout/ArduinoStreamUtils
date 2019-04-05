// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "Stream.hpp"
#include "StreamSpy.hpp"
#include "StreamStub.hpp"

#include "StreamUtils/BufferedStream.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>

using namespace StreamUtils;

TEST_CASE("BufferedStream") {
  StreamStub stub;
  StreamSpy spy{stub};
  StaticBufferedStream<4> bufferedStream{spy};
  Stream& stream = bufferedStream;

  SUBCASE("available()") {
    stub.setup("ABCDEFGH");

    SUBCASE("empty input") {
      stub.setup("");
      CHECK(stream.available() == 0);
      CHECK(spy.log() == "available() -> 0");
    }

    SUBCASE("read empty input") {
      stub.setup("");

      stream.read();

      CHECK(stream.available() == 0);
      CHECK(spy.log() ==
            "readBytes(4) -> 0"
            "available() -> 0");
    }

    SUBCASE("same a upstream") {
      CHECK(stream.available() == 8);
      CHECK(spy.log() == "available() -> 8");
    }

    SUBCASE("upstream + in buffer") {
      stream.read();

      CHECK(stream.available() == 7);
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "available() -> 4");
    }
  }

  SUBCASE("peek()") {
    SUBCASE("returns -1 when empty") {
      stub.setup("");

      int result = stream.peek();

      CHECK(result == -1);
      CHECK(spy.log() == "peek() -> -1");
    }

    SUBCASE("doesn't call readBytes() when buffer is empty") {
      stub.setup("A");

      int result = stream.peek();

      CHECK(result == 'A');
      CHECK(spy.log() == "peek() -> 65");
    }

    SUBCASE("doesn't call peek() when buffer is full") {
      stub.setup("AB");

      stream.read();
      int result = stream.peek();

      CHECK(result == 'B');
      CHECK(spy.log() == "readBytes(4) -> 2");
    }
  }

  SUBCASE("read()") {
    SUBCASE("reads 4 bytes at a time") {
      stub.setup("ABCDEFG");
      std::string result;

      for (int i = 0; i < 7; i++) {
        result += (char)stream.read();
      }

      CHECK(result == "ABCDEFG");
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "readBytes(4) -> 3");
    }

    SUBCASE("returns -1 when empty") {
      stub.setup("");

      int result = stream.read();

      CHECK(result == -1);
      CHECK(spy.log() == "readBytes(4) -> 0");
    }
  }

  SUBCASE("readBytes()") {
    SUBCASE("empty input") {
      stub.setup("");

      char c;
      size_t result = stream.readBytes(&c, 1);

      CHECK(result == 0);
      CHECK(spy.log() == "readBytes(4) -> 0");
    }

    SUBCASE("reads 4 bytes when requested one") {
      stub.setup("ABCDEFG");

      char c;
      size_t result = stream.readBytes(&c, 1);

      CHECK(c == 'A');
      CHECK(result == 1);
      CHECK(spy.log() == "readBytes(4) -> 4");
    }

    SUBCASE("copy one byte from buffer") {
      stub.setup("ABCDEFGH");
      stream.read();  // load buffer

      char c;
      size_t result = stream.readBytes(&c, 1);

      CHECK(c == 'B');
      CHECK(result == 1);
      CHECK(spy.log() == "readBytes(4) -> 4");
    }

    SUBCASE("copy content from buffer then bypass buffer") {
      stub.setup("ABCDEFGH");
      stream.read();  // load buffer

      char c[8] = {0};
      size_t result = stream.readBytes(c, 7);

      CHECK(c == std::string("BCDEFGH"));
      CHECK(result == 7);
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "readBytes(4) -> 4");
    }

    SUBCASE("copy content from buffer twice") {
      stub.setup("ABCDEFGH");
      stream.read();  // load buffer

      char c[8] = {0};
      size_t result = stream.readBytes(c, 4);

      CHECK(c == std::string("BCDE"));
      CHECK(result == 4);
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "readBytes(4) -> 4");
    }

    SUBCASE("read past the end") {
      stub.setup("A");

      char c;
      stream.readBytes(&c, 1);
      size_t result = stream.readBytes(&c, 1);

      CHECK(result == 0);
      CHECK(spy.log() ==
            "readBytes(4) -> 1"
            "readBytes(4) -> 0");
    }
  }

  SUBCASE("flush()") {
    stream.flush();
    CHECK(spy.log() == "flush()");
  }

  SUBCASE("copy constructor") {
    stub.setup("ABCDEFGH");
    bufferedStream.read();

    auto dup = bufferedStream;

    int result = dup.read();

    CHECK(result == 'B');
    CHECK(spy.log() == "readBytes(4) -> 4");
  }
}

TEST_CASE("Real example") {
  StreamStub stub;
  StreamSpy spy{stub};
  StaticBufferedStream<64> bufferedStream{spy};
  Stream& stream = bufferedStream;

  stub.setup("{\"helloWorld\":\"Hello World\"}");

  char c[] = "ABCDEFGH";
  CHECK(stream.readBytes(&c[0], 1) == 1);
  CHECK(stream.readBytes(&c[1], 1) == 1);
  CHECK(stream.readBytes(&c[2], 1) == 1);
  CHECK(stream.readBytes(&c[3], 1) == 1);

  CHECK(c == std::string("{\"heEFGH"));
  CHECK(spy.log() == "readBytes(64) -> 28");
}