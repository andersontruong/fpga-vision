#include <iostream>
#include <chrono>
#include "sgm.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  int numTests = 1;
  if (argc > 2) {
    perror("Invalid number of arguments!");
    return 1;
  } else if (argc == 2) {
    numTests = std::stoi(argv[1]);
  }
  std::vector<uint8_t> leftImage(IMAGE_SIZE);
  std::vector<uint8_t> rightImage(IMAGE_SIZE);

  if (readImage("data/left_gray.bin", leftImage) != 0) {
    return 1;
  }
  if (readImage("data/right_gray.bin", rightImage) != 0) {
    return 1;
  }

  std::vector<uint32_t> leftCensus(leftImage.size());
  std::vector<uint32_t> rightCensus(rightImage.size());

  double msTotal = 0.0;
  double msAverage = 0.0;
  for (int i = 0; i < numTests; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    computeCensus(leftImage, rightImage, IMAGE_HEIGHT, IMAGE_WIDTH, { 5, 5 }, leftCensus, rightCensus);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    msAverage += (double)(duration.count()) / 1000000.0;
  }
  msAverage /= numTests;
  msTotal += msAverage;
  std::cout << "Census: " << msAverage << " ms\n";

  const int maxDisparity = 32;
  std::vector<uint32_t> costVolume(IMAGE_SIZE*maxDisparity);

  msAverage = 0.0;
  for (int i = 0; i < numTests; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    computeCosts(leftCensus,
                 rightCensus,
                 IMAGE_HEIGHT,
                 IMAGE_WIDTH,
                 maxDisparity,
                 costVolume);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    msAverage += (double)(duration.count()) / 1000000.0;
  }
  msAverage /= numTests;
  msTotal += msAverage;
  std::cout << "Cost: " << msAverage << " ms\n";

  std::vector<uint32_t> optimizedCostVolume(costVolume);
  std::vector<uint32_t> disparity(IMAGE_SIZE);

  msAverage = 0.0;
  for (int i = 0; i < numTests; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    aggregateCosts(costVolume,
                 IMAGE_HEIGHT,
                 IMAGE_WIDTH,
                 maxDisparity,
                 8,
                 128,
                 optimizedCostVolume);
    selectMinCost(optimizedCostVolume,
                IMAGE_HEIGHT,
                IMAGE_WIDTH,
                maxDisparity,
                disparity);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    msAverage += (double)(duration.count()) / 1000000.0;
  }
  msAverage /= numTests;
  msTotal += msAverage;
  std::cout << "Aggregate: " << msAverage << " ms\n";

  std::cout << "Total: " << msTotal << " ms\n";

  if (writeImage("data/left_census.bin", leftCensus) != 0) {
    return 1;
  }

  if (writeImage("data/right_census.bin", rightCensus) != 0) {
    return 1;
  }

  if (writeImage("data/disparity.bin", disparity) != 0) {
    return 1;
  }


  return 0;
}
