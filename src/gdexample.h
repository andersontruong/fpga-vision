#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class sum : public Node {
	GDCLASS(sum, Node)

protected:
	static void _bind_methods();

public:
	sum();
	~sum();

  void sum_func(int num1, int num2) override;
};

}

#endif
