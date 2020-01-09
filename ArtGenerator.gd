extends Node2D

enum ObjectType{
	PIXEL,
	PIXEL_ARRAY
}

# Constant Declarations
const ColorBlack = Color(0.0, 0.0, 0.0)


# Declare member variables here. Examples:
var objectType : int #enums are currently just shorthand for dicts of constants, so type has to be int
var assetSize = 64
var pixelSize = 16
var screenSize : Vector2 = Vector2(1280, 720)
var pixelArray = [[0,0,0,0,0],[0,1,0,1,0],[0,0,0,0,0],[1,0,0,0,1],[0,1,1,1,0]] #default smile face

func getPixelSize():
	return pixelSize

func setPixelSize(var ps):
	pixelSize = ps
	pass

# color palette
var colorPalette : Dictionary = {"black": Color(0.0, 0.0, 0.0)}


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame
func _draw():
	drawPixelArray(Vector2(0,0), colorPalette.get("black"))
	pass


func setup(objectType : int, screenSize : Vector2 = Vector2(1280, 720), pixelSize = 16, assetSize : int = 64, colorPalette : Dictionary = {"black": Color(0.0, 0.0, 0.0)}):
	set("objectType", objectType)
	set("pixelSize", pixelSize)
	set("assetSize", assetSize)
	set("screenSize", screenSize)
	set("colorPalette", colorPalette)
	
	# Init 2D array to correct dimensions
	pixelArray.resize(assetSize)
	for x in range(assetSize):
		pixelArray[x]=[]
		pixelArray[x].resize(assetSize)
		for y in range(assetSize):
			 pixelArray[x][y]=0
	
	pass

func setPixelArray(pixelArray : Array):
	set("pixelArray", pixelArray)
	pass

func setPixelArrayElem(x : int, y : int, value):
	pixelArray[x][y]=value
	pass


func drawPixel(pos : Vector2 = Vector2(0,0), color : Color = ColorBlack):
	var rectAtPos = Rect2(pos.x, pos.y, pixelSize, pixelSize)
	draw_rect(rectAtPos,color)
	pass


func drawPixelArray(pos : Vector2 = Vector2(0,0), color : Color = ColorBlack):
	for i in range(0,pixelArray.size()):
		for j in range(0,pixelArray[i].size()):
			if str(pixelArray[i][j]) != "0":
				drawPixel(Vector2(i * pixelSize, j * pixelSize), pixelArray[i][j])




func addLine(origin : Vector2, dir : Vector2, color : Color = ColorBlack):
#	# normalize line length
#	from -= Vector2(1,1)
#	to -= Vector2(1,1)
#
	# step size
	var stepSize =  float(1.0) / float(pow(assetSize, 1.0))
	
	# dest pos
	var dest = origin + dir
	
	# if a line is more vertical than horizontal, than it should not have 2 pixels in same y coord
	# inverse for horizontal
	var isVertical
	if abs(dir.y) >= abs(dir.x):
		isVertical = true
	else:
		isVertical = false
	
	# temp x and y coords as we step through the line
	var tX = origin.x
	var tY = origin.y
	
	# holds the previous/current coord for x/y, depending on if line is vert or horiz
	# used to remove jaggies
	var lastXY
	var thisXY
	
	# Coefficient of line
	# coef = 0 draws first pixel of line
	# coef = 1 draws final pixel of line
	var coef = 0.0
	
	while(coef <= 1.0):
		tX = round(origin.x * (1-coef) + dest.x * coef)
		tY = round(origin.y * (1-coef) + dest.y * coef)
		
		coef += stepSize
		
		if tX < 0 || tX >= assetSize || tY < 0 || tY >= assetSize:
			continue
		
		if isVertical:
			thisXY = tY
		else:
			thisXY = tX
		
		if thisXY != lastXY:
			setPixelArrayElem(tX, tY, ColorBlack)
			lastXY = thisXY
		
	pass


# Converts array from 'human-ish readable' to 'coord readable'
#
# smile1 = [[0,0,0,0,0],[0,1,0,1,0],[0,0,0,0,0],[1,0,0,0,1],[0,1,1,1,0]]
# smile2 = [[0,0,0,1,0],[0,1,0,0,1],[0,0,0,0,1],[0,1,0,0,1],[0,0,0,1,0]]
func convertArray(origArray):
	
	# Create and size converted array
	var newArray = []
	newArray.resize(origArray[0].size())
	for x in range(origArray[0].size()):
		newArray[x]=[]
		newArray[x].resize(origArray.size())
		for y in range(origArray.size()):
			 newArray[x][y]=0
	
	# Copy elems from old to new Array, swapping x and y values
	for y in range(newArray.size()):
		for x in range(newArray[y].size()):
			newArray[x][y] = origArray[y][x]
	
	return newArray


# Takes an image file, and pixelizes it
func pixelizeImage(image : Image):
	
	var stepSizeX = image.get_width() / float(assetSize)
	var stepSizeY = image.get_height() / float(assetSize)

	for x in range(0,assetSize):
		for y in range(0,assetSize):
			var color = image.get_pixel(int(stepSizeX * x), int(stepSizeY * y))
			setPixelArrayElem(x,y,color)
	
	pass