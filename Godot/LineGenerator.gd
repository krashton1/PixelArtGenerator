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

func drawPixelLine(from : Vector2, to : Vector2, color : Color = ColorBlack):
	# step size
	var stepSize = pixelSize / max(screenSize.x, screenSize.y)
	
	# temp x and y coords as we step through the line
	var tX = from.x
	var tY = from.y
	
	# Coefficient of line
	# coef = 0 draws first pixel of line
	# coef = 1 draws final pixel of line
	var coef = 0.0
	
	while(coef <= 1.0):
		tX = floor((from.x * (1-coef) + to.x * coef) / pixelSize) * pixelSize
		tY = floor((from.y * (1-coef) + to.y * coef) / pixelSize) * pixelSize
		drawPixel(Vector2(tX, tY), color)
		coef += stepSize
	
	pass
