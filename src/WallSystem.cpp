#include "WallSystem.h"
#include "Tile.h"
#include "LuaLoader.h"

WallSegment::WallSegment(std::string file, int side, Vector2 pos, LuaLoader *loader)
{
	std::vector<float> rotation;
	rotation.resize(4);
	rotation[WallSystem::NORTH] = MATH_DEG_TO_RAD(0);
	rotation[WallSystem::WEST] = MATH_DEG_TO_RAD(90);
	rotation[WallSystem::EAST] = MATH_DEG_TO_RAD(270);
	rotation[WallSystem::SOUTH] = MATH_DEG_TO_RAD(180);

	std::vector<Vector2> doorOffset;
	doorOffset.resize(4);

	doorOffset[WallSystem::NORTH].set(1,0);
	doorOffset[WallSystem::WEST].set(1,0);
	doorOffset[WallSystem::EAST].set(0,0);
	doorOffset[WallSystem::SOUTH].set(1,1);


	ghost = false;
	hasExtra = false;
	WallSegment::file = file;
	WallSegment::side = side;
	WallSegment::pos = pos;
	_batch = NULL;
	index = -1;

	std::string extension;
	int index = file.find_last_of('.');
	if (index != -1)
		extension = file.substr(index);

	if (extension.compare(".png") == 0){
		trans.resize(3);
		paperFile = file;
		// make the wallRectangle
		Vector2 turn(0.5f, 0.9f);
		turn.rotate(Vector2(0.5f, 0.5f), rotation[side]);
		trans[0].set(pos.x + turn.x, pos.y + turn.y, 2.5f); // TOP
		trans[1].set(pos.x + turn.x, pos.y + turn.y, 1.5f); // MIDDLE
		trans[2].set(pos.x + turn.x, pos.y + turn.y, 0.5f); // BOTTOM
		turn.set(0.5f, 0.95f);
		turn.rotate(Vector2(0.5f, 0.5f), rotation[side]);
		topTrans.set(pos.x + turn.x, pos.y + turn.y, 3);

	}

	else if(extension.compare(".lua") == 0){
		loader->setFile(file);
		
		doorID = loader->getString("NODE_ID");
		doorTex = loader->getString("NODE_TEXTURE");
		wallAroundDoorID = loader->getString("SURROUNDING_WALL_ID");
		paperFile = loader->getString("SURROUNDING_WALL_TEXTURE");
		length = loader->getInt("LENGTH");
		doorFile = loader->getString("NODE_FILE");

		trans.push_back(Vector3(pos.x + doorOffset[side].x * (float)length / 2
			, pos.y + doorOffset[side].y *(float)length / 2, 0));

		Vector2 offset(0, 0.95f);
		offset.rotate(Vector2(0, 0), rotation[side]);
		topTrans.set(trans[0].x + offset.x, trans[0].y + offset.y, 3);

		
		int counter = 0;
		luabridge::LuaRef barTable = loader->getTable("BARRIERS");
		while(true){
			counter++;
			luabridge::LuaRef indivBar = loader->getTable(counter, barTable);
			if (indivBar.isNil())
				break;
			float startX = loader->getFloat(1, indivBar);
			float startY = loader->getFloat(2, indivBar);
			float endX = loader->getFloat(3, indivBar);
			float endY = loader->getFloat(4, indivBar);
			Vector2 start(startX, startY);
			Vector2 end(endX, endY);
			
			start.rotate(Vector2(0, 0), rotation[side]);
			end.rotate(Vector2(0, 0), rotation[side]);
			
			
			barriers.push_back(Vector4
			(start.x + trans[0].x
			, start.y + trans[0].y
			, end.x + trans[0].x
			, end.y + trans[0].y));
		}
	}


	
}

WallSystem::WallSystem(std::vector<std::vector<Tile*>> &tileMap, LuaLoader *loader)
{


	// make the size
	wallSegments.resize(tileMap.size());
	for (unsigned int i = 0; i < wallSegments.size(); ++i){
		wallSegments[i].resize(tileMap[i].size());
		for (unsigned int j = 0; j < wallSegments[i].size(); ++j){
			wallSegments[i][j].resize(4);
			for (unsigned int k = 0; k < wallSegments[i][j].size(); ++k){
				wallSegments[i][j][k] = NULL;
				std::string wallPaper;
				switch (k){
				case NORTH:
					wallPaper = tileMap[i][j]->northWallpaper;
					break;
				case WEST:
					wallPaper = tileMap[i][j]->westWallpaper;
					break;
				case EAST:
					wallPaper = tileMap[i][j]->eastWallpaper;
					break;
				case SOUTH:
					wallPaper = tileMap[i][j]->southWallpaper;
					break;
				}

				if (wallPaper.compare("") != 0 && wallPaper.compare("occupied") != 0)
					wallSegments[i][j][k] = new WallSegment(wallPaper, k, Vector2(i, j), loader);
			}

		}
	}
	buildCounter = 0;



	// look throught the wallsegments here
	for (unsigned int i = 0; i < wallSegments.size(); ++i)
	for (unsigned int j = 0; j < wallSegments[i].size(); ++j)
	for (unsigned int k = 0; k < wallSegments[i][j].size(); ++k){
		// first, check if the wallSegment is an actual door
		if (wallSegments[i][j][k] != NULL && wallSegments[i][j][k]->doorID.compare("") != 0){
			doors.push_back(wallSegments[i][j][k]);
			for (unsigned int l = 0; l < wallSegments[i][j][k]->barriers.size(); ++l)
				barriers.push_back(wallSegments[i][j][k]->barriers[l]);
		}// if not , it is probably a wall paper
		else if (wallSegments[i][j][k] != NULL && wallSegments[i][j][k]->_batch == NULL){
			int index = -1;
			for (unsigned int l = 0; l < texFiles.size(); ++l)
			if (wallSegments[i][j][k]->paperFile.compare(texFiles[l]) == 0){
				index = l;
				wallSegments[i][j][k]->_batch = batchList[l];
				break;
			}
			if (index == -1) // if it didn't find a texture already made
				// to to see if it is already set to build
			for (unsigned int l = 0; l < texFilesToBuild.size(); ++l)
			if (wallSegments[i][j][k]->paperFile.compare(texFilesToBuild[l]) == 0){
				buildList[l].push_back(wallSegments[i][j][k]);
				index = l;
			}
			if (index == -1){ // didn't find an existing batch or in a build list
				texFilesToBuild.push_back(wallSegments[i][j][k]->paperFile);
				std::vector<WallSegment*> temp;
				buildList.push_back(temp);
				buildList[buildList.size() - 1].push_back(wallSegments[i][j][k]);
				//sendToLists(wallSegments[i][j][k]->paperFile, "none", "none", this);
			}
		}
	}

	findAndBuildExtras();

	calculateBarriers();
}

bool WallSystem::segmentBeenUsed(WallSegment* segment, std::vector<WallSegment*> &existingSegments){
	for (unsigned int i = 0; i < existingSegments.size(); ++i)
	if (existingSegments[i] == segment)
		return true;
	return false;
}

void WallSystem::calculateBarriers()
{
	std::vector<Vector2> checkDirection;
	checkDirection.resize(4);
	checkDirection[NORTH].set(1, 0);
	checkDirection[WEST].set(0, 1);
	checkDirection[EAST].set(0, 1);
	checkDirection[SOUTH].set(1, 0);

	std::vector<Vector2> barrierPoint;
	barrierPoint.resize(4);
	barrierPoint[NORTH].set(0, 0.9f);
	barrierPoint[WEST].set(0.1f, 0);
	barrierPoint[EAST].set(0.9f, 0);
	barrierPoint[SOUTH].set(0, 0.1f);
	

	//std::vector < Vector4 > walls;
	std::vector<WallSegment*> checkedSegments;
	
	for (unsigned int i = 0; i < wallSegments.size(); ++i)
	for (unsigned int j = 0; j < wallSegments[i].size(); ++j)
	for (unsigned int k = 0; k < wallSegments[i][j].size(); ++k){
		Vector2 start(i, j);
		start += barrierPoint[k];
		Vector2 end(-1, -1);
		bool loop = true;
		int counter = 0;
	//	if (wallSegments[i][j][k] != NULL && wallSegments[i][j][k]->doorFile.compare("") == 0)
		while (loop){
			Vector2 direction(i + checkDirection[k].x * counter, j + checkDirection[k].y * counter);
			if (i < wallSegments.size() && j < wallSegments[i].size()) // in bounds?
			if (wallSegments[direction.x][direction.y][k] != NULL // check that there is a segment
				&& wallSegments[direction.x][direction.y][k]->doorFile.compare("") == 0 
				&& !segmentBeenUsed(wallSegments[direction.x][direction.y][k], checkedSegments)){
				//Vector3 trans = wallSegments[direction.x][direction.y][k]->trans[0];
				end.set(barrierPoint[k] + direction + checkDirection[k]);
				checkedSegments.push_back(wallSegments[direction.x][direction.y][k]);
				counter++;
			}
			else{
				if (end.x >= 0)
					barriers.push_back(Vector4(start.x, start.y, end.x, end.y));
				loop = false;
			}
			
			
		}
	}
	
	

	
}

void WallSystem::Update(const Vector3& trans)
{
	renderList.clear();
	spriteRenderList.clear();
	doorsToDraw.clear();

	int xMin = trans.x - RANGE;
	int xMax = trans.x + RANGE;
	int yMin = trans.y - RANGE;
	int yMax = trans.y + RANGE;
	// adjust so it doesn't go out range
	int high = wallSegments.size();
	if (xMin < 0)xMin = 0;
	if (xMax > high)xMax = high;
	if (yMin < 0)yMin = 0;
	if (yMax > high)yMax = high;

	for (int i = xMin; i < xMax; ++i)
	for (int j = yMin; j < yMax; ++j)
	for(int k = 0; k < 4; ++k){
		if (wallSegments[i][j][k] != NULL && wallSegments[i][j][k]->_batch != NULL){
			int index = -1;
			for (unsigned int l = 0; l < spriteRenderList.size(); ++l){
				if (wallSegments[i][j][k]->_batch == batchList[l]){
					renderList[l].push_back(wallSegments[i][j][k]);
					index = l;
					break;
				}
			}
			if (index == -1)
			{
				spriteRenderList.push_back(wallSegments[i][j][k]->_batch);
				std::vector<WallSegment*> temp;
				renderList.push_back(temp);
				renderList[renderList.size() - 1].push_back(wallSegments[i][j][k]);

			}
		}
		else if (wallSegments[i][j][k] != NULL && wallSegments[i][j][k]->door != NULL){
			doorsToDraw.push_back(wallSegments[i][j][k]);
		}

	}
}

void WallSystem::Render(const Matrix& viewProjMatrix)
{
	//Vector3 rSouth(1, 0, 0);
	//Vector3 rEastWest(0, 1, 0);
	//rEastWest.normalize();
	////Vector3 faceUp(0, 1, 0);
	//Vector3 faceSouth(0, 0, 1);
	//Vector3 faceEast(0, 0, 1);
	//Vector3 faceWest(0, 0, -1);
	//Vector4 color(0.5f, 0.5f, 0.5f, 0.5f);
	////faceUp.normalize();
	//faceEast.normalize();
	//rSouth.normalize();
	//Vector3 *right;
	//Vector3 *direction;
	//for (unsigned int i = 0; i < renderList.size(); ++i){
	//	spriteRenderList[i]->start();
	//	spriteRenderList[i]->setProjectionMatrix(viewProjMatrix);
	//	for (unsigned int j = 0; j < renderList[i].size(); ++j){
	//		if (renderList[i][j]->side == NORTH){
	//			right = &rSouth;
	//			direction = &faceSouth;
	//		}
	//		else if (renderList[i][j]->side == WEST){
	//			right = &rEastWest;
	//			direction = &faceEast;
	//		}
	//		else if (renderList[i][j]->side == EAST){
	//			right = &rEastWest;
	//			direction = &faceWest;
	//		}
	//		// top
	//		spriteRenderList[i]->draw(
	//			renderList[i][j]->trans[0], *right, *direction, 1, 1
	//			, 0
	//			, 0.5
	//			, 0.5
	//			, 1
	//			, color, Vector2(0.5, 0.5), 0);
	//		// middle
	//		spriteRenderList[i]->draw(
	//			renderList[i][j]->trans[1], *right, *direction, 1, 1
	//			, 0.5
	//			, 0.5
	//			, 1
	//			, 1
	//			, color, Vector2(0.5, 0.5), 0);
	//		// bottom
	//		spriteRenderList[i]->draw(
	//			renderList[i][j]->trans[2], *right, *direction, 1, 1
	//			, 0
	//			, 0
	//			, 0.5
	//			, 0.5
	//			, color, Vector2(0.5, 0.5), 0);
	//	}
	//	spriteRenderList[i]->finish();
	//}



	Vector3 rSouth(1, 0, 0);
	Vector3 rEastWest(0, 1, 0);
	rEastWest.normalize();
	//Vector3 faceUp(0, 1, 0);
	Vector3 faceSouth(0, 0, 1);
	Vector3 faceEast(0, 0, 1);
	Vector3 faceWest(0, 0, -1);

	Vector4 color(0.5f, 0.5f, 0.5f, 1.0f);
	//faceUp.normalize();
	faceEast.normalize();
	rSouth.normalize();
	Vector3 *right;
	Vector3 *direction;
	for (unsigned int i = 0; i < renderList.size(); ++i){
		spriteRenderList[i]->start();
		spriteRenderList[i]->setProjectionMatrix(viewProjMatrix);
		for (unsigned int j = 0; j < renderList[i].size(); ++j){
			if (renderList[i][j]->side == NORTH){
				right = &rSouth;
				direction = &faceSouth;
			}
			else if (renderList[i][j]->side == WEST){
				right = &rEastWest;
				direction = &faceEast;
			}
			else if (renderList[i][j]->side == EAST){
				right = &rEastWest;
				direction = &faceWest;
			}
			else if (renderList[i][j]->side == SOUTH){
				right = &rSouth; // use this one for visual purposes
				direction = &faceSouth; // and this one too
			}

			// top
			spriteRenderList[i]->draw(
				renderList[i][j]->trans[0], *right, *direction, 1, 1
				, 0
				, 0.5f
				, 0.5f
				, 1
				, color, Vector2(0.5f, 0.5f), 0);
			// middle
			spriteRenderList[i]->draw(
				renderList[i][j]->trans[1], *right, *direction, 1, 1
				, 0.5f
				, 0.5f
				, 1
				, 1
				, color, Vector2(0.5f, 0.5f), 0);
			// bottom
			spriteRenderList[i]->draw(
				renderList[i][j]->trans[2], *right, *direction, 1, 1
				, 0
				, 0
				, 0.5f
				, 0.5f
				, color, Vector2(0.5f, 0.5f), 0);

			// if we have extra to draw on the segment
			if (renderList[i][j]->hasExtra){
		//		if (false){
				// top
				spriteRenderList[i]->draw(
					renderList[i][j]->extraTrans[0], *right, *direction, 0.1, 1
					, 0
					, 0.5f
					, 0.5f
					, 1
					, color, Vector2(0.5f, 0.5f), 0);
				// middle
				spriteRenderList[i]->draw(
					renderList[i][j]->extraTrans[1], *right, *direction, 0.1, 1
					, 0.5f
					, 0.5f
					, 1
					, 1
					, color, Vector2(0.5f, 0.5f), 0);
				// bottom
				spriteRenderList[i]->draw(
					renderList[i][j]->extraTrans[2], *right, *direction, 0.1, 1
					, 0
					, 0
					, 0.5f
					, 0.5f
					, color, Vector2(0.5f, 0.5f), 0);
			}

		}
		spriteRenderList[i]->finish();
	}

	for (unsigned int i = 0; i < doorsToDraw.size(); ++i){
		doorsToDraw[i]->door->getDrawable()->draw();
		doorsToDraw[i]->wallAroundDoor->getDrawable()->draw();
	}

	// now draw the tops of the walls
	Vector3 r(1, 0, 0);
	Vector3 faceUp(0, 1, 0);

	faceUp.normalize();
	r.normalize();
	float rotation;


	wallTopBatch->start();
	wallTopBatch->setProjectionMatrix(viewProjMatrix);
	for (unsigned int i = 0; i < renderList.size(); ++i){

		for (unsigned int j = 0; j < renderList[i].size(); ++j){
			if (renderList[i][j]->side == NORTH){
				right = &r;
				direction = &faceUp;
				rotation = 0;
			}
			else if (renderList[i][j]->side == WEST){
				right = &r;
				direction = &faceUp;
				rotation = MATH_DEG_TO_RAD(-90);
			}
			else if (renderList[i][j]->side == EAST){
				right = &r;
				direction = &faceUp;
				rotation = MATH_DEG_TO_RAD(90);
			}
			else if (renderList[i][j]->side == SOUTH){
				right = &r;
				direction = &faceUp;
				rotation = MATH_DEG_TO_RAD(180);
			}


			wallTopBatch->draw(
				renderList[i][j]->topTrans, *right, *direction, 1, 0.1f
				, 0
				, 0
				, 1
				, 0.1f
				, color, Vector2(0.5f, 0.5f), rotation);

			// check for extra
			if (renderList[i][j]->hasExtra && (renderList[i][j]->side == NORTH || renderList[i][j]->side == SOUTH)){
				wallTopBatch->draw(
				renderList[i][j]->extraTopTrans, *right, *direction, 0.1f, 0.1f
					, 0
					, 0
					, 1
					, 1
					, color, Vector2(0.5f, 0.5f), rotation);
			}

		}

	}

	// make tops for doors
	for (unsigned int i = 0; i < doorsToDraw.size(); ++i){
		if (doorsToDraw[i]->side == NORTH){
			wallTopBatch->draw(
				doorsToDraw[i]->topTrans, r, faceUp, 2, 0.1f
				, 0
				, 0
				, 1
				, 0.1f
				, color, Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(0));
		}
		else if (doorsToDraw[i]->side == WEST){
			wallTopBatch->draw(
				doorsToDraw[i]->topTrans, r, faceUp, 2, 0.1f
				, 0
				, 0
				, 1
				, 0.1f
				, color, Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(-90));
		}
		else if (doorsToDraw[i]->side == EAST){
			wallTopBatch->draw(
				doorsToDraw[i]->topTrans, r, faceUp, 2, 0.1f
				, 0
				, 0
				, 1
				, 0.1f
				, color, Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(90));
		}
		else if (doorsToDraw[i]->side == SOUTH){
			wallTopBatch->draw(
				doorsToDraw[i]->topTrans, r, faceUp, 2, 0.1f
				, 0
				, 0
				, 1
				, 0.1f
				, color, Vector2(0.5f, 0.5f), MATH_DEG_TO_RAD(180));
		}
	}

	// go ahead and make the close offs
	if (false) // let's not do this part for now
	for (unsigned int i = 0; i < closeOffs.size(); ++i){
		if (closeOffs[i].side == WEST)
			wallTopBatch->draw(
			closeOffs[i].trans, rSouth, faceSouth, 0.2, 3
			, 0
			, 0
			, 1
			, 0.1
			, color, Vector2(0.5f, 0.5f), 0);
		else if (closeOffs[i].side == NORTH){
			wallTopBatch->draw(
				closeOffs[i].trans, rEastWest, faceWest, 0.2, 3
				, 0
				, 0
				, 1
				, 0.1
				, color, Vector2(0.5f, 0.5f), 0);
			wallTopBatch->draw(
				closeOffs[i].trans, rEastWest, faceEast, 0.2, 3
				, 0
				, 0
				, 1
				, 0.1
				, color, Vector2(0.5f, 0.5f), 0);
		}

	}

	wallTopBatch->finish();
}

void WallSystem::changeWallpaper(std::string, WallSide side)
{

}

void WallSystem::prepSelf()
{
	// build the wallpapers first, no doors
	for (unsigned int i = 0; i < buildList.size(); ++i)
		sendToLists(buildList[i][0]->paperFile, "none", "none", this);
	// now move on to the doors
	for (unsigned int i = 0; i < doors.size(); ++i){
		// actual door trim first
		sendToLists(doors[i]->doorTex, doors[i]->doorFile, doors[i]->doorID, this);
		// then the wall around the door
		sendToLists(doors[i]->paperFile, doors[i]->doorFile, doors[i]->wallAroundDoorID, this);
	}
	// wall top
	sendToLists("res/wallTop.png", "none", "none", this);
}

void WallSystem::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	Effect* _eft = Effect::createFromFile("res/shaders/sprite2.vert", "res/shaders/sprite2.frag");
	// start making wallpaper
	if (buildCounter < buildList.size()){
		
		SpriteBatch *_batch = SpriteBatch::create(tex, _eft);

		copyMaterial(NULL, _batch->getMaterial());
		
		_batch->getStateBlock()->setDepthTest(true);
		_batch->getStateBlock()->setCullFace(true);
		_batch->getStateBlock()->setBlend(true); // go back to false 
		
		for (unsigned int i = 0; i < buildList[buildCounter].size(); ++i)
			buildList[buildCounter][i]->_batch = _batch;
		batchList.push_back(_batch);
		texFiles.push_back(texFilesToBuild[buildCounter]);
		buildCounter++;
	}
	// time to make doors
	else if(buildCounter >= buildList.size() && buildCounter < buildList.size() + doors.size() * 2){
		
		int index = (buildCounter - buildList.size()) / 2;
		int part = (buildCounter - buildList.size()) % 2;
		float rotation;
		if (doors[index]->side == WEST)
			rotation = MATH_DEG_TO_RAD(90);
		else if (doors[index]->side == EAST)
			rotation = MATH_DEG_TO_RAD(-90);
		else if (doors[index]->side == SOUTH)
			rotation = MATH_DEG_TO_RAD(180);
		else
			rotation = 0;
		if (part == 0){ // the trim part
			doors[index]->door = nodeToScene(node, tex, _scene);
			doors[index]->door->setTranslation(doors[index]->trans[0]);
			doors[index]->door->setRotation(Vector3(0, 0, 1), rotation);
		}
		else if (part == 1){ // the wall around door
			doors[index]->wallAroundDoor = nodeToScene(node, tex, _scene);
			doors[index]->wallAroundDoor->setTranslation(doors[index]->trans[0]);
			doors[index]->wallAroundDoor->setRotation(Vector3(0, 0, 1), rotation);
		}
		buildCounter++;
	}
	else{
		wallTopBatch = SpriteBatch::create(tex);
		
		buildList.clear();
		texFilesToBuild.clear();
		buildCounter = 0;
	}

	//if (buildCounter >= buildList.size() + doors.size() * 2){

	//}
}

std::vector<Vector4> WallSystem::getBarriers()
{
	return barriers;
}

void WallSystem::findAndBuildExtras(){
	std::vector<WallSegment*> checkedSegments;

	for (unsigned int i = 0; i < wallSegments.size(); ++i)
	for (unsigned int j = 0; j < wallSegments[i].size(); ++j)
	for (unsigned int k = 0; k < wallSegments[i][j].size(); ++k)
	if (wallSegments[i][j][k] != NULL)
	{// check first to see if we checked it
		bool used = false;
		for (unsigned int l = 0; l < checkedSegments.size(); ++l)
		if (checkedSegments[l] == wallSegments[i][j][k]){
			used = true;
			break;
		}
		if (!used){
			bool loop = true;
			Vector2 checkPos(i, j);
			Vector2 checkDirection;
			if (k == NORTH)
				checkDirection.set(1, 0);
			else if (k == SOUTH)
				checkDirection.set(1, 0);
			else if (k == WEST)
				checkDirection.set(0, 1);
			else if (k == EAST)
				checkDirection.set(0, 1);
			while (loop){
				// check back first
				checkedSegments.push_back(wallSegments[checkPos.x][checkPos.y][k]); // register it

				Vector2 back = checkPos - checkDirection;
				Vector2 forward = checkPos + checkDirection;
				if (wallSegments[back.x][back.y][k] == NULL){ // if yes, we found our dropoff point
					if (hasPerpendicularWall(back, checkDirection)){
						wallSegments[checkPos.x][checkPos.y][k]->hasExtra = true;
						// add more about trans here
						if (k == NORTH){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.9, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.9, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.9, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x - 0.050, checkPos.y + 0.95, 3);
						}
						else if (k == SOUTH){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.1, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.1, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x - 0.05, checkPos.y + 0.1, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x - 0.05, checkPos.y + 0.05, 3);
						}
						else if (k == WEST){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y - 0.05, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y - 0.05, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y - 0.05, 0.5)); // bottom
						//	wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 0.025, checkPos.y - 0.1, 3);
						}
						else if (k == EAST){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y - 0.05, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y - 0.05, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y - 0.05, 0.5)); // bottom
						//	wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 0.95, checkPos.y + 0.025, 3);
						}

					}
					else{
						CloseOff temp;
						temp.side = k;

						if (k == NORTH)
							temp.trans.set(checkPos.x, checkPos.y + 1, 1.5);
						else if (k == WEST)
							temp.trans.set(checkPos.x, checkPos.y, 1.5);

						if (temp.trans.x > 0)
							closeOffs.push_back(temp);
					}

					loop = false;

				}
				if (wallSegments[forward.x][forward.y][k] == NULL){// if yes, we found a dropoff point
					if (hasPerpendicularWall(forward, checkDirection)){
						wallSegments[checkPos.x][checkPos.y][k]->hasExtra = true;
						// add more about trans here
						if (k == NORTH){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.9, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.9, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.9, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 1.050, checkPos.y + 0.95, 3);
						}
						else if (k == SOUTH){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.1, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.1, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 1.05, checkPos.y + 0.1, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 1.05, checkPos.y + 0.05, 3);
						}
						else if (k == WEST){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y + 1.05, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y + 1.05, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.1, checkPos.y + 1.05, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 0.0975, checkPos.y + 1.025, 3);
						}
						else if (k == EAST){
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y + 1.05, 2.5)); // top
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y + 1.05, 1.5)); // middle
							wallSegments[checkPos.x][checkPos.y][k]->extraTrans.push_back(Vector3(checkPos.x + 0.9, checkPos.y + 1.05, 0.5)); // bottom
							wallSegments[checkPos.x][checkPos.y][k]->extraTopTrans.set(checkPos.x + 0.925, checkPos.y + 1.025, 3);
						}

					}
					else{

						CloseOff temp;
						temp.side = k;

						if (k == NORTH)
							temp.trans.set(checkPos.x + 1, checkPos.y + 1, 1.5);
						else if (k == WEST)
							temp.trans.set(checkPos.x, checkPos.y + 1, 1.5);

						if (temp.trans.x > 0)
							closeOffs.push_back(temp);
					}
					loop = false;

				}

				checkPos += checkDirection;


			}

		}
	}
}

bool WallSystem::hasPerpendicularWall(Vector2 pos, Vector2 direction){
	Vector2 perpVec(direction.y * -1, direction.x);

	Vector2 checkPos = pos + perpVec;
	for (int i = 0; i < 4; ++i)
	if (wallSegments[checkPos.x][checkPos.y][i] != NULL){
		return true;
	}


	checkPos = pos - perpVec;

	for (int i = 0; i < 4; ++i)
	if (wallSegments[checkPos.x][checkPos.y][i] != NULL){
		return true;
	}

	int i = 7;
	return false;
}