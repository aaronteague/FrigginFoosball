#pragma once

#include "Entity.h"

struct Tile;

class Block : public Entity{
public:
	
	/** Constructor
	@param loader pointer to the lua loader
	@param file the file with info for the block
	@param position where to place the block to be made
	@param firstTime indicates if we need to load just the block file or also the extras with it*/
	Block(LuaLoader *loader, std::string file, Vector2 position, bool firstTime = false);
	
	
	virtual void Update(const float& elapsedTime);
	virtual void Render();

	
	/** clears tileMap, barriers, and housePoints to save a little bit on memory*/
	void clearInitialExcess();

	/** returns the initial tile map*/
	std::vector<std::vector<Tile*>> getInitialTileMap();

	/** returns the initial house points*/
	std::vector<Vector2>& getInitialHousePoints();

	/** returns the initial barriers*/
	std::vector<Vector4> getInitialBarriers(){return barriers;}

	/** returns the character start points */
	std::vector<Vector3> getStartPoints(){ return startPoints; }
	
	/** set the positions*/
	virtual void SetPosition(float x, float y);

	/** get the position*/
	virtual const Vector2 &GetPosition(){ return position; }

	/** gets the node*/
	Node* GetNode(){ return node; }
protected:
	
	
private:
	std::string luaFile;
	std::string nodeF;
	std::vector<std::string> textureFiles;
	std::string nodeI;

	Vector2 position;

	Node* node;

	int blockTileLength;
	std::vector<std::vector<Tile*>> tileMap;

	std::vector<Vector4> barriers;
	std::vector<Vector2> housePoints;
	std::vector<Vector3> startPoints;

	std::string currentTextureFile;

	Block();
	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);
};