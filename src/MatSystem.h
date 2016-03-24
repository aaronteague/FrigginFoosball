#pragma once

#include "gameplay.h"
#include "Entity.h"

using namespace gameplay;

struct Mat{
	std::string file;
	SpriteBatch* _batch;
	Vector3 trans;
};

class MatSystem : public Entity{
public:
	/** constructor*/
	MatSystem(const int worldWidth, const int worldLength);

	/** adds a mat file in specified place.  replaces mat in place if one already exists in location*/
	void addMat(const std::string file, const Vector2 &pos);

	/** removes a mat file from location*/
	void removeMat(const Vector2& pos);

	/** updates the matsystem, organizing the rendering for efficiency.
	@param trans the vector to base the visibily on*/
	void Update(const Vector3& trans);

	/** draws the tiles organized from update function
	@param viewProjMatrix a matrix of the camera's offset so we can position tiles correctly*/
	void Render(const Matrix& viewProjMatrix);

	/** Destructor*/
	~MatSystem();
	

private:
	std::vector<std::string> texFiles;
	std::vector<std::string> texFilesToBuild;
	std::vector<std::vector<Mat>> matMap;
	std::vector<std::vector<Mat*>> renderList;
	std::vector<SpriteBatch*> spriteRenderList;
	std::vector<SpriteBatch*> batchList;
	std::vector<std::vector<Mat*>> buildList;
	unsigned int buildCounter;

	const int X_RANGE = 10;
	const int Y_RANGE = 10;
	Vector3 r;
	Vector3 faceUp;
	Vector4 color;

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	virtual void Update(const float& elapsedTime){} // not used
	virtual void Render(){} // not used
};