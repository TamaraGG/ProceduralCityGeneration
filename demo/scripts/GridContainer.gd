extends GridContainer


func _on_mouse_entered():
	$"../../../../../CameraPanel/MyCamera".turn_off()
	$"../../../../../MainScene/MyTileMap".turn_off_input()

func _on_mouse_exited():
	$"../../../../../CameraPanel/MyCamera".turn_on()
	$"../../../../../MainScene/MyTileMap".turn_on_input()
