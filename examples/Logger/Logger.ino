// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License
//
// This example shows how to log what goes through a Stream, both reads and
// writes operations.

#include <StreamUtils.h>

// Create a new stream that will forward all calls to Serial, and log to Serial.
// Everything will be written twice to the Serial!
LoggingStream loggingStream(Serial, Serial);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;

  // Write to the serial port.
  // Because loggingStream logs each write operation, the string will we written
  // twice
  loggingStream.println("Hello World!");
}

void loop() {
  // Read from the serial port.
  // Because loggingStream logs each read operation, everything we read is
  // printed back to the serial port.
  while (loggingStream.available()) {
    loggingStream.read();
  }
}