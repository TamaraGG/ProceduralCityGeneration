extends Node2D



func _process(_delta):
	$NavigationAgent2D.get_next_path_position()
