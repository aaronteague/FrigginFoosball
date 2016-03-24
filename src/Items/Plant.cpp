#include "Plant.h"
#include "../FSM.h"

#include "../PlayField.h"

#include "../States/PlantIdle.h"

Plant::Plant(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
	:Item(loader, luaFile, position, rotation, status, containedWorld)
{
	entityType = "Plant";

	icon = new ItemIcon(loader, this);

	name = loader->getString("NAME");

	harvestable = loader->getBool("HARVESTABLE");
	
	if (harvestable){
		counter = 1;
		luabridge::LuaRef attachTable = loader->getTable("ATTACH_POINTS");
		while (true){
			luabridge::LuaRef transTable = loader->getTable(counter, attachTable);
			if (transTable.isNil())
				break;

			float x = loader->getFloat(1, transTable);
			float y = loader->getFloat(2, transTable);
			float z = loader->getFloat(3, transTable);
			Vector3 trans(x, y, z);

			Attach point;
			point.trans = trans;
			point.crop = NULL;
			attachPoints.push_back(point);

			counter++;
		}
	}

	luabridge::LuaRef lifeTable = loader->getTable("STAGE");
	counter = 1;
	while (true){
		luabridge::LuaRef stageTable = loader->getTable(counter, lifeTable);
		if (stageTable.isNil())
			break;

		Stage stage;
		stage.file = loader->getString("file", stageTable);
		stage.textureF = loader->getString("texture", stageTable);
		stage.normalF = loader->getString("normal", stageTable);
		stage.id = loader->getString("id", stageTable);
		stage.jointCount = loader->getInt("jointCount", stageTable);
		if (stage.jointCount > 0)
			stage.animationFile = loader->getString("animation", stageTable);
		stage.radius = loader->getFloat("radius", stageTable);
		stage.dimensions.set(1, 1, -1);
		stage.dimensions.z = loader->getFloat("height", stageTable);
		stage.duration = loader->getInt("duration", stageTable);
		stageList.push_back(stage);

		counter++;
	}

	cropFile = loader->getString("CROP_FILE");

	// default values til they get changed when load happens
	age = 10;
	points = 0;
	currentAnimClip = NULL;
	counter = 0;

	interactOffset.set(0, 0);
	
	
	interactRange = 1;

	stateMachine->SetCurrentState(new PlantIdle());
}

// add conditions to set the plant down
bool Plant::SetPreviewPosition(float x, float y, PlayField* playField)
{
	if (playField->isFreeTile(x, y))
	{
		position.set(x, y);
		if (node != NULL){
			node->setTranslation((int)x + currentDimensions.x / 2, (int)y + currentDimensions.y / 2, 0);
			SetRotation(rotation);
		}
		return true;
	}else

		return false;
}

void Plant::ageUp(int &fertility, const int &water, Scene* _scene)
{
	// i think this function might be leaky

	int lastStage = calculateStage(age);

	age++;
	points += water;
	points += fertility;

	int newStage = calculateStage(age);

	if (newStage == stageList.size() - 1 && lastStage != newStage)
		fertility--;

	if (newStage == stageList.size() - 1)
		addCrop();

	if (node != NULL && lastStage != newStage){
		if (currentAnimClip != NULL){
			currentAnimClip->stop();
			SAFE_RELEASE(currentAnimClip);
		}
		_scene->removeNode(node);
		defaultDimensions = stageList[newStage].dimensions;
		node = NULL;
		prepSelf();  // should this be "AddToEntityList"?
		counter = 0;
	}
}

void Plant::prepSelf()
{
	counter = 0;

	int stageNum = calculateStage(age);

	defaultDimensions = stageList[stageNum].dimensions;

	radius = stageList[stageNum].radius;
	if (radius <= 0)
		shape = PhysicShape::NONE;
	else
		shape = PhysicShape::CIRCLE;

	sendToLists(stageList[stageNum].textureF, stageList[stageNum].file, stageList[stageNum].id, this);
	sendToLists(stageList[stageNum].normalF, "none", "none", this);

	if (shape == PhysicShape::CIRCLE && (status == Status::ABSTRACT || status == Status::SOLID))
		buildPhys(stageList[stageNum].radius);
	
}

void Plant::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	 
	int stageNum;
	switch (counter){
	case 0:
		stageNum = calculateStage(age);
		if (stageList[stageNum].jointCount > 0){
			Item::node = nodeToScene(node, tex, _scene, TEXTURED_ANIMATED_NORMAL, true, stageList[stageNum].jointCount);
			// test out the animation here

			Animation* _animation = Item::node->getAnimation("animations");
			_animation->createClips(stageList[stageNum].animationFile.c_str());
			
			animations.resize(NUM_OF_ANIMATIONS);
			animations[Animation_LIST::IDLE] = _animation->getClip("Idle");
			if (stageNum == stageList.size() - 1)
				animations[Animation_LIST::ACTION] = _animation->getClip("Action");
			currentAnimClip = animations[IDLE];
			currentAnimClip->play();

			SAFE_RELEASE(_animation);

		}
		else
			Item::node = nodeToScene(node, tex, _scene, TEXTURED_NORMAL, true);


		pointTowards.set(Item::node->getTranslationX(), Item::node->getTranslationY());

		break;
	case 1:
		applyNormalMap(dynamic_cast<Model*>(Item::node->getDrawable())->getMaterial(), tex);
		if (status == ABSTRACT || status == SOLID){
			//SetPosition(position.x, position.y);
			Item::node->setTranslation(position.x + 0.5f, position.y + 0.5f, 0);
			SetRotation(rotation);
			status = SOLID; // finished
		}
		break;
	}
	counter++;
}

int Plant::calculateStage(int age){
	int cummulativeDuration = 0;
	for (unsigned int i = 0; i < stageList.size(); ++i){
		cummulativeDuration += stageList[i].duration;
		if (age < cummulativeDuration)
			return i;
	}
	return stageList.size() - 1;
}



bool Plant::addCrop(){
	// look for an available spot to put the crop
	int index = -1;
	for (unsigned int i = 0; i < attachPoints.size(); ++i)
	if (attachPoints[i].crop == NULL){
		index = i;
		break;
	}

	// if there isn't a spot available, return false
	if (index == -1)
		return false;

	// might need to make this added from age-up function parameter
	LuaLoader* loader = new LuaLoader(L);

	// add crop to list to be built
	Item* crop = Item::Create(loader, cropFile, Vector2(-1, -1), Rotation::SOUTH, Status::FREE_BOUND, physWorld);
	crop->AddToEntityList();
	attachPoints[index].crop = crop;

	delete(loader);

	return true;
}

void Plant::Update(const float& elapsedTime)
{
	// a stupid work around
	stateMachine->Update((float&)elapsedTime, true);

	for (unsigned int i = 0; i < attachPoints.size(); ++i)
	if (attachPoints[i].crop != NULL)
		attachPoints[i].crop->GetNode()->setTranslation(attachPoints[i].trans + node->getTranslation());

}


void Plant::Render()
{
	

	for (unsigned int i = 0; i < attachPoints.size(); ++i)
	if (attachPoints[i].crop != NULL)
		attachPoints[i].crop->Render();

	node->getDrawable()->draw();
}