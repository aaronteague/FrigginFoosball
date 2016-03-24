#include "Weather.h"
#include <math.h>
#include "Menus\TimeAndDate.h"
#include "Tile.h"

Weather::Weather(int currentWeather, int windInfluence, Vector2 screenSize, Vector2 worldSize, int state)
	:windInfluence(windInfluence)
{
	Weather::worldSize = worldSize;
	textureF = "res\rain.png";

//	cloudsSrcCoord.push_back(Vector4(0, 0, 0.28f, 0.25f));
//	cloudsSrcCoord.push_back(Vector4(0.5f, 0.0f, 1, 0.4f));
	cloudsSrcCoord.push_back(Vector4(0,0,1,1));

	rainDropImg.set(61, 9, 10, 210);
	rainDropBox.set(rainDropImg);

	//for (int i = 0; i < NUM_OF_SPLATS; i++)
	//	rainSplats[i].phase = MATH_RANDOM_0_1() * 10;
	
	// set up the clouds
	clouds.resize(NUM_OF_CLOUDS);
	for (int i = 0; i < NUM_OF_CLOUDS; ++i){
		clouds[i].size = MATH_RANDOM_0_1() * 40 + 60;
		clouds[i].trans.x = MATH_RANDOM_0_1() * CLOUD_HORIZONTAL_RANGE * 2 - CLOUD_HORIZONTAL_RANGE;
		clouds[i].trans.y = MATH_RANDOM_0_1() * CLOUD_VERTICAL_RANGE * 2 - CLOUD_VERTICAL_RANGE + CLOUD_Y_OFFSET;
		clouds[i].trans.z = i + CLOUD_Z_OFFSET - NUM_OF_CLOUDS;
		clouds[i].speed = CLOUD_BASE_SPEED * sqrt(i);
		clouds[i].srcCoords.set(cloudsSrcCoord[MATH_RANDOM_0_1() * cloudsSrcCoord.size()]);
	}
	rainSplats.resize(NUM_OF_SPLATS);
	Weather::currentWeather = state;
	rainInitialized = false;
	counter = 0;
	lightColor.set(Vector4::one());
	rainEmitter = NULL;
}

void Weather::Update(const float& elapsedTime, const Vector3& trans)
{
//	const int SPLAT_DURATION = 10;
	//int rangeMin = NUM_OF_SPLATS / SPLAT_DURATION;
	//int rangeMax = NUM_OF_SPLATS / SPLAT_DURATION + NUM_OF_SPLATS / SPLAT_DURATION;

	//for (int i = rangeMin; i < rangeMax; ++i){
	//	int index = MATH_RANDOM_0_1() * NUM_OF_POSSIBLE_POS;

	//}
	
	for (int i = 0; i < NUM_OF_CLOUDS; ++i){
		clouds[i].trans.x += clouds[i].speed;
		if (clouds[i].trans.x < CLOUD_HORIZONTAL_RANGE * -1)
			resetCloud(clouds[i]);
		clouds[i].adjTrans = clouds[i].trans + trans;
	}

	// make it rain if that is the weather
//	rainEmitter->setPosition(trans + Vector3(0, 30, -50), Vector3(0,0,0));

	//if (currentWeather == RAIN){
	//	if (!rainInitialized){
	//		for (int i = 0; i < NUM_OF_DROPS; ++i){
	//			resetDrop(rainDrops[i], trans, true);
	//		}
	//		rainInitialized = true;
	//	}
	//	const int DROP_RESET_THRESHOLD = 10;
	//	for (int i = 0; i < NUM_OF_DROPS; ++i){
	//		if (rainDrops[i].trans.z < DROP_RESET_THRESHOLD)
	//			resetDrop(rainDrops[i], trans, false);
	//		rainDrops[i].trans.x += rainDrops[i].speedAndDirection.x;
	//		// we don't need to move on the y-axis
	//		rainDrops[i].trans.z -= rainDrops[i].speedAndDirection.z;
	//	}


	//}



	rainEmitter->update(elapsedTime);

	//if (counter >= NUM_OF_SPLATS)
	//	counter = 0;
	//int x = MATH_RANDOM_0_1() * worldSize.x;
	//int y = MATH_RANDOM_0_1() * worldSize.y;
	//rainSplats[counter]->setTranslation(x, y, 0);
//	counter++;
}

void Weather::resetCloud(Cloud &cloud)
{
	cloud.trans.y = MATH_RANDOM_0_1() * CLOUD_VERTICAL_RANGE * 2 - CLOUD_VERTICAL_RANGE + CLOUD_Y_OFFSET;
	cloud.trans.x = CLOUD_HORIZONTAL_RANGE;
	cloud.size = MATH_RANDOM_0_1() * 40 + 60;
}


void Weather::resetDrop(WaterDrop &drop, const Vector3& trans, bool randHeight)
{

	float x, y, z, angleRad;
	// build angle with just little variation
	angleRad = MATH_DEG_TO_RAD(MATH_RANDOM_0_1() * 10 - 5 + windInfluence);
	drop.speedAndDirection.x = sin(angleRad) * SPEED;
	drop.speedAndDirection.z = cos(angleRad) * SPEED;
	if (randHeight)
		// for a lot of variation
		z = MATH_RANDOM_0_1() * RAIN_HEIGHT;
	else
		// for just a little variation
		z = RAIN_HEIGHT + MATH_RANDOM_0_1() * RAIN_HEIGHT / 10;

	float xOffset = z * tan(MATH_DEG_TO_RAD(windInfluence));
	x = MATH_RANDOM_0_1() * RANGE * 2 + trans.x - RANGE - xOffset;
	y = MATH_RANDOM_0_1() * RANGE * 2 + trans.y - RANGE + Y_OFFSET;
	drop.trans.set(x, y, z);
	drop.angleRad = angleRad;
}

void Weather::prepSelf()
{
	// make the clouds
	sendToLists("res/Weather/Clouds2.png", "none", "none", this);

	// make rain
//	sendToLists("res/Weather/splat.png", "none", "none", this);
}
void Weather::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (counter == 0){ // clouds first
		cloudBatch = SpriteBatch::create(tex);
		cloudBatch->getStateBlock()->setDepthTest(true);
		
		if (currentWeather != SUNNY){
			if (currentWeather == RAIN)
				rainEmitter = ParticleEmitter::create("res/Weather/rain.particle");
			else if (currentWeather == SNOW)
				rainEmitter = ParticleEmitter::create("res/Weather/snow.particle");

			_scene->getActiveCamera()->getNode()->setDrawable(rainEmitter);
		//	_scene->getActiveCamera()->getNode()->setParticleEmitter(rainEmitter);
			rainEmitter->start();
		}
	}
	if (counter == 1){ // rain
		// splat
		Texture::Sampler *sampler = Texture::Sampler::create(tex);
		Effect* effect = Effect::createFromFile("res/shaders/textured.vert", "res/shaders/textured.frag", "MODULATE_ALPHA");
		
		Mesh* mesh = Mesh::createQuad(0, 0, 1, 1, 0, 0, 0.5f, 0.5f);
		
		for (int i = 0; i < NUM_OF_SPLATS; ++i){
			Node* node = Node::create();
			node->rotateX(MATH_DEG_TO_RAD(90));
			Model* model = Model::create(mesh);
			Material* material = Material::create(effect);
			material->getStateBlock()->setDepthTest(true);
			material->getStateBlock()->setBlend(false);
			material->getParameter("u_diffuseTexture")->setSampler(sampler);
			material->setParameterAutoBinding("u_worldMatrix", RenderState::WORLD_MATRIX);
			material->getParameter("u_clipPlane")->setValue(Vector4(1, -1, 0, 7));
			material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
			model->setMaterial(material);

			node->setDrawable(model);
			node->setDrawable(model);
			_scene->addNode(node);

			rainSplats[i] = node;

			SAFE_RELEASE(model);
			SAFE_RELEASE(material);
		}
		SAFE_RELEASE(effect);
		SAFE_RELEASE(mesh);

		// rain

		//rainEmitter = ParticleEmitter::create("res/fire.particle");

		//rainNode = Node::create("rain");
		//rainNode->setParticleEmitter(rainEmitter);
		//_scene->addNode(rainNode);

		//rainNode->setTranslation(95, 96, 10);
		//rainEmitter->start();

		counter = 0;
	}
	counter++;

}



void Weather::RenderClouds(const Matrix &viewProjection)
{
	Vector3 r(1, 0, 0);
	Vector3 faceUp(0, 1, 0);

	// let's make clouds first
	cloudBatch->start();
	cloudBatch->setProjectionMatrix(viewProjection);
	for (int i = 0; i < NUM_OF_CLOUDS; ++i){
		Vector4 coord = clouds[i].srcCoords;
		cloudBatch->draw(clouds[i].adjTrans, r, faceUp, clouds[i].size, clouds[i].size, coord.x, coord.y, coord.z, coord.w, lightColor, Vector2(0.5f, 0.5f), 0);
	}
	cloudBatch->finish();

	//// next, we will make it rain!
	//if (currentWeather == RAIN){
	//	rainBatch->start();
	//	rainBatch->setProjectionMatrix(viewProjection);

	//	Vector3 t(0, 0, 0.1f);
	//	//Vector3 r(1, 0, 0);
	//	Vector3 faceSouth(0, 0, 1);

	//	Vector4 color(0.9f, 0.9f, 0.9f, 0.9f);

	//	faceSouth.normalize();
	//	faceUp.normalize();
	//	r.normalize();

	//	for (int i = 0; i < NUM_OF_DROPS; i++){
	//		rainBatch->draw(rainDrops[i].trans, r, faceSouth
	//			, 0.5f, 1.4f, 0.2f, 0.15f, 0.4f, 0.9f
	//			, color, Vector2(0.5f, 0.5f)
	//			, rainDrops[i].angleRad);


	//	}
	//	rainBatch->finish();
	//}


}
void Weather::RenderPrecipitation(const Matrix &viewProjection)
{
//	for (int i = 0; i < NUM_OF_SPLATS; ++i)
//		rainSplats[i]->getModel()->draw();
	if (rainEmitter != NULL)
		rainEmitter->draw();
}


luabridge::LuaRef Weather::save(){
	luabridge::LuaRef weatherTable = luabridge::newTable(L);
	weatherTable["lastWeather"] = currentWeather;
	weatherTable["windInfluence"] = windInfluence;
	return weatherTable;
}

void Weather::load(luabridge::LuaRef weatherRef)
{
	currentWeather = weatherRef["lastWeather"];
	windInfluence = weatherRef["windInfluence"];
}

void Weather::updateLight(Scene* _scene, TimeAndDate* time){
	Light* light = _scene->findNode("light")->getLight();
	int hour = time->getHour();
	switch (hour){
	case 0:
	case 1:
	case 3:
	case 4:
	case 5:
		
		light->setColor(0.3f, 0.3f, 0.3f);
		break;
	case 6:
		light->setColor(0.5f, 0.4f, 0.4f);
		break;
	case 7:
		light->setColor(0.6f, 0.7f, 0.7f);
		break;
	case 8:
		light->setColor(0.9f, 0.9f, 0.9f);
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		light->setColor(1, 1, 1);
		break;
	case 19:
		light->setColor(0.9f, 0.9f, 0.9f);
		break;
	case 20:
		light->setColor(0.9f, 0.7f, 0.7f);
		break;
	case 21:
		light->setColor(0.7f, 0.5f, 0.5f);
		break;
	case 22:
		light->setColor(0.4f, 0.4f, 0.4f);
		break;
	case 23:
		light->setColor(0.3f, 0.3f, 0.3f);
		break;
	default:
		light->setColor(Vector3::one());
	}
	
	Vector3 c = light->getColor();
	lightColor.set(c.x, c.y, c.z, 1);

	//run through scene and update all materials
	_scene->visit(this, &Weather::updateMaterial);
	

	
}

bool Weather::updateMaterial(Node* node){
	if (dynamic_cast<Model*>(node->getDrawable()) != NULL){
		Vector3 c(lightColor.x, lightColor.y, lightColor.z);
		int numOfParts = dynamic_cast<Model*>(node->getDrawable())->getMeshPartCount();
		if (numOfParts == 1)
			dynamic_cast<Model*>(node->getDrawable())->getMaterial(-1)->getParameter("u_directionalLightColor[0]")->setValue(c);
		else
		for (int i = 0; i < numOfParts; ++i)
		for (int i = 0; i < 1; ++i)
			dynamic_cast<Model*>(node->getDrawable())->getMaterial(i)->getParameter("u_directionalLightColor[0]")->setValue(c);
	}
	return true;
}

//void Weather::updateWetAreas(std::vector<std::vector<Tile*>> tileMap){
//	float width = tileMap.size();
//	float length = tileMap[0].size();
//	for (int i = 0; i < NUM_OF_POSSIBLE_POS; ++i)
//	{
//		while (true){
//			float x = MATH_RANDOM_0_1() * width;
//			float y = MATH_RANDOM_0_1() * length;
//			
//			if (tileMap[x][y]->type == Tile::EMPTY){
//				possibleSplatPositions[i].set(x, y, 0);
//				break;
//			}
//		}
//	}
//
//
//}

