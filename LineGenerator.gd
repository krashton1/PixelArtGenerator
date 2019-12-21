extends "res://ArtGenerator.gd"

# Declare member variables here. Examples:
var originPos : Vector2 = Vector2(0,0)
var destPos : Vector2 = Vector2(0,0)

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func _draw():
	
	drawPixelLine(originPos,destPos)
	
	pass
