extends MultiMeshInstance3D

var delta_mod = 0
var viewportLeft
var viewportRight
var sumNode

var IMAGE_HEIGHT = 128
var IMAGE_WIDTH = 224

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    viewportLeft = get_node("../ViewportLeft")
    viewportRight = get_node("../ViewportRight")
    sumNode = get_node("../sum")
    pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
    delta_mod += 1
    #if delta_mod % 24 == 0:
        #visible = false
    if delta_mod % 25 == 0:
        delta_mod = 0
        var leftImage = viewportLeft.get_texture().get_image()
        var rightImage = viewportRight.get_texture().get_image()
        visible = true
        ##print(leftImage.get_data_size())
        leftImage.convert(0)
        #print(leftImage.get_data())
        rightImage.convert(0)
        #print(leftImage.get_data_size())
        var disparity = sumNode.add(leftImage.get_data(), rightImage.get_data())
        #print(disparity.size())
        #draw(leftImage.get_data())
        # draw(disparity)
    
    
func print():
    print("Child!")

func draw(disparity: PackedByteArray):
    var points : Array
    var colors : Array
    for x in IMAGE_WIDTH:
        for y in IMAGE_HEIGHT:
            var depth = disparity[(IMAGE_HEIGHT - y) * IMAGE_WIDTH - x - 1]
            #if depth > 0:
            var depth_color = Vector3(depth, depth/10, 0)
            points.push_back(Vector3(x/400.0 - 0.07, y / 400.0 + 0.21, 1.2))
            colors.push_back(depth_color)
            #else:
                #var depth_color = Vector3(0, 0, 0)
                #points.push_back(Vector3(x/400.0 - 0.35, y / 400.0 + 0.05, 1))
                #colors.push_back(depth_color)

    multimesh = MultiMesh.new()
    multimesh.transform_format = MultiMesh.TRANSFORM_3D
    multimesh.use_colors=true
        
    var pmesh := PointMesh.new()
    var material := StandardMaterial3D.new()
    material.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
    material.use_point_size = true
    material.albedo_color=Color(1,1,1)
    material.point_size=9
    material.vertex_color_use_as_albedo=true
    pmesh.material=material    
    
    multimesh.mesh=pmesh
    multimesh.instance_count = len(points)
    
    for i in multimesh.instance_count:
        multimesh.set_instance_transform(i, Transform3D(Basis(), points[i]))    
        multimesh.set_instance_color(i,Color(colors[i].x/256.0,colors[i].y/256.0,colors[i].z/256.0))
