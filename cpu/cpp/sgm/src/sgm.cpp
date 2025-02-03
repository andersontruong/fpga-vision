#include "sgm.h"
#include <bitset>
#include <iostream>
#include <algorithm>

void computeCensus(std::vector<uint8_t> left,
                   std::vector<uint8_t> right,
                   int height,
                   int width,
                   std::pair<int, int> kernelShape,
                   std::vector<uint32_t>& leftCensus,
                   std::vector<uint32_t>& rightCensus
) {
  const int kernelHeight = kernelShape.first;
  const int kernelWidth  = kernelShape.second;
  const int yOffset = floor(kernelHeight / 2);
  const int xOffset = floor(kernelWidth  / 2);

  for (int y = yOffset; y < height - yOffset; y++) {
    for (int x = xOffset; x < width - xOffset; x++) {
      uint8_t centerLeft  = left[y*width + x];
      uint8_t centerRight = right[y*width + x];

      // Compute census kernel
      uint32_t leftCompare  = 0;
      uint32_t rightCompare = 0;
      for (int yy = 0; yy < kernelHeight; yy++) {
        for (int xx = 0; xx < kernelWidth; xx++) {
          uint8_t leftPixel  =  left[(y - yOffset + yy)*width + x - xOffset + xx];
          uint8_t rightPixel = right[(y - yOffset + yy)*width + x - xOffset + xx];

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
}

void computeCosts(const std::vector<uint32_t>& leftCensus,
                  const std::vector<uint32_t>& rightCensus,
                  int height,
                  int width,
                  int maxDisparity,
                  std::vector<uint32_t>& costVolume
) {

  for (int d = 0; d < maxDisparity; d++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int rhs = x >= d ? rightCensus[y*width + x - d] : 0;
        uint32_t leftXor = leftCensus[y*width + x] ^ rhs;
        int count = std::bitset<32>(leftXor).count();
        costVolume[(y*width + x)*maxDisparity + d] = count;
      }
    }
  }
}

void aggregateCosts(const std::vector<uint32_t>& costVolume,
                  int height,
                  int width,
                  int maxDisparity,
                  int P1,
                  int P2,
                  std::vector<uint32_t>& optimizedCostVolume) {

  std::vector<uint32_t> LRcost(costVolume);
  std::vector<uint32_t> RLcost(costVolume);
  std::vector<uint32_t> TBcost(costVolume);
  std::vector<uint32_t> BTcost(costVolume);

  // Left-to-Right cost
  for (int y = 0; y < height; y++) {
    // Initial left-side cost
    auto begin = LRcost.begin() + ((y*width))*maxDisparity;
    uint32_t prevMin = *std::min_element(begin, begin + maxDisparity);
    for (int x = 1; x < width; x++) {
      uint32_t currMin = 0xFFFFFFFF;
      for (int d = 0; d < maxDisparity; d++) {
        uint32_t currIndex = ((y*width) + x)*maxDisparity + d;
        uint32_t prevIndex = ((y*width) + x - 1)*maxDisparity + d;
        // Matching term
        uint32_t D = LRcost[currIndex];
        // Regularization term
        uint32_t reg = LRcost[prevIndex];
        reg = std::min(reg, LRcost[prevIndex]);
        if (d > 0) {
          reg = std::min(reg, LRcost[prevIndex - 1] + P1);
        }
        if (d < (maxDisparity - 1)) {
          reg = std::min(reg, LRcost[prevIndex + 1] + P1);
        }
        reg = std::min(reg, prevMin + P2);
        uint32_t cost = D + reg - prevMin;
        currMin = std::min(currMin, cost);
        LRcost[currIndex] = cost;
        optimizedCostVolume[currIndex] += cost;
      }
      prevMin = currMin;
    }
  }

  // Left-to-Right cost
  for (int y = 0; y < height; y++) {
    // Initial right-side cost
    auto begin = RLcost.begin() + ((y*width))*maxDisparity;
    uint32_t prevMin = *std::min_element(begin, begin + maxDisparity);
    for (int x = width - 2; x >= 0; x--) {
      uint32_t currMin = 0xFFFFFFFF;
      for (int d = 0; d < maxDisparity; d++) {
        uint32_t currIndex = ((y*width) + x)*maxDisparity + d;
        uint32_t prevIndex = ((y*width) + x + 1)*maxDisparity + d;
        // Matching term
        uint32_t D = RLcost[currIndex];
        // Regularization term
        uint32_t reg = RLcost[prevIndex];
        reg = std::min(reg, RLcost[prevIndex]);
        if (d > 0) {
          reg = std::min(reg, RLcost[prevIndex - 1] + P1);
        }
        if (d < (maxDisparity - 1)) {
          reg = std::min(reg, RLcost[prevIndex + 1] + P1);
        }
        reg = std::min(reg, prevMin + P2);
        uint32_t cost = D + reg - prevMin;
        currMin = std::min(currMin, cost);
        RLcost[currIndex] = cost;
        optimizedCostVolume[currIndex] += cost;
      }
      prevMin = currMin;
    }
  }

  for (int x = 0; x < width; x++) {
    // Initial top-side cost
    uint32_t prevMin = 0xFFFFFFFF;
    for (int d = 0; d < maxDisparity; d++) {
      prevMin = std::min(prevMin, TBcost[x*maxDisparity + d]);
    }
    for (int y = 1; y < height; y++) {
      uint32_t currMin = 0xFFFFFFFF;
      for (int d = 0; d < maxDisparity; d++) {
        uint32_t currIndex = ((y*width) + x)*maxDisparity + d;
        uint32_t prevIndex = (((y-1)*width) + x)*maxDisparity + d;
        // Matching term
        uint32_t D = TBcost[currIndex];
        // Regularization term
        uint32_t reg = TBcost[prevIndex];
        reg = std::min(reg, TBcost[prevIndex]);
        if (d > 0) {
          reg = std::min(reg, TBcost[prevIndex - 1] + P1);
        }
        if (d < (maxDisparity - 1)) {
          reg = std::min(reg, TBcost[prevIndex + 1] + P1);
        }
        reg = std::min(reg, prevMin + P2);
        uint32_t cost = D + reg - prevMin;
        currMin = std::min(currMin, cost);
        TBcost[currIndex] = cost;
        optimizedCostVolume[currIndex] += cost;
      }
      prevMin = currMin;
    }
  }

  for (int x = 0; x < width; x++) {
    // Initial bottom-side cost
    uint32_t prevMin = 0xFFFFFFFF;
    for (int d = 0; d < maxDisparity; d++) {
      prevMin = std::min(prevMin, BTcost[((height-1)*width + x)*maxDisparity + d]);
    }
    for (int y = height - 2; y >= 0; y--) {
      uint32_t currMin = 0xFFFFFFFF;
      for (int d = 0; d < maxDisparity; d++) {
        uint32_t currIndex = ((y*width) + x)*maxDisparity + d;
        uint32_t prevIndex = (((y+1)*width) + x)*maxDisparity + d;
        // Matching term
        uint32_t D = BTcost[currIndex];
        // Regularization term
        uint32_t reg = BTcost[prevIndex];
        reg = std::min(reg, BTcost[prevIndex]);
        if (d > 0) {
          reg = std::min(reg, BTcost[prevIndex - 1] + P1);
        }
        if (d < (maxDisparity - 1)) {
          reg = std::min(reg, BTcost[prevIndex + 1] + P1);
        }
        reg = std::min(reg, prevMin + P2);
        uint32_t cost = D + reg - prevMin;
        currMin = std::min(currMin, cost);
        BTcost[currIndex] = cost;
        optimizedCostVolume[currIndex] += cost;
      }
      prevMin = currMin;
    }
  }
}

void selectMinCost(const std::vector<uint32_t>& costVolume,
                  int height,
                  int width,
                  int maxDisparity,
                  std::vector<uint32_t>& disparity
) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint32_t cost = 0x0;
      int disp = -1;
      for (int d = 0; d < maxDisparity; d++) {
        int localCost = costVolume[(y*width + x)*maxDisparity + d];
        if (localCost < cost || disp == -1) {
          cost = localCost;
          disp = d;
        }
      }
      disparity[y*width + x] = std::max(0, disp);
    }
  }
}
