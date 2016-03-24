#pragma once
#include "../Item.h"


class Plant : public Item{
	friend class Item; /**<@ref*/
	friend class PlantIdle; /**<@ref*/
public:
	/** ages up the item by one day and changes it based on fertility and water in tile  It would either grow, decay, or stay the same*/
	void ageUp(int &fertility, const int &water, Scene* _scene);

	/** sets up the position of the preview version of the item  Uses logic of what's around it to determine if it could be placed in desired location
	@return if able to be set*/
	bool SetPreviewPosition(float x, float y, PlayField* playField);

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	/** updates the logic of this item.  only need to run this if it is visible  otherwise, it is a waste of CPU*/
	virtual void Update(const float& elapsedTime);

	/** draws this object, only need to call this if it is visible range*/
	virtual void Render();
protected:
	/** constructor.  Used only by the "Create" function of "item"*/
	Plant(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld);
private:
	struct Stage{
		std::string file;
		std::string textureF;
		std::string normalF;
		std::string id;
		Vector3 dimensions;
		int jointCount;
		std::string animationFile;
		float radius;
		int duration;
	};
	struct Attach{
		Item* crop;
		Vector3 trans;
	};
	const int NUM_OF_ANIMATIONS = 2;
	enum Animation_LIST{
		IDLE,
		ACTION
	};

	// various stats of the plant
	int age;
	int points;
	int seedCost;
	int returnStageAfterHarvest;
	std::string cropFile;

	int counter;
	bool harvestable;
	std::vector<Attach> attachPoints;

	std::vector<Stage> stageList;

	int calculateStage(int age);
	bool addCrop();
	
	Plant(){} // not using this
};