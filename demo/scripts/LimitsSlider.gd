extends HSlider

#@onready var tileMap : MyTileMap = $"../../../../../../Panel3/MainScene/MyTileMap"
# Called when the node enters the scene tree for the first time.
func _ready():
	value = $"../../../../../../MainScene/MyTileMap/RoadGenerator/LSystemGenerator".get_iteration_limit()
	

func _on_drag_ended(_value_changed):
	$"../../../../../../MainScene/MyTileMap/RoadGenerator/LSystemGenerator".set_iteration_limit(value)

func _on_mouse_entered():
	$"../../../../../../CameraPanel/MyCamera".turn_off()
	$"../../../../../../MainScene/MyTileMap".turn_off_input()

func _on_mouse_exited():
	$"../../../../../../CameraPanel/MyCamera".turn_on()
	$"../../../../../../MainScene/MyTileMap".turn_on_input()
