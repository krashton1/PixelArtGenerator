extends "res://ArtGenerator.gd"

# Declare member variables here. Examples:
var minPts = 8
var maxPts = 12

var minRadius = 18
var maxRadius = 22

var points = []

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass



func SetupRock():
	
	points.clear()
	
	
	var pts = randi() % (maxPts - minPts) + minPts
	
	var origin : Vector2 = Vector2((assetSize-1)/2, assetSize-1)
	
	for i in range(0,pts+1):
		var thisRadius = randi() % (maxRadius - minRadius) + minRadius
#		var angle = randf() * PI
		var angle = PI / float(pts) * float(i)
		
		var ptX = origin.x + thisRadius * cos(angle)
		var ptY = origin.y + thisRadius * sin(PI + angle)
		
		var pt = Vector2(ptX, ptY)
		points.append(pt)
	
	
	for i in range(0, points.size()-1):
		addLine(points[i], points[i+1]-points[i])
	
	fillColor(origin, colorPalette.get("white"))
	
	pass
