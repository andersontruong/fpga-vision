#pragma once

#include <utility>
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <cmath>

#define IMAGE_WIDTH  480
#define IMAGE_HEIGHT 270
#define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGHT

void computeCensus(std::vector<uint8_t> left,
                   std::vector<uint8_t> right,
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
                  std::vector<uint32_t>& disparity);
