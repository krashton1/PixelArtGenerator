

#include "Main.h"
#include <windows.h>
#include <OS.hpp>
#include <ResourceLoader.hpp>
#include <ResourceInteractiveLoader.hpp>
#include <PackedScene.hpp>
#include <cassert>  
#include <time.h>
#include <Input.hpp>
#include <string>

namespace godot
{

Main::Main()
{
	// Wait for Debugger
	if (OS::get_singleton()->get_cmdline_args().size() > 0 && OS::get_singleton()->get_cmdline_args()[0] == String("-waitForDebug"))
	{
		while (!IsDebuggerPresent())
			Sleep(100);
	}

	// Init rand
	srand(time(NULL));
}

Main::~Main()
{

}

void Main::_register_methods()
{
	register_method((char*)"_ready", &_ready);
	register_method((char*)"_draw", &_draw);
	register_method((char*)"_init", &_init);
	register_method((char*)"_process", &_process);
}

void Main::_init()
{
	Ref<PackedScene> artGenScene = ResourceLoader::get_singleton()->load("res://AssetGenerator.tscn");
	Ref<PackedScene> rockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");
	Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");

	Ref<PackedScene> mapGenScene = ResourceLoader::get_singleton()->load("res://Map.tscn");


	Node2D* mapScene = Object::cast_to<Node2D>(mapGenScene->instance());
	mMapScene = mapScene;
	add_child(mapScene);


	// ---------------------------------------- Art ---------------------------------------- //

	//AssetGenerator* smileGen = Object::cast_to<AssetGenerator>(artGenScene->instance());
	//smileGen->addSmile();
	////smileGen->addMountain(40, new Color(0, 0, 0), new Color(1, 0, 0));
	//smileGen->set_position(Vector2(700, 0));
	//smileGen->apply_scale(Vector2(0.2, 0.2));
	//add_child(smileGen, "smileGen");
	//mGenerators.push_back(smileGen);


	// ---------------------------------------- Rocks ---------------------------------------- //

	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());
	//		
	//		if (i % 3 == 0)
	//			rockGen->setType(RockGenerator::RockTypeGraphite);
	//		else if (i % 3 == 1)
	//			rockGen->setType(RockGenerator::RockTypeBush);
	//		else if (i % 3 == 2)
	//			rockGen->setType(RockGenerator::RockTypeCloud);

	//		//rockGen->addMountain(20, new Color(0.25, 0.25, 0.25), new Color(0.4, 0.4, 0.5), new Color(0.5, 0.5, 0.6));


	//		rockGen->apply_scale(Vector2(0.2, 0.2));
	//		rockGen->set_position(Vector2(i * 200, j * 200));
	//		add_child(rockGen, "rockGen" + i + j);
	//		mGenerators.push_back(rockGen);
	//	}
	//}


	// ---------------------------------------- Trees ---------------------------------------- //
	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());

	//		if(j==0)
	//			treeGen->setType(TreeGenerator::TreeTypeDeciduous);
	//		else if(j==1)
	//			treeGen->setType(TreeGenerator::TreeTypeSnowConiferous);
	//		else if(j==2)
	//			treeGen->setType(TreeGenerator::TreeTypeCactus);

	//		treeGen->apply_scale(Vector2(0.2, 0.2));
	//		treeGen->set_position(Vector2(i * 200, j * 200));
	//		add_child(treeGen, "treeGen" + i + j);
	//		mGenerators.push_back(treeGen);
	//	}
	//}

	//Ref<PackedScene> sceneGenScene = ResourceLoader::get_singleton()->load("res://SceneGenerator.tscn");
	//SceneGenerator* sceneGen = Object::cast_to<SceneGenerator>(sceneGenScene->instance());
	//add_child(sceneGen, "sceneGen");
	//mSceneGenerator = sceneGen;

}

void Main::_ready()
{

}

void Main::_draw()
{

}

void Main::_process()
{
	float lastTime = mTime;
	float currentTime = float(clock());
	float deltaTime = (currentTime - lastTime) / 1000.0;
	mTime = currentTime;

	if (mIsLoading)
	{
		if (mLoader->poll() == Error::ERR_FILE_EOF)
			initSceneGenerator(mLoader);
	}

	if (Input::get_singleton()->is_action_just_pressed("ui_right"))
	{
		if (mPan < 20)
			mPan++;
	}

	if (Input::get_singleton()->is_action_just_pressed("ui_left"))
	{
		if(mPan>0)
			mPan--;
	}

	if (Input::get_singleton()->is_action_just_pressed("ui_accept") && mSceneGenerator == nullptr)
	{

		if (Array(mMapScene->get("biomeVisits")).size() >= 1)
		{

			Node* loadingIcon = mMapScene->get_node("LoadingIcon");
			loadingIcon->set("visible", true);
			
			if (!mIsLoading)
			{
				mLoader = ResourceLoader::get_singleton()->load_interactive("res://SceneGenerator.tscn");
				mIsLoading = true;
			}
		}
	}

	if(mPan)
	{
		mSceneGenerator->updateDistance(mPan* deltaTime);
		mSceneGenerator->update();

	}

	if (mSceneGenerator != nullptr && mSceneGenerator->getDistance() >=  1.5 * mCurrentBiomeIndex)
	{

		//Array visits = mMapScene->get("biomeVisits");
		if (mCurrentBiomeIndex + 1 < mVisits.size())
		{
			//std::string biome0 = String(visits[mCurrentBiomeIndex]).utf8().get_data();
			//std::string biome1 = String(visits[mCurrentBiomeIndex + 1]).utf8().get_data();

			mSceneGenerator->setBiomes(String(Array(mVisits[mCurrentBiomeIndex])[0]), String(Array(mVisits[mCurrentBiomeIndex + 1])[0]), String(Array(mVisits[mCurrentBiomeIndex])[1]), String(Array(mVisits[mCurrentBiomeIndex + 1])[1]));

		}
		mCurrentBiomeIndex += 1;
		
	}

}

void Main::initSceneGenerator(Ref<ResourceInteractiveLoader> loader)
{


	Ref<PackedScene> sceneGenScene = loader->get_resource();
	SceneGenerator* sceneGen = Object::cast_to<SceneGenerator>(sceneGenScene->instance());
	add_child(sceneGen, "sceneGen");
	mSceneGenerator = sceneGen;
	mIsLoading = false;

	mCurrentBiomeIndex = 0;

	mVisits.clear();
	Array visits = mMapScene->get("biomeVisits");
	for (int x = 0; x < visits.size(); x++)
	{
		mVisits.append(visits[x]);
	}

	if (mCurrentBiomeIndex + 1 < mVisits.size())
	{
		//std::string biome0 = String(visits[mCurrentBiomeIndex]).utf8().get_data();
		//std::string biome1 = String(visits[mCurrentBiomeIndex + 1]).utf8().get_data();

		std::string var = String(Array(mVisits[mCurrentBiomeIndex])[0]).utf8().get_data();;

		mSceneGenerator->setBiomes(String(Array(mVisits[mCurrentBiomeIndex])[0]), String(Array(mVisits[mCurrentBiomeIndex + 1])[0]), String(Array(mVisits[mCurrentBiomeIndex])[1]), String(Array(mVisits[mCurrentBiomeIndex + 1])[1]));

	}
	else if(mCurrentBiomeIndex + 1 == mVisits.size())
	{
		//std::string biome0 = String(mVisits[mCurrentBiomeIndex]).utf8().get_data();
		mSceneGenerator->setBiomes(String(Array(mVisits[mCurrentBiomeIndex])[0]), String(Array(mVisits[mCurrentBiomeIndex])[0]), String(Array(mVisits[mCurrentBiomeIndex])[1]), String(Array(mVisits[mCurrentBiomeIndex])[1]));
	}
	else
	{
		return;
	}
	mSceneGenerator->setup();

	mCurrentBiomeIndex += 1;
}

}