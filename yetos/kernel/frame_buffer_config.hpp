#pragma once

#include <stdint.h>

enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
  // TODO: There are other formats; PixelBitMask and PixelBltOnly.
};

struct FrameBufferConfig {
  // Pointer to frame buffer.
  uint8_t *frame_buffer;
  // the number of horizontal pixels including margins.
  uint32_t pixels_per_scan_line;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};
