extends Node2D

enum ObjectType{
	PIXEL,
	PIXEL_ARRAY
}

# Constant Declarations
const ColorBlack = Color(0.0, 0.0, 0.0)

# Declare member variables here. Examples:
var objectType : int #enums are currently just shorthand for dicts of constants, so type has to be int
var pixelSize : int = 16
var screenSize : Vector2 = Vector2(1280, 720)
var pixelArray = [[0,0,0,0,0],[0,1,0,1,0],[0,0,0,0,0],[1,0,0,0,1],[0,1,1,1,0]] #default smile face

# color palette
var colorPalette : Dictionary = {"black": Color(0.0, 0.0, 0.0)}


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func setup(objectType : int, screenSize : Vector2 = Vector2(1280, 720), pixelSize : int = 16, colorPalette : Dictionary = {"black": Color(0.0, 0.0, 0.0)}):
	set("objectType", objectType)
	set("pixelSize", pixelSize)
	set("screenSize", screenSize)
	set("colorPalette", colorPalette)
	pass

func setPixelArray(pixelArray : Array):
	set("pixelArray", pixelArray)
	pass


func _draw():
	
	if(objectType == ObjectType.PIXEL):
		drawPixel(Vector2(0,0), colorPalette.get("black"))
	elif(objectType == ObjectType.PIXEL_ARRAY):
		drawPixelArray(Vector2(0,0), colorPalette.get("black"))
	
	pass


func drawPixel(pos : Vector2 = Vector2(0,0), color : Color = ColorBlack):
	var rectAtPos = Rect2(pos.x, pos.y, pixelSize, pixelSize)
	draw_rect(rectAtPos,color)
	pass


func drawPixelArray(pos : Vector2 = Vector2(0,0), color : Color = ColorBlack):
	for i in range(0,pixelArray.size()):
		for j in range(0,pixelArray[i].size()):
			if pixelArray[j][i] == 1:
				drawPixel(Vector2(i * pixelSize, j * pixelSize), color)
	
	
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