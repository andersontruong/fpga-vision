extends Node

var n1 : int
var n2 : int

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	n1 = get_parent().parent_num1
	n2 = get_parent().parent_num2
	sum_func(n1, n2)

func sum_func(num1: int, num2: int) -> void:
	pass
		
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
