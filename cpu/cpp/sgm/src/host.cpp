#include <iostream>
#include <chrono>
#include "sgm.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  std::vector<uint8_t> leftImage(IMAGE_SIZE);
  std::vector<uint8_t> rightImage(IMAGE_SIZE);

  if (readImage("data/left_gray.bin", leftImage) != 0) {
    return 1;
  }
  if (readImage("data/right_gray.bin", rightImage) != 0) {
    return 1;
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto census = computeCensus(leftImage, rightImage, IMAGE_HEIGHT, IMAGE_WIDTH, { 5, 5 });
  auto end = std::chrono::high_resolution_clock::now();
  auto leftCensus  = census.first;
  auto rightCensus = census.second;

  if (writeImage("data/left_census.bin", leftCensus) != 0) {
    return 1;
  }

  if (writeImage("data/right_census.bin", rightCensus) != 0) {
    return 1;
  }

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double ms = (double)(duration.count()) / 1000000.0;

  std::cout << ms << " ms\n";

  return 0;
}
