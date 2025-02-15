// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2022
// MIT License

#pragma once

#include <Stream.h>
#include <WString.h>

using IPAddress = String;

class Client : public Stream {
 public:
  virtual int connect(IPAddress ip, uint16_t port) = 0;
  virtual int connect(const char *host, uint16_t port) = 0;
  virtual uint8_t connected() = 0;
  virtual void stop() = 0;
  virtual operator bool() = 0;

  virtual int read() = 0;
  virtual int read(uint8_t *buf, size_t size) = 0;
};
