#include "gdexample.h"
#include "sgm.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <algorithm>

using namespace godot;

void sum::_bind_methods() {
  ClassDB::bind_method(D_METHOD("add", "num1", "num2"), &sum::add);
}

sum::sum() {
}

sum::~sum() {
}

PackedByteArray sum::add(PackedByteArray arr1, PackedByteArray arr2) {
  std::vector<uint32_t> leftCensus(IMAGE_SIZE);
  std::vector<uint32_t> rightCensus(IMAGE_SIZE);
  const int maxDisparity = 128;
  std::vector<uint32_t> costVolume(IMAGE_SIZE*maxDisparity);

  std::vector<uint32_t> optimizedCostVolume(costVolume);
  PackedByteArray disparity, disparityFilter;
  disparity.resize(IMAGE_SIZE);
  disparityFilter.resize(IMAGE_SIZE);

  auto start = std::chrono::high_resolution_clock::now();
  computeCensus(
    arr1,
    arr2,
    IMAGE_HEIGHT,
    IMAGE_WIDTH,
    { 5, 5 },
    leftCensus,
    rightCensus
  );
  /*for (int i = 0; i < IMAGE_SIZE; i++) {*/
  /*    disparity[i] = rightCensus[i];*/
  /*}*/
  computeCosts(
    leftCensus,
    rightCensus,
    IMAGE_HEIGHT,
    IMAGE_WIDTH,
    maxDisparity,
    costVolume
  );
  aggregateCosts(
    costVolume,
    IMAGE_HEIGHT,
    IMAGE_WIDTH,
    maxDisparity,
    8,
    128,
    optimizedCostVolume
  );
  selectMinCost(
    optimizedCostVolume,
    IMAGE_HEIGHT,
    IMAGE_WIDTH,
    maxDisparity,
    disparity
  );
  const int medianFilterSize = 5;
  for (int y = 0; y < IMAGE_HEIGHT; y++) {
      for (int x = 0; x < IMAGE_WIDTH; x++) {
          std::vector<uint8_t> window(medianFilterSize*medianFilterSize);
          for (int yy = 0; yy < medianFilterSize; yy++) {
              for (int xx = 0; xx < medianFilterSize; xx++) {
                  int yCoord = y + yy - int(medianFilterSize/2);
                  int xCoord = x + xx - int(medianFilterSize/2);

                  if (yCoord >= 0 && yCoord < IMAGE_HEIGHT && xCoord >= 0 && xCoord < IMAGE_WIDTH) {
                      window[yy*medianFilterSize + xx] = disparity[yCoord*IMAGE_WIDTH + xCoord];
                  } else {
                      window[yy*medianFilterSize + xx] = 0;
                  }
              }
              std::sort(window.begin(), window.end());
              disparityFilter[y*IMAGE_WIDTH + x] = window[(medianFilterSize*medianFilterSize)/2];
          }
      }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  double ms = duration / 1000.0;

  UtilityFunctions::print(ms);

  return disparityFilter;
}
