extends ProgressBar

# Called when the node enters the scene tree for the first time.
func _ready():
	min_value = $"../ZoomSlider".min_value
	max_value = $"../ZoomSlider".max_value



func _on_mouse_entered():
	$"../../../../../CameraPanel/MyCamera".turn_off()
	$"../../../../../MainScene/MyTileMap".turn_off_input()


func _on_mouse_exited():
	$"../../../../../CameraPanel/MyCamera".turn_on()
	$"../../../../../MainScene/MyTileMap".turn_on_input()

