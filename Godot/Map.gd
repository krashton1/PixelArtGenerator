extends Node2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var shapeCounts = [["rock",2],["sand",3],['grass',7],['boreal',6],['snow',4],['range1', 2],['range2', 3]]
var mountainRanges = []

var biomeVisits = []

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _process(delta):
	if Input.is_action_just_pressed("ui_click"):
		
		var pos = get_global_mouse_position()
		print("Pos: " + str(pos))
		
		var space_state = get_world_2d().direct_space_state
		var result = space_state.intersect_point(pos, 32, [], 0x7FFFFFFF, true, true )
		if result.size() == 0:
			return
		var index = result[0]['shape']
#		print("Index: " + str(index))
		
		var shiftIndex = -1
		for i in range(shapeCounts.size()):
			index -= shapeCounts[i][1]
			if index < 0:
				shiftIndex = i
				break
#		print("ShapeIndex: " + str(shiftIndex))
		
		var area_shape = get_node("Biomes")
		var coll = area_shape.get_child(shiftIndex)
		print("Biome: " + coll.name)
		
		
		var pin = Sprite.new()
		pin.texture = load("res://Assets/pin.png")
		pin.set_name("Pin" + str(biomeVisits.size()))
		pin.position = pos + Vector2(0,-24)
		add_child(pin)
		
		var lastBiome = ""
		if biomeVisits.size() > 0:
			lastBiome = biomeVisits[biomeVisits.size()-1]
			
		if lastBiome == coll.name:
			return
		
		if lastBiome != "":
			if coll.name == "BiomeSnow":
				if lastBiome == 'BiomeGrass':
					biomeVisits.append('BiomeBoreal')
				elif lastBiome != 'BiomeBoreal':
					biomeVisits.append('BiomeGrass')
					biomeVisits.append('BiomeBoreal')
			elif coll.name == 'BiomeBoreal':
				if lastBiome == 'BiomeRock' or lastBiome == 'BiomeDesert':
					biomeVisits.append('BiomeGrass')
			elif coll.name == 'BiomeGrass':
				if lastBiome == 'BiomeSnow':
					biomeVisits.append('BiomeBoreal')
			elif coll.name == 'BiomeRock' or coll.name == 'BiomeSand':
				if lastBiome == 'BiomeBoreal':
					biomeVisits.append('BiomeGrass')
				elif lastBiome == 'BiomeSnow':
					biomeVisits.append('BiomeBoreal')
					biomeVisits.append('BiomeGrass')
		
		biomeVisits.append(coll.name)
		print(biomeVisits)


func loadingIconVisible():
	var icon = get_node("LoadingIcon")
	icon.visible = true