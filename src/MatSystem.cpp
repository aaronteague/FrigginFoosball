#include "MatSystem.h"

MatSystem::MatSystem(const int worldWidth, const int worldLength)
{
	matMap.resize(worldWidth);
	for (int i = 0; i < worldWidth; ++i){
		matMap[i].resize(worldLength);
		for (int j = 0; j < worldLength; ++j){
			matMap[i][j].file = "";
			matMap[i][j]._batch = NULL;
		}
	}
	buildCounter = 0;

	r.set(1, 0, 0);
	faceUp.set(0, 1, 0);
	color.set(0.9f, 0.9f, 0.9f, 1.0f);

	faceUp.normalize();
	r.normalize();
}
void MatSystem::addMat(const std::string file, const Vector2 &pos)
{
	if (matMap[pos.x][pos.y]._batch != NULL){
		removeMat(pos);
	}
	matMap[pos.x][pos.y].file = file;
	
	matMap[pos.x][pos.y].trans.set(pos.x + 0.5f, pos.y + 0.5f, 0.01f);
	
}
void MatSystem::removeMat(const Vector2& pos)
{
	
	SpriteBatch* _batch = matMap[pos.x][pos.y]._batch;
	matMap[pos.x][pos.y]._batch = NULL;
	matMap[pos.x][pos.y].file = "";

	if (_batch != NULL){
		int refCount = 0;

		for (unsigned int i = 0; i < matMap.size(); ++i)
			for (unsigned int j = 0; j < matMap[i].size(); ++j)
				if (matMap[i][j]._batch == _batch)
					refCount++;

		if (refCount == 0){
			int index = -1;
			for (unsigned int i = 0; i < batchList.size(); ++i)
				if (_batch == batchList[i])
					index = i;

			batchList.erase(batchList.begin() + index);
			texFiles.erase(texFiles.begin() + index);

			// if it has been sorted already for rendering
			// make it so that it doesn't
			for (unsigned int i = 0; i < spriteRenderList.size(); ++i){
				if (_batch == spriteRenderList[i]){
					spriteRenderList.erase(spriteRenderList.begin() + i);
					renderList.erase(renderList.begin() + i);
					break;
				}
			}

			delete(_batch);
		}
	}
		
}

void MatSystem::Update(const Vector3& trans)
{
	renderList.clear();
	spriteRenderList.clear();

	int xMin = trans.x - X_RANGE;
	int xMax = trans.x + X_RANGE;
	int yMin = trans.y - Y_RANGE;
	int yMax = trans.y + Y_RANGE;
	// adjust so it doesn't go out range
	int high = matMap.size();
	if (xMin < 0)xMin = 0;
	if (xMax > high)xMax = high;
	if (yMin < 0)yMin = 0;
	if (yMax > high)yMax = high;

	for (int i = xMin; i < xMax; ++i)
		for (int j = yMin; j < yMax; ++j){			
			if (matMap[i][j]._batch != NULL){
				int index = -1;
				for (unsigned int k = 0; k < spriteRenderList.size(); ++k){
					if (matMap[i][j]._batch == batchList[k]){
						renderList[k].push_back(&matMap[i][j]);
						index = k;
						break;
					}
				}
				if (index == -1)
				{
					spriteRenderList.push_back(matMap[i][j]._batch);
					std::vector<Mat*> temp;
					renderList.push_back(temp);
					renderList[renderList.size() - 1].push_back(&matMap[i][j]);
					
				}
			}
			
		}

}
void MatSystem::Render(const Matrix& viewProjMatrix)
{
	for (unsigned int i = 0; i < renderList.size(); ++i){
		spriteRenderList[i]->start();
		spriteRenderList[i]->setProjectionMatrix(viewProjMatrix);
		for (unsigned int j = 0; j < renderList[i].size(); ++j)
			spriteRenderList[i]->draw(
			renderList[i][j]->trans, r, faceUp, 1, 1
			, 0
			, 0
			, 1
			, 1
			, color, Vector2(0.5, 0.5), 0);

		spriteRenderList[i]->finish();
	}
}

void MatSystem::prepSelf()
{
	for (unsigned int i = 0; i < matMap.size(); ++i)
		for (unsigned int j = 0; j < matMap[i].size(); ++j){
			if (matMap[i][j]._batch == NULL
				&& matMap[i][j].file.compare("") != 0
				&& matMap[i][j].file.compare("none") != 0){
				int index = -1;
				for (unsigned int k = 0; k < texFiles.size(); ++k)
					if (matMap[i][j].file.compare(texFiles[k]) == 0){
						// is this a bug?
						//int index = k;
						//matMap[i][j]._batch = batchList[k];
						//break;
					}
				if (index == -1){ // if it didn't find an existing texture made
					 // try to find if it is already set to build
					for (unsigned int k = 0; k < texFilesToBuild.size(); ++k)
						if (matMap[i][j].file.compare(texFilesToBuild[k]) == 0){
							buildList[k].push_back(&matMap[i][j]);
							index = k;
						}
					
				}
				if (index == -1){ // didn't find an existing batch or wasn't in build list
					texFilesToBuild.push_back(matMap[i][j].file);
					std::vector<Mat*> temp;
					buildList.push_back(temp);
					buildList[buildList.size() - 1].push_back(&matMap[i][j]);
					sendToLists(matMap[i][j].file, "none", "none", this);
				}
			
					
				
			}
		}
}
void MatSystem::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (buildCounter < buildList.size()){
		SpriteBatch *_batch = SpriteBatch::create(tex);
		for (unsigned int i = 0; i < buildList[buildCounter].size(); ++i)
			buildList[buildCounter][i]->_batch = _batch;
		batchList.push_back(_batch);
		texFiles.push_back(texFilesToBuild[buildCounter]);
		buildCounter++;
	}
	if (buildCounter >= buildList.size()){
		buildList.clear();
		texFilesToBuild.clear();
		buildCounter = 0;
	}
	
}

MatSystem::~MatSystem()
{
	for (unsigned int i = 0; i < batchList.size(); ++i)
		delete(batchList[i]);

}