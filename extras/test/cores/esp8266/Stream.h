// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2022
// MIT License

#pragma once

#include "Print.h"

struct Stream : Print {
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;

  virtual size_t readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
      int c = read();
      if (c < 0)
        break;
      *buffer++ = (char)c;
      count++;
    }
    return count;
  }

  virtual size_t readBytes(uint8_t *buffer, size_t length) {
    return readBytes((char *)buffer, length);
  }

  virtual String readString() {
    String result;
    int c;
    while ((c = read()) >= 0) {
      result += static_cast<char>(c);
    }
    return result;
  }

  void setTimeout(unsigned long) {}
};
