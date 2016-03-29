#include "Player.h"
#include "FSM.h"
#include "Table.h"
#include "LuaLoader.h"
#include "MyCamera.h"

Player::Player(Scene *_scene, Table* table, MyCamera* camera)
:Character(_scene, table)
{
	luabridge::getGlobalNamespace(L)
		//	.beginNamespace("player")
		.deriveClass<Player, Character>("Player")
		.addFunction("Load", &Player::Load)
		.endClass();
	//	.endNamespace();

	node = NULL;
	Player::camera = camera;
	Player::_scene = _scene;
}

void Player::Load(std::string file)
{
	if (node && file.compare(currentFile) == 0)
		return; // no change, just leave it

	if (node)
		Clear();
		
	

	LuaLoader loader(L);
	loader.setFile(file);

	if (ownedPoles.size() == 0)
		ownedPoles = table->getHandles(2);
	if (allPoles.size() == 0)
		allPoles = table->getHandles();

	std::string gpbFile = loader.getString("GPB_FILE");
	Bundle* characterBundle = Bundle::create(gpbFile.c_str());
	std::string id = loader.getString("ID");
	node = characterBundle->loadNode(id.c_str());

	std::string textureFile = loader.getString("CHARACTER_TEXTURE");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(textureFile.c_str(), true), TEXTURED_ANIMATED, false, 50), 0);
	std::string shirtFile = loader.getString("CHARACTER_SHIRT");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(shirtFile.c_str(), true), TEXTURED_ANIMATED, false, 50), 1);
	std::string eyeFile = loader.getString("EYE_NEUTRAL");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(eyeFile.c_str(), true), TEXTURED_ANIMATED, false, 50), 2);
	std::string mouthFile = loader.getString("MOUTH_NEUTRAL");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(mouthFile.c_str(), true), TEXTURED_ANIMATED, false, 50), 3);
	Animation* _animation = node->getAnimation("animations");
	_animation->createClips("res/animation.animation");

	setupAnimSetList(_animation, &loader);

//	_scene->addNode(node);
	characterBundle->release();

	node->rotateZ(MATH_DEG_TO_RAD(180));

	
}

void Player::Update(const float& elapsedTime)
{
	if (node == NULL)
		return;

	updateAnimation();
	checkForLuaResume();
	//float distance = (camera->getNode()->getTranslation() - node->getTranslation() + Vector3(0, 0, 12)).length();
	//if (camera->getNode()->getTranslation().y < node->getTranslation().y
	//	&& node->getScene() != NULL)
	//	_scene->removeNode(node);

	//else if (camera->getNode()->getTranslation().y >= node->getTranslation().y
	//	&& node->getScene() == NULL)
	//	_scene->addNode(node);

	

}

void Player::FadeToCharacter()
{
	_scene->addNode(node);
}