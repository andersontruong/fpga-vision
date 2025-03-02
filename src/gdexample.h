#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class sum : public Node {
	GDCLASS(sum, Node)

protected:
	static void _bind_methods();

public:
	sum();
	~sum();

  PackedByteArray add(PackedByteArray arr1, PackedByteArray arr2);
};

}

#endif
