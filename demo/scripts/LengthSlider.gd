extends HSlider

#@onready var tileMap : MyTileMap = $"../../../../../../Panel3/MainScene/MyTileMap"

func _ready():
	value = $"../../../../../../MainScene/MyTileMap/RoadGenerator".get_length()
	

func _on_drag_ended(_value_changed):
	$"../../../../../../MainScene/MyTileMap/RoadGenerator".set_length(value)
	
func _on_mouse_entered():
	$"../../../../../../CameraPanel/MyCamera".turn_off()
	$"../../../../../../MainScene/MyTileMap".turn_off_input()

func _on_mouse_exited():
	$"../../../../../../CameraPanel/MyCamera".turn_on()
	$"../../../../../../MainScene/MyTileMap".turn_on_input()
	
