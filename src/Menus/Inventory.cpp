#include "Inventory.h"
#include "../Item.h"
#include "../MyCurve.h"
#include "../PlayField.h"
#include "../Character.h"


Inventory::Inventory(const Vector2 &screenSize, Character* belongsTo)
:Menu(screenSize), belongsTo(belongsTo)
{
	_curve = NULL;
	iconTouched = NULL;
	_menuBatch = NULL;
	textureF = "res/Menu/Inventory-Menu.png";


	buildCounter = 0;
	touchTimer = 0;

	
	itemArray.resize(NUM_OF_ITEM_SLOTS);
	for (unsigned int i = 0; i < itemArray.size(); ++i)
		itemArray[i] = NULL;

	MENU_FIX = 2.0 * scale.y;
	const int TAB_WIDTH = 37;
	const int TAB_HEIGHT = 38;
	const int MENU_WIDTH = 275;
	const int MENU_HEIGHT = 200;

	dockPosition.set(screenSize.x - TAB_WIDTH * MENU_FIX
		, MENU_HEIGHT * -1 * MENU_FIX + TAB_HEIGHT * MENU_FIX);
	outPosition.set(screenSize.x - MENU_WIDTH * MENU_FIX
		, MENU_HEIGHT * MENU_FIX);
//	dockPosition *= menuFix;

	outPosition.set(screenSize.x - MENU_WIDTH * MENU_FIX, 0);
//	outPosition *= menuFix;

	Vector2 dockToOutDifference(outPosition.x - dockPosition.x
		, outPosition.y - dockPosition.y);
	dockToOutLength = dockToOutDifference.length();

	menuRec.set(dockPosition.x, dockPosition.y
		, MENU_WIDTH * MENU_FIX, MENU_HEIGHT * MENU_FIX);

	// figure out these magic numbers and get rid of them
	buttonOffset.set(12 * MENU_FIX
		, menuRec.height - 22 * MENU_FIX - 14 * MENU_FIX);

	const int MENU_IMG_WIDTH = 2060;
	const int MENU_IMG_HEIGHT = 1480;

	menuImgRec.set(0, 0, MENU_IMG_WIDTH, MENU_IMG_HEIGHT);

	const int BUTTON_WIDTH = 22;
	const int BUTTON_HEIGHT = 22;
	const int BACK_BUTTON_X = 182;
	const int BACK_BUTTON_Y = 219;
	const int PAUSE_BUTTON_X = 215;
	const int PAUSE_BUTON_Y = 215;
	buttonImgRecBack.set(BACK_BUTTON_X, BACK_BUTTON_Y
		, BUTTON_WIDTH, BUTTON_HEIGHT);
	buttonImgRecPause.set(PAUSE_BUTTON_X, PAUSE_BUTON_Y
		, BUTTON_WIDTH, BUTTON_HEIGHT);

	INVENTORY_OFFSET_X = 41;
	INVENTORY_OFFSET_Y = 15;
	INVENTORY_SLOT_WIDTH = 53.5;
	INVENTORY_SLOT_HEIGHT = 54.5;
}

bool Inventory::addItem(std::string itemFile, LuaLoader* loader, int indexToPlaceInto)
{
	Item* item = Item::Create(loader, itemFile, Vector2(-1, -1), Item::SOUTH, Item::ABSTRACT, NULL);
	ItemIcon* icon = item->GetIcon();
	icon->rec.set(0, 0, INVENTORY_SLOT_WIDTH, INVENTORY_SLOT_HEIGHT);
	return addItem(icon, indexToPlaceInto);
}

bool Inventory::addItem(ItemIcon* icon, int indexToPlaceInto)
{
	bool itemAdded = false;
	if (indexToPlaceInto > -1){
		if (itemArray[indexToPlaceInto] != NULL)
			return false; // if there is something already already in the spot, send a false
		else{
			itemArray[indexToPlaceInto] = icon;  // if that spot is empty, you're good to go
			itemAdded = true;
			setIconDimensions(icon, indexToPlaceInto);
			icon->visible = true;
		}
	}
	if (itemAdded == false){
		for (unsigned int i = 0; i < itemArray.size(); ++i){
			if (itemArray[i] == NULL){ // see if we have a blank spot for an item
				itemArray[i] = icon;
				setIconDimensions(icon, i);
				icon->visible = true;
				itemAdded = true;
				break;
			}
		}
	}
	if (!itemAdded)
		return false;  // if we couldn't find a spot for it

	addToRenderList(icon);
	bool inRenderList = false;

	for (unsigned int i = 0; i < renderList.size(); ++i)  // check to see if
	for (unsigned int j = 0; j < renderList[i].size(); ++j)  // the icon is already
	if (icon == renderList[i][j])							// in the list to render
		inRenderList = true;								// if it is, make bool true

	if (!inRenderList){
		int indexUsed = -1; // means the texture is not in use
		for (unsigned int i = 0; i < textureArray.size(); ++i)
		if (textureArray[i].compare(icon->texture) == 0)
			indexUsed = i;

		if (indexUsed == -1){ // if the texture is not used already
			textureArray.push_back(icon->texture);
			itemBatch.push_back(NULL);
			std::vector<ItemIcon*> iconsToRender;
			renderList.push_back(iconsToRender);
			renderList[renderList.size() - 1].push_back(icon);
		}
		else{
			renderList[indexUsed].push_back(icon); // if we have the texture already
		}										// put it in the list to sort
	}

	return true;
}

void Inventory::Update(const float& elapsedTime)
{
	touchTimer += elapsedTime;
	if (_curve != NULL){
		_curve->evaluate(elapsedTime, menuRec.x, menuRec.y);
		setIconPositions();
		if (_curve->isComplete()){
			delete(_curve);
			_curve = NULL;
		}
	}

}

void Inventory::Render()
{
	_menuBatch->start();
	_menuBatch->draw(menuRec, menuImgRec);
	_menuBatch->finish();
	
	
	for (unsigned int i = 0; i < renderList.size(); ++i){
		itemBatch[i]->start();
		for (unsigned int j = 0; j < renderList[i].size(); ++j){
			if(renderList[i][j]->visible)
				itemBatch[i]->draw(renderList[i][j]->rec, renderList[i][j]->imgRec);
		}
		itemBatch[i]->finish();
	}
	if (iconTouched != NULL)
	if (iconTouched->preview)
		iconTouched->item->Render();
}

void Inventory::prepSelf()
{
	// first put the icons in there
	for (unsigned int i = 0; i < textureArray.size(); ++i)
		sendToLists(textureArray[i], "none", "none", this);
	// finally, lets prep the texutre for the actual menu
	sendToLists(textureF, "none", "none", this);
}

void Inventory::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	// first few passes will build the icon textures
	if (buildCounter < itemBatch.size())
		itemBatch[buildCounter] = SpriteBatch::create(tex);
	// last pass will build the actual menu
	else
		_menuBatch = SpriteBatch::create(tex);

	buildCounter++;
}



bool Inventory::touchPress(Vector2 position)
{
	touchTimer = 0;
	if (menuRec.contains(position.x, position.y)){ // make sure it is in the menu area
		
		for (unsigned int i = 0; i < itemArray.size(); ++i){
			if (itemArray[i] != NULL && itemArray[i]->rec.contains(position.x, position.y)){
				
				itemArray[i]->offset.set
					(itemArray[i]->rec.x - position.x
					, itemArray[i]->rec.y - position.y);


				
				iconTouched = itemArray[i];

				// rid menu of this icon
				for (unsigned int i = 0; i < itemArray.size(); ++i)
					if (itemArray[i] == iconTouched)
						itemArray[i] = NULL;
				return true;
			}
		}
	
		// if the icon is not touched
		touchOffset.set(menuRec.x - position.x
			, menuRec.y - position.y);
		menuRec.x = position.x + touchOffset.x;
		menuRec.y = position.y + touchOffset.y;
		return true;
		
	}
	
	
	return false;
}
void Inventory::touchMove(Vector2 position)
{
	if (iconTouched != NULL)
	{
		if (!menuRec.contains(position.x, position.y)){
			if (iconTouched->visible == true || iconTouched->preview == false){
				iconTouched->visible = false;

				

				// add ghost
				Vector2 fieldCoords = playField->screenToFieldCoordinates(position);
				
	/*			iconTouched->preview =
					iconTouched->item->addPreviewToPlayField
					(playField
					, playField->getScene()
					, fieldCoords);*/
				if (iconTouched->item->canPlace(fieldCoords, playField)){
					Item::ClearLists();
					iconTouched->preview = true;
					iconTouched->item->SetStatus(Item::PREVIEW);
					iconTouched->item->SetPosition(fieldCoords.x, fieldCoords.y, playField);
					iconTouched->item->AddToEntityList();
					Item::LoadAssets();
					Item::BuildAndFinish(playField->getScene());
					Item::ClearLists();
				}
				
				

				// scoot menu back to docked
				Vector2 menuPos(menuRec.x, menuRec.y);
				if (menuPos != dockPosition){
					float dockLength = (menuPos - dockPosition).length();
					slideTo(dockPosition, dockLength / dockToOutLength * SLIDE_TIME);				
				}

			}
			else{
				

				Vector2 fieldCoords = playField->screenToFieldCoordinates(position);
				// check to see if fieldCoords is within character's radius
				float length = fieldCoords.distance(belongsTo->GetPosition());
				if (length < belongsTo->GetRadius())
					belongsTo->ghostEquipItem(iconTouched->item);
				else
					//iconTouched->ghost =
						iconTouched->item->SetPosition(fieldCoords.x, fieldCoords.y, playField);
			}
		}
		else{
			iconTouched->rec.x = position.x + iconTouched->offset.x;
			iconTouched->rec.y = position.y + iconTouched->offset.y;

			if (iconTouched->visible == false){
				iconTouched->visible = true;
				// remove ghost
		//		iconTouched->item->removeFromPlayField(playField->getScene());
				iconTouched->item->SetStatus(Item::ABSTRACT);
				
				iconTouched->preview = false;		

				// scoot menu back to docked
				Vector2 menuPos(menuRec.x, menuRec.y);
				if (menuPos != outPosition){
					float outLength = (menuPos - outPosition).length();
					slideTo(outPosition, outLength / dockToOutLength * SLIDE_TIME);
				}
			}
			else{
			//	iconTouched->rec.x = position.x + iconTouched->offset.x;
			//	iconTouched->rec.y = position.y + iconTouched->offset.y;
			}
		}
		
		

		
	}
	else{
		menuRec.x = position.x + touchOffset.x;
		menuRec.y = position.y + touchOffset.y;

		// adjust it to not go out of bounds
		if (menuRec.x < outPosition.x)
			menuRec.x = outPosition.x;
		if (menuRec.y > outPosition.y)
			menuRec.y = outPosition.y;
		if (menuRec.x > dockPosition.x)
			menuRec.x = dockPosition.x;
		if (menuRec.y < dockPosition.y)
			menuRec.y = dockPosition.y;

		setIconPositions();
	}

}
void Inventory::touchRelease(Vector2 position)
{
	touchOffset.set(-1, -1);

	if (iconTouched != NULL){



		if (iconTouched->preview){
			
			Vector2 fieldCoords = playField->screenToFieldCoordinates(position);

			// check to see if fieldCoords is within character's radius
			float length = fieldCoords.distance(belongsTo->GetPosition());
			if (length < belongsTo->GetRadius())
				belongsTo->equipItem(iconTouched->item);
			else
				playField->putItemInTile(iconTouched->item, fieldCoords.x, fieldCoords.y);
			// remove the icon here
			removeItem(iconTouched);
		}
		else{
			// put the icon back where it belongs
			addItem(iconTouched
				, findClosestMatch(iconTouched));
		}
		iconTouched = NULL;
	}
	else{
		Vector2 menuPos(menuRec.x, menuRec.y);

		float dockLength = (menuPos - dockPosition).length();
		float outLength = (menuPos - outPosition).length();

		// check for a tap first
		if (outLength < TAP_LEEWAY && touchTimer < TAP_DURATION)
			slideTo(dockPosition, dockLength / dockToOutLength * SLIDE_TIME);
		else if (dockLength < TAP_LEEWAY && touchTimer < TAP_DURATION)
			slideTo(outPosition, outLength / dockToOutLength * SLIDE_TIME);
		// otherwise see if drug
		else if (menuPos != dockPosition && menuPos != outPosition){
		
			if (dockLength < outLength)
				slideTo(dockPosition, dockLength / dockToOutLength * SLIDE_TIME);
			else if(outLength < dockLength)
				slideTo(outPosition, outLength / dockToOutLength * SLIDE_TIME);
		}
	}


	touchTimer = 0;
}

void Inventory::setIconDimensions(ItemIcon *icon, int index)
{
	if (index >= 0){
		icon->offset.set
			(index % INVENTORY_COLUMNS * INVENTORY_SLOT_WIDTH * MENU_FIX + INVENTORY_OFFSET_X * MENU_FIX
			, index / INVENTORY_COLUMNS * INVENTORY_SLOT_HEIGHT * MENU_FIX + INVENTORY_OFFSET_Y * MENU_FIX);
		icon->rec.set
			(menuRec.x + icon->offset.x
			, menuRec.y + icon->offset.y
			, INVENTORY_SLOT_WIDTH * MENU_FIX
			, INVENTORY_SLOT_HEIGHT * MENU_FIX);
	}
	else{
		icon->rec.set(-1000, -1000, INVENTORY_SLOT_WIDTH * MENU_FIX
			, INVENTORY_SLOT_HEIGHT * MENU_FIX);
	}
}

void Inventory::setIconPositions()
{
	for (unsigned int i = 0; i < itemArray.size(); ++i)
		if (itemArray[i] != NULL)
			itemArray[i]->rec.set
			(menuRec.x + itemArray[i]->offset.x
			, menuRec.y + itemArray[i]->offset.y
			, itemArray[i]->rec.width
			, itemArray[i]->rec.height);
}

void Inventory::slideTo(const Vector2 &destination, const float &time)
{
	delete(_curve);
	_curve = new MyCurve(Curve::LINEAR);
	_curve->addPoint(0, menuRec.x, menuRec.y);
	_curve->addPoint(time, destination.x, destination.y);
}

int Inventory::findClosestMatch(ItemIcon* icon)
{
	int bestResult = -1;
	int bestLength = 100000;
	for (unsigned int i = 0; i < itemArray.size(); ++i){
		if (itemArray[i] == NULL){
			Vector2 check
				(i % INVENTORY_COLUMNS * INVENTORY_SLOT_WIDTH * MENU_FIX + menuRec.x + INVENTORY_OFFSET_X * MENU_FIX
				, i / INVENTORY_COLUMNS * INVENTORY_SLOT_HEIGHT * MENU_FIX + menuRec.y + INVENTORY_OFFSET_Y * MENU_FIX);
			check.x -= icon->rec.x;
			check.y -= icon->rec.y;
			if (check.length() < bestLength){
				bestResult = i;
				bestLength = check.length();
			}
		}
	}
	return bestResult;
}

void Inventory::removeItem(ItemIcon* icon)
{
	for (unsigned int i= 0; i < renderList.size(); ++i)
	for (unsigned int j = 0; j < renderList[i].size(); ++j)
	if (renderList[i][j] == icon){
		renderList[i].erase(renderList[i].begin() + j);
		if (renderList[i].size() == 0){
			renderList.erase(renderList.begin() + i);
			
			delete(itemBatch[i]);
			itemBatch.erase(itemBatch.begin() + i);
			textureArray.erase(textureArray.begin() + i);
		}
	}
}

void Inventory::holdItem(Item* item)
{
	item->SetStatus(Item::PREVIEW);
	playField->setFocusedMenu(this); // make sure this has focus if it is holding shit
//	item->addGhostToPlayField(playField, playField->getScene(), item->GetPosition());
	iconTouched = item->GetIcon();
//	iconTouched->ghost = true;
	iconTouched->preview = false;
//	iconTouched->visible = true;
	iconTouched->offset.set(iconTouched->rec.width / -2, iconTouched->rec.height / -2);
	setIconDimensions(iconTouched, -1);
	addToRenderList(iconTouched);

	//AddToEntityList();
	//Prepare();
	//Finalize(playField->getScene());
	
}

void Inventory::addToRenderList(ItemIcon* icon)
{
	bool inRenderList = false;

	for (unsigned int i = 0; i < renderList.size(); ++i)  // check to see if
		for (unsigned int j = 0; j < renderList[i].size(); ++j)  // the icon is already
			if (icon == renderList[i][j])							// in the list to render
				inRenderList = true;								// if it is, make bool true

	if (!inRenderList){
		int indexUsed = -1; // means the texture is not in use
		for (unsigned int i = 0; i < textureArray.size(); ++i)
			if (textureArray[i].compare(icon->texture) == 0)
				indexUsed = i;

		if (indexUsed == -1){ // if the texture is not used already
			textureArray.push_back(icon->texture);
			if (_menuBatch == NULL)
				itemBatch.push_back(NULL);
			else
				itemBatch.push_back(SpriteBatch::create(icon->texture.c_str()));
			std::vector<ItemIcon*> iconsToRender;
			renderList.push_back(iconsToRender);
			renderList[renderList.size() - 1].push_back(icon);
		}
		else{
			renderList[indexUsed].push_back(icon); // if we have the texture already
		}										// put it in the list to sort
	}
}

luabridge::LuaRef Inventory::save()
{
	luabridge::LuaRef itemsTable = luabridge::newTable(L);
	for (int i = 0; i < NUM_OF_ITEM_SLOTS; ++i)
	if (itemArray[i] != NULL)
		itemsTable[i + 1] = itemArray[i]->item->getLuaFile();
	return itemsTable;
}

void Inventory::load(luabridge::LuaRef inventoryRef, LuaLoader* loader)
{
	
	for (int i = 0; i < NUM_OF_ITEM_SLOTS; ++i){
		luabridge::LuaRef itemRef = inventoryRef[i + 1];
		if (!itemRef.isNil())
			addItem(itemRef["luaFile"], loader, i);
	}
}