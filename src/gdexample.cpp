#include "gdexample.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;
using namespace std;

void sum::_bind_methods() {
}

sum::sum() {
}

sum::~sum() {
}

void sum::sum_func(int num1, int num2) {
  cout << num1 + num2 << endl;
}
