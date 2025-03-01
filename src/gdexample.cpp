#include "gdexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;

void sum::_bind_methods() {
  ClassDB::bind_method(D_METHOD("add", "num1", "num2"), &sum::add);
}

sum::sum() {
}

sum::~sum() {
}

void sum::add(TypedArray<uint8_t> arr1, TypedArray<uint8_t> arr2) {
  UtilityFunctions::print(arr1, arr2);
}
