extends HSlider


#@onready var tileMap = $"../../../../../Panel3/MainScene/MyTileMap"

func _ready():
	max_value = $"../../../../../CameraPanel/MyCamera".get_max_zoom()
	min_value = $"../../../../../CameraPanel/MyCamera".get_min_zoom()
	value = $"../../../../../CameraPanel/MyCamera".get_zoom().x
	step = 0.01

func _on_mouse_entered():
	$"../../../../../CameraPanel/MyCamera".turn_off()
	$"../../../../../MainScene/MyTileMap".turn_off_input()


func _on_mouse_exited():
	$"../../../../../CameraPanel/MyCamera".turn_on()
	$"../../../../../MainScene/MyTileMap".turn_on_input()


func _on_value_changed(_value_changed):
	if value > min_value:
		$"../../../../../CameraPanel/MyCamera".change_zoom(value * Vector2(1,1))
	$"../ZoomProgressBar".value = value
