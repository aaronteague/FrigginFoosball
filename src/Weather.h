#pragma once

#include "gameplay.h"
#include "Entity.h"

class TimeAndDate;
class MyCamera;
struct Tile;

using namespace gameplay;

const int NUM_OF_SPLATS = 300;
const int NUM_OF_DROPS = 400;
//const int NUM_OF_POSSIBLE_POS = 5000;

struct WaterDrop{
	Vector3 trans;
	Vector3 speedAndDirection;
	float angleRad;
};

struct WaterSplat{
	Vector3 trans;
	float phase;
	
};

struct Cloud{
	Vector3 trans;
	Vector3 adjTrans;
	gameplay::Vector4 srcCoords;
	float size;
	float speed;
};

class Weather : public Entity{
public:
	Weather(int currentWeather, int windInfluence, Vector2 screenSize, Vector2 worldSize, int state = RAIN);

	virtual void Update(const float& elapsedTime, const Vector3& trans);
//	void Render(const Matrix &viewProjection);
	void RenderClouds(const Matrix &viewProjection);
	void RenderPrecipitation(const Matrix &viewProjection);

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	luabridge::LuaRef save();
	void load(luabridge::LuaRef weatherRef);

	void updateLight(Scene* _scene, TimeAndDate* time);
//	void updateWetAreas(std::vector<std::vector<Tile*>> tileMap);

	enum WeatherOptions{
		SUNNY,
		RAIN,
		SNOW
	};
	

private:
	std::vector<gameplay::Vector4> cloudsSrcCoord;

	const float WATER_DROP_SPEED = 0.03f;
	const int WATER_DROP_ZONE = 25;
	const int WATER_SPLAT_ZONE = 15;
	const float DROP_ANGLE_SWAY = 0.01f;
	const int RAIN_HEIGHT = 20;
	const float SPEED = 0.6f;
	const float SPLATS_PHASE_SPEED = 0.01f;
	const int NUM_OF_CLOUDS = 5;
	const float CLOUD_Y_OFFSET = 50;
	const float CLOUD_Z_OFFSET = -35;
	const float CLOUD_VERTICAL_RANGE = 80;
	const float CLOUD_HORIZONTAL_RANGE = 50;
	const float CLOUD_BASE_SPEED = -0.01f;


	const float RANGE = 10;
	const float Y_OFFSET = -5;

	std::string textureF;
	Vector2 worldSize;

	Vector4 lightColor;
	Node* rainNode;

	int counter;
	int currentWeather;
	int windInfluence;
	bool rainInitialized;
	std::vector<PhysicsCollisionObject*> collidable;

	std::vector<Cloud> clouds;

	WaterDrop rainDrops[NUM_OF_DROPS];
	std::vector<Node*> rainSplats;
//	Vector3 possibleSplatPositions[NUM_OF_POSSIBLE_POS];

	SpriteBatch* rainBatch;
	SpriteBatch* cloudBatch;
	gameplay::Rectangle rainDropImg;
	gameplay::Rectangle rainDropBox;
	gameplay::Rectangle rainSplatBox;

	ParticleEmitter* rainEmitter;

	void resetDrop(WaterDrop &drop, const Vector3& trans, bool randHeight);
	void resetCloud(Cloud &cloud);

//	bool getCollisions(Node* node);

//	virtual void prepSelf();
//	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	Weather(){} // won't use the default constructor
	virtual void Update(const float& elapsedTime){}  // not using this one

	virtual void Render(){}  // not using this version

	bool updateMaterial(Node* node);
};