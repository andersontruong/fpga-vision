#pragma once

#include <utility>
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <cmath>

#include <iostream>

#define IMAGE_WIDTH  480
#define IMAGE_HEIGHT 270
#define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGHT

template <size_t height, size_t width, size_t numChannels>
using MatrixU8 = std::array<std::array<std::array<uint8_t, numChannels>, width>, height>;

template <size_t height, size_t width, size_t numChannels>
using MatrixU32 = std::array<std::array<std::array<uint32_t, numChannels>, width>, height>;

std::pair<std::vector<uint32_t>, std::vector<uint32_t>> computeCensus(std::vector<uint8_t> left, std::vector<uint8_t> right, const int height, const int width, std::pair<int, int> kernelShape) {
  const int kernelHeight = kernelShape.first;
  const int kernelWidth  = kernelShape.second;
  const int y_offset = floor(kernelHeight / 2);
  const int x_offset = floor(kernelWidth  / 2);

  std::vector<uint32_t> leftCensus(left.size());
  std::vector<uint32_t> rightCensus(right.size());

  for (int y = y_offset; y < height - y_offset; y++) {
    for (int x = x_offset; x < width - x_offset; x++) {
      uint8_t centerLeft  = left[y*width + x];
      uint8_t centerRight = right[y*width + x];

      // Compute census kernel
      uint32_t leftCompare  = 0;
      uint32_t rightCompare = 0;
      for (int yy = 0; yy < kernelHeight; yy++) {
        for (int xx = 0; xx < kernelWidth; xx++) {
          uint8_t leftPixel  =  left[(y - y_offset + yy)*width + x - x_offset + xx];
          uint8_t rightPixel = right[(y - y_offset + yy)*width + x - x_offset + xx];

          // Set bit to 1 if greater than center, 0 else
          if (leftPixel  > centerLeft) leftCompare |= 1;
          if (rightPixel > centerRight) rightCompare |= 1;

          // Shift left to pack comparison bits
          leftCompare  <<= 1;
          rightCompare <<= 1;
        }
      }


      leftCensus[y*width + x]  = leftCompare;
      rightCensus[y*width + x] = rightCompare;
    }
  }

  return { leftCensus, rightCensus };
}

