extends Node2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var shapeCounts = [["rock",2],["sand",3],['grass',7],['boreal',6],['snow',4],['range1', 2],['range2', 3]]

var biomeVisits = []

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _process(delta):
	if Input.is_action_just_pressed("ui_click"):
		
		var nearMountainRange = ""
		var resultBiomeIndex = 0
		
		
		var pos = get_global_mouse_position()
		print("Pos: " + str(pos))
		
		var space_state = get_world_2d().direct_space_state
		var result = space_state.intersect_point(pos, 32, [], 0x7FFFFFFF, true, true )
		print(result)
		if result.size() == 0:
			return
		elif result.size() == 2 and result[0].collider.name == 'MountainRanges':
			resultBiomeIndex=1
			nearMountainRange = 'BiomeMountain'
		elif result.size() == 2 and result[1].collider.name == 'MountainRanges':
			resultBiomeIndex=0
			nearMountainRange = 'BiomeMountain'
		elif result.size() ==1 and result[0].collider.name != 'MountainRanges':
			resultBiomeIndex=0
		else:
			return
		var index = result[resultBiomeIndex]['shape']
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
		var lastMountainRange = ""
		if biomeVisits.size() > 0:
			lastBiome = biomeVisits[biomeVisits.size()-1][0]
			lastMountainRange = biomeVisits[biomeVisits.size()-1][1]
			
		var medianMountainRange = ""
		if nearMountainRange != "" and lastMountainRange != "":
			medianMountainRange = 'BiomeMountain'
			
#		if lastBiome == coll.name:
#			return
		
		if lastBiome != "":
			if coll.name == "BiomeSnow":
				if lastBiome == 'BiomeGrass':
					biomeVisits.append(['BiomeBoreal', medianMountainRange])
				elif lastBiome != 'BiomeBoreal':
					biomeVisits.append(['BiomeGrass', medianMountainRange])
					biomeVisits.append(['BiomeBoreal', medianMountainRange])
			elif coll.name == 'BiomeBoreal':
				if lastBiome == 'BiomeRock' or lastBiome == 'BiomeDesert':
					biomeVisits.append(['BiomeGrass', medianMountainRange])
			elif coll.name == 'BiomeGrass':
				if lastBiome == 'BiomeSnow':
					biomeVisits.append(['BiomeBoreal', medianMountainRange])
			elif coll.name == 'BiomeRock' or coll.name == 'BiomeSand':
				if lastBiome == 'BiomeBoreal':
					biomeVisits.append(['BiomeGrass', medianMountainRange])
				elif lastBiome == 'BiomeSnow':
					biomeVisits.append(['BiomeBoreal', medianMountainRange])
					biomeVisits.append(['BiomeGrass', medianMountainRange])
		
		biomeVisits.append([coll.name, nearMountainRange])
		print(biomeVisits)