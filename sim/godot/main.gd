extends Node

func _ready() -> void:
	var arr1 = [1,2,3,4]
	var arr2 = [2,3,4,5]
	$sum.add(arr1, arr2)
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
