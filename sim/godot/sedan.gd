extends CharacterBody3D

var leftCamera
var rightCamera

# How fast the player moves in meters per second.
@export var speed = 5
# The downward acceleration when in the air, in meters per second squared.
@export var fall_acceleration = 75

var max_speed = 500
var friction = -5
var acceleration = 8
var rotation_speed = 70
var target_acceleration = Vector3.ZERO
var target_velocity = Vector3.ZERO

func _ready() -> void:
    leftCamera = get_node("./ViewportLeft/CameraPivotLeft")
    rightCamera = get_node("./ViewportRight/CameraPivotRight")

func _physics_process(delta):
    var direction = Vector3.ZERO

    if Input.is_action_pressed("move_backward"):
        direction = Vector3(0, 0, -1)
        #direction.z += 1
        if Input.is_action_pressed("move_right"):
            rotation_degrees.y -= rotation_speed*delta
        if Input.is_action_pressed("move_left"):
            rotation_degrees.y += rotation_speed*delta

    if Input.is_action_pressed("move_forward"):
        direction = Vector3(0, 0, 1)
        #direction.z -= 1
        if Input.is_action_pressed("move_right"):
            rotation_degrees.y += rotation_speed*delta
        if Input.is_action_pressed("move_left"):
            rotation_degrees.y -= rotation_speed*delta

    if direction != Vector3.ZERO:
        direction = direction.normalized()
        $Pivot.basis = Basis.looking_at(direction)

    target_acceleration = direction*acceleration

    # Ground Velocity
    target_velocity += (target_acceleration + friction*target_velocity) * delta
    if target_velocity.length() <= 0.25 and target_acceleration.length() < 0.1:
        target_velocity = Vector3.ZERO
    if target_velocity.length() > max_speed:
        target_velocity = Vector3.ONE * max_speed

    # Vertical Velocity
    #if not is_on_floor(): # If in the air, fall towards the floor. Literally gravity
        #target_velocity.y = target_velocity.y - (fall_acceleration * delta)

    # Moving the Character
    #print(rotation)
    velocity = target_velocity.rotated(Vector3(0, 1, 0), rotation.y)
    var oldPos = global_position
    move_and_slide()
    var curPos = global_position
    var diff = curPos - oldPos
    #var left_diff : Vector3 = global_position - leftCamera.global_position
    #var right_diff : Vector3 = global_position - rightCamera.global_position
    ##print(global_position)
    leftCamera.global_position += diff
    rightCamera.global_position += diff
