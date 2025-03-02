#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include <utility>
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <cmath>

#define IMAGE_WIDTH  224
#define IMAGE_HEIGHT 128
#define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGHT

using namespace godot;

void computeCensus(PackedByteArray left,
                   PackedByteArray right,
                   const int height,
                   const int width,
                   std::pair<int, int> kernelShape,
                   std::vector<uint32_t>& leftCensus,
                   std::vector<uint32_t>& rightCensus);

void computeCosts(const std::vector<uint32_t>& leftCensus,
                  const std::vector<uint32_t>& rightCensus,
                  int height,
                  int width,
                  int maxDisparity,
                  std::vector<uint32_t>& costVolume);

void aggregateCosts(const std::vector<uint32_t>& costVolume,
                  int height,
                  int width,
                  int maxDisparity,
                  int P1,
                  int P2,
                  std::vector<uint32_t>& optimizedCostVolume);

void selectMinCost(const std::vector<uint32_t>& costVolume,
                  int height,
                  int width,
                  int maxDisparity,
                  PackedByteArray& disparity);
