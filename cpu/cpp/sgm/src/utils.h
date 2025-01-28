#pragma once

#include "sgm.h"
#include <string>
#include <vector>
#include <stdint.h>
#include <stdio.h>

int readImage(std::string filename, std::vector<uint8_t>& outImage) {
  FILE* fp = fopen(filename.c_str(), "rb");
  if (fp == NULL) {
    perror("Error opening file for readImage");
    return 1;
  }
  size_t bytesRead = fread(outImage.data(), sizeof(uint8_t), outImage.size(), fp);

  if (bytesRead == 0) {
    perror("No bytes read!");
  }

  fclose(fp);

  return 0;
}

int writeImage(std::string filename, const std::vector<uint8_t>& outImage) {
  FILE* fp = fopen(filename.c_str(), "wb");
  if (fp == NULL) {
    perror("Error opening file for writeImage");
    return 1;
  }
  fwrite(outImage.data(), sizeof(uint8_t), outImage.size(), fp);

  fclose(fp);

  return 0;
}

int writeImage(std::string filename, const std::vector<uint32_t>& outImage) {
  FILE* fp = fopen(filename.c_str(), "wb");
  if (fp == NULL) {
    perror("Error opening file for writeImage");
    return 1;
  }
  fwrite(outImage.data(), sizeof(uint32_t), outImage.size(), fp);

  fclose(fp);

  return 0;
}
