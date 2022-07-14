/**
 * @file graphics.cpp
 * @brief Tools for graphics.
 */

#pragma once

#include "frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};

/** PixelWriter is abstruct class for drawing pixels. */
class PixelWriter {
public:
  PixelWriter(const FrameBufferConfig &config) : config_{config} {}
  virtual ~PixelWriter() = default;
  virtual void Write(int x, int y, const PixelColor &c) = 0;

protected:
  uint8_t *PixelAt(int x, int y) {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

private:
  const FrameBufferConfig &config_;
};

/** RGBResv8BitPerColorPixelWriter is abstruct class for drawing pixels. */
class RGBResv8BitPerColorPixelWriter : public PixelWriter {
public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor &c) override;
};

/** BGRResv8BitPerColorPixelWriter is abstruct class for drawing pixels. */
class BGRResv8BitPerColorPixelWriter : public PixelWriter {
public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor &c) override;
};

template <typename T> struct Vector2D { T x, y; };

/**
 * DrawRectangle draws a rectangle.
 */
void DrawRectangle(
    PixelWriter &writer,
    const Vector2D<int> &pos,
    const Vector2D<int> &size,
    const PixelColor &c);

/**
 * FillRectangle fills a rectangle.
 */
void FillRectangle(
    PixelWriter &writer,
    const Vector2D<int> &pos,
    const Vector2D<int> &size,
    const PixelColor &c);
