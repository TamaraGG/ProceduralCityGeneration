extends Node2D


func _ready():
	var tilemap = $MyTileMap
	tilemap.generate_map()

	get_tree().get_root().size_changed.connect(resize)
	
	
func resize():
	#$"../MyCamera".window_resized()
	pass
	
	
func _on_my_camera_zoom_changed(new_zoom):
	#$"../CanvasLayer/Panel2/MarginContainer/VBoxContainer/ZoomProgressBar".value = new_zoom
	$"../CanvasLayer/ScalePanel/MarginContainer/VBoxContainer/ZoomSlider".value = new_zoom
	


func _on_my_tile_map_time_a_star_changed(time):
	$"../CanvasLayer/FindPathPanel/MarginContainer/VBoxContainer/AlgorithmsGrid/AStarTimeLabel".text = time + " ms"


func _on_my_tile_map_time_dijkstra_changed(time):
	$"../CanvasLayer/FindPathPanel/MarginContainer/VBoxContainer/AlgorithmsGrid/DijkstraPathButton".text = time + " ms"
