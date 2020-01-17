extends Node2D

# Declare member variables here. Examples:
var colorPalette = {}

var dynamicLineNode

var timer = 0


# Called when the node enters the scene tree for the first time.
func _ready():
	
	if (OS.get_cmdline_args().join("") == "-waitForDebug"):
		yield(get_tree().create_timer(8.0),"timeout")
		
	randomize()
	buildColorPalette()
	
	var artGenerator = preload("res://ArtGenerator.tscn")
	var playerScene = preload("res://Player.tscn")
	var player = playerScene.instance()
	add_child(player)
	player.position = Vector2(200,200)
	

#	var smileGenerator = artGenerator.instance()
#	add_child(smileGenerator)
#	smileGenerator.setup(1, Vector2(1280,720), 16, 64, colorPalette)
#	smileGenerator.position = Vector2(720,360)
#
#	var smile = [[0,0,0,0,0],[0,1,0,1,0],[0,0,0,0,0],[1,0,0,0,1],[0,1,1,1,0]]
#	smile = smileGenerator.convertArray(smile)
#
#	for x in range(0,smile.size()):
#		for y in range(0,smile[x].size()):
#			if smile[x][y] == 1:
#				smileGenerator.setPixelArrayElem(x,y,"#c1c1c1")
#
#
#
#
#	var image = Image.new()
#	image.load("res://rengar.png")
#	image.lock()
#
#	var rengarGenerator = artGenerator.instance()
#	add_child(rengarGenerator)
#	rengarGenerator.setup(0, Vector2(1280,720), 4, 64, colorPalette)
#	rengarGenerator.position = Vector2(320,360)
#
#	rengarGenerator.pixelizeImage(image)
##	rengarGenerator.set_scale(Vector2(0.25,0.25))
#
#
#
#
	var lineGenerator = artGenerator.instance()
	add_child(lineGenerator)
	lineGenerator.set_name("lineGen")
##	lineGenerator.setup(0, Vector2(1280,720), 3.5, 64, colorPalette)
#	lineGenerator.position = Vector2(1000,0)
#	lineGenerator.addLine(Vector2(0,0),Vector2(63,63))
#
#
#
#
#	var shapeGenerator = artGenerator.instance()
#	add_child(shapeGenerator)
#	shapeGenerator.setup(0, Vector2(1280,720), 4, 64, colorPalette)
#	shapeGenerator.position = Vector2(500,0)
#
#	shapeGenerator.addLine(Vector2(31,31),Vector2(0,0))
#	shapeGenerator.addLine(Vector2(30,31),Vector2(0,0))
#	shapeGenerator.addLine(Vector2(32,31),Vector2(0,0))
#	shapeGenerator.addLine(Vector2(31,30),Vector2(0,0))
#	shapeGenerator.addLine(Vector2(31,32),Vector2(0,0))
#
#	shapeGenerator.addCircle(Vector2(31,31), 30, 31)
#
#	shapeGenerator.fillColor(Vector2(0,0), colorPalette.get('white'))
#
#
#	var tri = [Vector2(31, 21), Vector2(37, 37), Vector2(25, 37)]
#	var triGen = artGenerator.instance()
#	add_child(triGen)
#	triGen.setup(0, Vector2(1280,720), 4, 64, colorPalette)
#	triGen.position = Vector2(900,400)
#	triGen.drawShape(tri, colorPalette.get('black'), true, colorPalette.get('white'))
	
	
	
#	var rockGenParent = artGenerator.instance()
#	var rockGen = rockGenParent.get_child(0)
#	rockGenParent.set_name("rockGen")
#	add_child(rockGenParent)
#	rockGen.setup(0, Vector2(1280,720), 4, 64, colorPalette)
#	rockGen.position = Vector2(200,0)
#	rockGen.SetupRock()
	
	



	pass # Replace with function body.


func _draw():
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	var l = 64
	var x1 = randi()%64
	var y1 = randi()%64
	var x2 = randi()%l-l/2
	var y2 = randi()%l-l/2
	
#	var lineGenerator = get_node("lineGen")
##	lineGenerator.clearPixelArray()
#	lineGenerator.addLine(Vector2(x1,y1),Vector2(x2,y2))
#	lineGenerator.update()
	
#	var ps = lineGenerator.getPixelSize()
#	if ps > 1.7:
#		lineGenerator.setPixelSize(ps-.01)
	
	
#	timer += delta
#	if (timer > 1):
#		timer-= 1
#		var rockGen = get_node("rockGen")
#		rockGen.get_child(0).clearPixelArray()
#		rockGen.get_child(0).SetupRock()
#		rockGen.get_child(0).update()
	
	pass


func buildColorPalette():
	colorPalette["white"] = Color("#c1c1c1")
	colorPalette["grey-3"] = Color("#7a7a7a")
	colorPalette["grey-2"] = Color("#6e6e6e")
	colorPalette["grey-1"] = Color("#646464")
	colorPalette["grey"] = Color("#404040")
	colorPalette["grey+1"] = Color("#303030")
	colorPalette["grey+2"] = Color("#202020")
	colorPalette["black"] = Color("#181818")
	colorPalette["green-4"] = Color("#003004")
	colorPalette["green-3"] = Color("#054000")
	colorPalette["green-2"] = Color("#155010")
	colorPalette["green-1"] = Color("#256020")
	colorPalette["green"] = Color("#26a031")
	colorPalette["green+1"] = Color("#32b326")
	colorPalette["green+2"] = Color("#3dc131")
	colorPalette["cyan-4"] = Color("#002830")
	colorPalette["cyan-3"] = Color("#003f40")
	colorPalette["cyan-2"] = Color("#104f50")
	colorPalette["cyan-1"] = Color("#205f60")
	colorPalette["cyan"] = Color("#2293a2")
	colorPalette["cyan+1"] = Color("#29aaaf")
	colorPalette["cyan+2"] = Color("#35b8bc")
	colorPalette["blue-4"] = Color("#000c30")
	colorPalette["blue-3"] = Color("#001a40")
	colorPalette["blue-2"] = Color("#102a50")
	colorPalette["blue-1"] = Color("#1a3864")
	colorPalette["blue"] = Color("#284d9c")
	colorPalette["blue+1"] = Color("#2e60ac")
	colorPalette["blue+2"] = Color("#316bc1")
	colorPalette["purple-4"] = Color("#280030")
	colorPalette["purple-3"] = Color("#300040")
	colorPalette["purple-2"] = Color("#401050")
	colorPalette["purple-1"] = Color("#502060")
	colorPalette["purple"] = Color("#7e2a93")
	colorPalette["purple+1"] = Color("#8e26b3")
	colorPalette["purple+2"] = Color("#9c31c1")
	colorPalette["red-4"] = Color("#300000")
	colorPalette["red-3"] = Color("#40000a")
	colorPalette["red-2"] = Color("#50101a")
	colorPalette["red-1"] = Color("#611e28")
	colorPalette["red"] = Color("#942638")
	colorPalette["red+1"] = Color("#b3263d")
	colorPalette["red+2"] = Color("#c13148")
	colorPalette["orange-4"] = Color("#301400")
	colorPalette["orange-3"] = Color("#402000")
	colorPalette["orange-2"] = Color("#53300c")
	colorPalette["orange-1"] = Color("#63401c")
	colorPalette["orange"] = Color("#a05926")
	colorPalette["orange+1"] = Color("#b36a26")
	colorPalette["orange+2"] = Color("#c9792a")
	colorPalette["yellow-4"] = Color("#302800")
	colorPalette["yellow-3"] = Color("#403600")
	colorPalette["yellow-2"] = Color("#5a4c05")
	colorPalette["yellow-1"] = Color("#6a5c15")
	colorPalette["yellow"] = Color("#a68c1f")
	colorPalette["yellow+1"] = Color("#ba9f1e")
	colorPalette["yellow+2"] = Color("#d8b41a")
	colorPalette["brown-4"] = Color("#241b0c")
	colorPalette["brown-3"] = Color("#302310")
	colorPalette["brown-2"] = Color("#403320")
	colorPalette["brown-1"] = Color("#504330")
	colorPalette["brown"] = Color("#7c6442")
	colorPalette["brown+1"] = Color("#8f724a")
	colorPalette["brown+2"] = Color("#9d7d54")
	pass

	
#func draw_circle_arc(center, radius, angle_from, angle_to, color):
#	var nb_points = 32
#	var points_arc = PoolVector2Array()
#
#	for i in range(nb_points + 1):
#		var angle_point = deg2rad(angle_from + i * (angle_to-angle_from) / nb_points - 90)
#		points_arc.push_back(center + Vector2(cos(angle_point), sin(angle_point)) * radius)
#
#	for index_point in range(nb_points):
#		draw_line(points_arc[index_point], points_arc[index_point + 1], color)
#
#func draw_pixel(pos, color):
#	var size = 16
#	var posX = pos.x# - size/2.0
#	var posY = pos.y# - size/2.0
#	var rectAtPos = Rect2(posX, posY, size, size)
#
#	draw_rect(rectAtPos,color)
#
#func drawPixelLine(from, to, color):
#	var x = from.x
#	var y = from.y
#
#	var coef = 0
#
#	while(coef <= 1):
#		draw_pixel(Vector2(floor(x/16)*16, floor(y/16)*16),color)
#
#		coef = coef + 0.01
#		x = from.x * (1-coef) + to.x * coef
#		y = from.y * (1-coef) + to.y * coef
#
#func _draw():
#	var center = Vector2(400, 400)
#	var colorBlack = Color(0.0, 0.0, 0.0)
#	var colorRed = Color(1.0, 0.0, 0.0)
#
#	var artArray = [[0,0,0,0,0],[0,1,0,1,0],[0,0,0,0,0],[1,0,0,0,1],[0,1,1,1,0]]
#
#	for i in range(0,artArray.size()):
#		for j in range(0,artArray[i].size()):
#			if artArray[j][i] == 1:
#				draw_pixel(center + Vector2(i*16, j*16), colorBlack)
#
#	draw_line(Vector2(120,0),Vector2(320,600),colorBlack,16)
#	drawPixelLine(Vector2(0,0),Vector2(200,600),colorBlack)
	
	#draw_pixel(center,color)