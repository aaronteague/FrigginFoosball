#include "Item.h"
#include "Box2D\Box2D.h"
#include "PlayField.h"

#include "Items\Plant.h"
#include "Items\House.h"
#include "Items\Crop.h"
#include "Items\Tool.h"

ItemIcon::ItemIcon(LuaLoader* loader, Item* item)
{
	ItemIcon::item = item;
	texture = loader->getString("ICON_TEXTURE_FILE");
//	int iconX = loader->getInt("IMG_ICON_POS_X");
//	int iconY = loader->getInt("IMG_ICON_POS_Y");
	int iconWidth = loader->getInt("IMG_ICON_WIDTH");
	int iconHeight = loader->getInt("IMG_ICON_HEIGHT");
	imgRec.set(0, 0, iconWidth, iconHeight);

	rec.set(0, 0, 45, 45);
	visible = true;
	preview = false;
	
}



Item* Item::Create(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
{
	loader->setFile(luaFile);

	std::string itemType = loader->getString("TYPE");

	Item* temp = NULL;

	if (itemType.compare("Plant") == 0)
		temp = new Plant(loader, luaFile, position, rotation, status, containedWorld);

	else if (itemType.compare("House") == 0)
		temp = new House(loader, luaFile, position, rotation, status, containedWorld);

	else if (itemType.compare("Crop") == 0)
		temp = new Crop(loader, luaFile, position, rotation, status, containedWorld);
	else if (itemType.compare("Tool") == 0)
		temp = new Tool(loader, luaFile, position, rotation, status, containedWorld);

	return temp;
}

Item::Item(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
:position(position), physWorld(containedWorld), rotation(rotation)
, radius(-1), physBody(NULL), node(NULL), status(status), luaFile(luaFile)
{
	//Item::position = position;
	//Item::physWorld = containedWorld;
	//Item::rotation = rotation;

	//radius = -1;
	//physBody = NULL;
	//node = NULL;
	//Item::status = status;
	//Item::luaFile = luaFile;

	


//	textureF = loader->getString("NODE_TEXTURE");

	defaultDimensions.x = loader->getInt("MODEL_WIDTH");
	defaultDimensions.y = loader->getInt("MODEL_LENGTH");
	defaultDimensions.z = loader->getInt("MODEL_HEIGHT");
	fixDimensions(rotation);

	//std::string bodyType = loader->getString("BODY_TYPE");
	//
	//	if (bodyType.compare("CIRCLE") == 0){
	//		shape = PhysicShape::CIRCLE;
	//	}
	//	else if (bodyType.compare("BOX") == 0){
	//		shape = PhysicShape::BOX;
	//	}
		icon = new ItemIcon(loader, this);
}

Item::~Item()
{
	if (node != NULL){
		if (node->getScene() != NULL){
			Scene* _scene = node->getScene();
			_scene->removeNode(node);
		}
		node->release();
		
	}
	if (physWorld != NULL && physBody != NULL){
		physWorld->DestroyBody(physBody);
		
	}
}

void Item::buildPhys(float radius)
{
	if (physBody != NULL)
		physWorld->DestroyBody(physBody);

	b2BodyDef bodyDef;
	b2FixtureDef fixDef;
	bodyDef.type = b2_staticBody;
	b2CircleShape shape;
	shape.m_p.Set(position.x + currentDimensions.x / 2, position.y + currentDimensions.y / 2);
	shape.m_radius = radius;
	fixDef.shape = &shape;

	physBody = physWorld->CreateBody(&bodyDef);
	physBody->CreateFixture(&fixDef);
}
void Item::buildPhys(int width, int length)
{
	if (physBody != NULL)
		physWorld->DestroyBody(physBody);

	b2BodyDef bodyDef;
	b2FixtureDef fixDef;
	b2Vec2 vertices[4];
	vertices[0].Set(position.x, position.y);
	vertices[1].Set(position.x + width, position.y);
	vertices[2].Set(position.x + width, position.y + length);
	vertices[3].Set(position.x, position.y + length);

	b2PolygonShape shape;
	shape.Set(vertices, 4);
	fixDef.shape = &shape;
	bodyDef.type = b2_staticBody;
	physBody = physWorld->CreateBody(&bodyDef);
	physBody->CreateFixture(&fixDef);
	physBody->SetUserData(this);
}

void Item::buildPhys(std::vector<Vector2> points){
	if (physBody != NULL)
		physWorld->DestroyBody(physBody);
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	
	physBody = physWorld->CreateBody(&bodyDef);

	std::vector<Vector2> pointsUsed;
	
	if (points.size() == 0)
		GP_ERROR("We need to have points.  One does not simply have no points!");
	for (unsigned int i = 0; i < points.size(); ++i){
		if (pointsUsed.size() > 0){
			b2EdgeShape shape;
			b2Vec2 line[2];
			line[0].Set(pointsUsed[pointsUsed.size() - 1].x, pointsUsed[pointsUsed.size() - 1].y);
			line[1].Set(points[i].x, points[i].y);
			shape.Set(line[0], line[1]);
			b2FixtureDef fixDef;
			fixDef.shape = &shape;
			physBody->CreateFixture(&fixDef);
		}
		pointsUsed.push_back(points[i]);
	}

	// close off
	b2EdgeShape shape;
	b2Vec2 line[2];
	Vector2 first = pointsUsed[0];
	Vector2 last = pointsUsed[pointsUsed.size() - 1];
	line[0].Set(first.x, first.y);
	line[1].Set(last.x, last.y);
	shape.Set(line[0], line[1]);
	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	physBody->CreateFixture(&fixDef);

	physBody->SetUserData(this);
}

bool Item::SetPosition(float x, float y, PlayField* playField)
{
	if (status == PREVIEW){
		if (playField == NULL)
			GP_ERROR("if we set the position to this item with a status of 'preview', you need to give me a playfield to work off of");

		if (!canPlace(Vector2(x, y), playField))
			return false;

		else{
			if (node != NULL){
				SetRotation(rotation);
			}
		}
	}


	position.set((int)x, (int)y);
	if (physBody != NULL){
		physWorld->DestroyBody(physBody);
		physBody = NULL;
	}
	if (status == SOLID)
		if (shape == PhysicShape::CIRCLE)
			buildPhys(radius);  // will update with new position
		else if (shape == PhysicShape::BOX)
			buildPhys(currentDimensions.x, currentDimensions.y);
	if (node != NULL){
		node->setTranslation((int)position.x + currentDimensions.x / 2, (int)position.y + currentDimensions.y / 2, 0);

		Vector3 start(currentDimensions.x / -2 + position.x
			, currentDimensions.y / -2 + position.y
			, 0);
		Vector3 end(currentDimensions.x / 2 + position.x
			, currentDimensions.y / 2 + position.y
			, currentDimensions.z);
		BoundingBox box(start, end);
		dynamic_cast<Model*>(Item::node->getDrawable())->getMesh()->setBoundingBox(box);
		// later on, fix the interactPoint based on rotation of item
		interactPoint.set(interactOffset.x + node->getTranslationX(), interactOffset.y + node->getTranslationY());
	}

	return true;
}


void Item::prepSelf()
{
	if (status == ABSTRACT){
		
		sendToLists(textureF, nodeF, nodeI, this);
		if (shape == BOX)
			buildPhys(currentDimensions.x, currentDimensions.y);
		else if (shape == CIRCLE)
			buildPhys(radius);
	}
	if(status == SOLID)
		SetPosition(position.x, position.y);
}
void Item::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (status != ABSTRACT){
		Item::node->setTranslation(position.x + currentDimensions.x / 2, position.y + currentDimensions.y / 2, 0);
		SetRotation(rotation);
	}		
}



void Item::Render()
{
	node->getDrawable()->draw();
}

void Item::SetRotation(Rotation rotation)
{
	Item::rotation = rotation;
	Item::node->setRotation(Matrix::identity()); // default until we could fix the original
	Item::node->rotateZ(MATH_DEG_TO_RAD(-90));
	Item::node->rotateZ(MATH_DEG_TO_RAD(rotation * 90));

	fixDimensions(rotation);
}

void Item::fixDimensions(Rotation rotation){
	if (rotation == WEST || rotation == EAST)
	{
		currentDimensions.set(defaultDimensions.y, defaultDimensions.x, defaultDimensions.z);
	}
	else{
		currentDimensions.set(defaultDimensions.x, defaultDimensions.y, defaultDimensions.z);
	}
}



void Item::SetStatus(Status nextStatus, b2World* physWorld)
{
	if (Item::status == ABSTRACT){
		if (nextStatus == PREVIEW){

		}
		else if (nextStatus == FREE_BOUND){

		}
		else if (nextStatus == SOLID){

		}
	}
	else if (Item::status == PREVIEW){
		if (nextStatus == ABSTRACT){

			Scene* _scene = node->getScene();
			_scene->removeNode(node);
			node->release();
			node = NULL;
		}
		else if (nextStatus == FREE_BOUND){

		}
		else if (nextStatus == SOLID){
			if (physWorld == NULL)
				GP_ERROR("you need a physworld to put physics into");
			Item::physWorld = physWorld;
			if (shape == PhysicShape::BOX)
				buildPhys(currentDimensions.x, currentDimensions.y);
			else if (shape == PhysicShape::CIRCLE)
				buildPhys(radius);
		}
	}
	else if (Item::status == FREE_BOUND){
		if (nextStatus == ABSTRACT){

		}
		else if (nextStatus == PREVIEW){

		}
		else if (nextStatus == SOLID){

		}
	}
	else if (Item::status == SOLID){
		if (nextStatus == ABSTRACT){
			physWorld->DestroyBody(physBody);
			physBody = NULL;
			Scene* _scene = node->getScene();
			_scene->removeNode(node);
			node->release();
			node = NULL;
		}
		else if (nextStatus == PREVIEW){

		}
		else if (nextStatus == FREE_BOUND){

		}
	}

	Item::status = nextStatus;
}



Vector2 Item::getInteractPoint(){
	return interactPoint;
}
Vector2 Item::getPointTowards(){
	return pointTowards;
}
bool Item::inInteractZone(Vector2 pos){
	return pos.distance(interactPoint) < interactRange;
}

void Item::playAnimation(AnimationClip* clip)
{
	if (currentAnimClip != NULL  && currentAnimClip->isPlaying())
			currentAnimClip->stop();
		currentAnimClip = clip;
		currentAnimClip->play();
	
}


bool Item::canPlace(Vector2 pos, PlayField* playField){
	// test playfield to see if we can park in this spot
	if (playField->isFreeTile(pos.x, pos.y))
		return true;
	else
		return false;
}

void Item::SetAlpha(float alpha){
	if (node != NULL && transparent){
		if (dynamic_cast<Model*>(node->getDrawable())->getMaterial() != NULL)
			dynamic_cast<Model*>(node->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setFloat(alpha);
		else if (dynamic_cast<Model*>(node->getDrawable())->getMaterial(0) != NULL)
			dynamic_cast<Model*>(node->getDrawable())->getMaterial(0)->getParameter("u_modulateAlpha")->setFloat(alpha);
	}
		
}