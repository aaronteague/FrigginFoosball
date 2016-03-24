#include "MyCamera.h"
#include "FSM.h"
#include "Table.h"
#include "Dependencies\LuaBridge\LuaBridge.h"

// state
#include "States\CameraFollow.h"
#include "States\CameraStage.h"
#include "States\CameraOvermap.h"

bool MyCamera::luaBinded = false;

MyCamera::MyCamera(Scene* _scene, float screenWidth, float screenHeight)
{

	node = _scene->getActiveCamera()->getNode();
	node->getCamera()->setFarPlane(150);
	viewportRec.set(0, 0, screenWidth, screenHeight); 

	defaultPos.set(0, DEFAULT_Y_OFFSET, DEFAULT_HEIGHT);

	node->setTranslation(defaultPos);
	node->setRotation(Matrix::identity());
	node->rotateX(MATH_DEG_TO_RAD(45.46f));

	// bind up that lua
	if (!luaBinded)
	luabridge::getGlobalNamespace(L)
		.beginNamespace("camera")
		.beginClass<MyCamera>("MyCamera")
		.addFunction("SetPosition", &MyCamera::SetPosition)
		.addFunction("LookAt", &MyCamera::LookAt)
		.addFunction("Follow", &MyCamera::Follow)
		.addFunction("SetState", &MyCamera::SetState)
		.addFunction("PanAround", &MyCamera::PanAround)
		.addFunction("PeekUp", &MyCamera::PeekUp)
		.addFunction("ZoomIntoTarget", &MyCamera::ZoomIntoTarget)
		.endClass()
		.endNamespace();
	
	luaBinded = true;
//	stateMachine->SetCurrentState(new CameraStage());
}

Vector2 MyCamera::screenToFieldCoordinates(Vector2 touchPosition)
{
	Camera* camera = node->getScene()->getActiveCamera();
	Vector3 point;
	Vector2 newPoint;
	if (camera){
		// get info about the position

		Matrix m = Matrix::identity();
		Vector3 min(0, 0, 0);
		m.transformPoint(&min);
		// unproject point into world space
		Ray ray;
		camera->pickRay(viewportRec, touchPosition.x, touchPosition.y, &ray);

		// find the quad's plane.  we know its normal is the quads forward vector
		Vector3 normal = Vector3(0, 0, -1);

		// to get the plane's distance from the origin, we'll find the distance from the plane defines
		const float& a = normal.x; const float&b = normal.y; const float& c = normal.z;
		const float d = (-a*min.x) - (b*min.y) - (c*min.z);
		const float distance = abs(d) / sqrt(a*a + b*b + c*c);
		Plane plane(normal, -distance);
		// check for collision with plane.
		float collisionDistance = ray.intersects(plane);
		if (collisionDistance != Ray::INTERSECTS_NONE)
		{
			// multiply the ray's direction vector by collision distance
			Vector3 camOffset = node->getTranslation() - defaultPos;
			//camOffset.z = 0;
			//camOffset.y = 0;
			point.set(ray.getOrigin() - camOffset + collisionDistance*ray.getDirection());
//			point += camOffset;
			m.invert();


			newPoint.set(point.x, point.y);

			if (newPoint.x <= 0.0001f && newPoint.x >= -0.0001f && newPoint.y <= 0.0001f && newPoint.y >= -0.0001f)
				GP_ERROR("the touch is a lie!");

			

			return newPoint;
		}
	}

	GP_ERROR("ouch, we didn't find the plane");
	return 0;

}

bool MyCamera::isTouching(Node* node, int x, int y, const Vector3 dimensions)
{
	Ray ray;
	MyCamera::node->getCamera()->pickRay(viewportRec, x, y, &ray);

	Vector3 trans = node->getTranslation();
	BoundingBox worldSpaceBox
		(trans.x - dimensions.x / 2
		, trans.y - dimensions.y / 2
		, 0
		, trans.x + dimensions.x / 2
		, trans.y + dimensions.y / 2
		, dimensions.z);


	if (ray.intersects(worldSpaceBox) != Ray::INTERSECTS_NONE)
		return true;
	else
		return false;

}

Ray* MyCamera::getPickRay(int x, int y)
{
	MyCamera::node->getCamera()->pickRay(viewportRec, x, y, &pickRay);

	return &pickRay;
}

bool MyCamera::isTouching(Node* node, int x, int y, const float radius)
{
	Ray ray;
	MyCamera::node->getCamera()->pickRay(viewportRec, x, y, &ray);

	Vector3 trans = node->getTranslation();

	BoundingSphere worldSpaceSphere(trans, radius);


	if (ray.intersects(worldSpaceSphere) != Ray::INTERSECTS_NONE)
		return true;
	else
		return false;
}

void MyCamera::Update(const float& elapsedTime)
{
	stateMachine->Update((float&)elapsedTime, true);

	// if pan
	if (panSpeed > 0){
		Vector3 targetPos = target->getLookPosition();
		//float length = (node->getTranslationWorld() - targetPos).length();
		
		curAngle += panSpeed * elapsedTime * 0.01f;

		float x = panRadius * cos(MATH_DEG_TO_RAD(curAngle)) + targetPos.x;
		float y = panRadius * sin(MATH_DEG_TO_RAD(curAngle)) + targetPos.y;

		node->setTranslationX(x);
		node->setTranslationY(y);

		LookAt(target);
	}
}
//
//void MyCamera::SetTarget(Table* table)
//{
//	MyCamera::table = table;
//	
//}

void MyCamera::SetPosition(Vector3 newPosition)
{
	panSpeed = -1;
	Vector3 pos = node->getTranslation();
//	float x = newPosition.x + 0.1f;
	//node->setTranslation(newPosition);
	node->setTranslation(newPosition);
	followNode = NULL;
}

void MyCamera::LookAt(Entity* target)
{
	MyCamera::target = target;
//	node->setTranslation(10, -20, 0);

	Vector3 targetTrans = Vector3(0, 0, 0);
	Vector3 camTrans = node->getTranslation();
	Vector3 direction = camTrans - targetTrans;
	direction.normalize();



	
	
	// get direction quaternion
	Matrix lookAt;
	Vector3 up;
	Vector3 trans;
	node->getMatrix().getUpVector(&up);
	node->getWorldMatrix().getTranslation(&trans);
//	Matrix::createLookAt(trans.x, trans.y, trans.z, 0, 0, 0, 0, 0, 1, &lookAt);
	Matrix::createLookAt(node->getTranslationWorld(), target->getLookPosition(), Vector3(0, 0, 1), &lookAt);
	lookAt.invert();
	////////////////////////////
//	Quaternion quat(lookAt);
	node->setRotation(lookAt);
	
//	node->rotateZ(MATH_DEG_TO_RAD(-1 * turn));
//	node->rotateX(MATH_DEG_TO_RAD(90));
//	node->rotate(quat);
	//
	//node->rotateZ(MATH_RAD_TO_DEG(0));
 // 	node->rotateX(MATH_DEG_TO_RAD(-turn5));
	std::string id(target->getNode()->getId());
	if (id.compare("Pamela") == 0)
	{
//		node->rotateZ(MATH_DEG_TO_RAD(-180));
//		node->rotateX(MATH_DEG_TO_RAD(45));
	}
	else if (id.compare("Field") == 0)
	{
//		node->rotateZ(MATH_DEG_TO_RAD(-90));
//		node->rotateX(MATH_DEG_TO_RAD(45));
		
		
	}
	else // probably the opponent
	{
//		node->rotateZ(MATH_DEG_TO_RAD(-75));
//		node->rotateX(MATH_DEG_TO_RAD(65));
	}
	
}

void MyCamera::Follow(Entity* target)
{
	LookAt(target);
//	followNode = target->getNode();
//	followOffset = target->getNode()->getTranslationWorld() - node->getTranslationWorld();
}

void MyCamera::SetState(std::string state)
{

	//node->setTranslation(defaultPos);
	//node->setRotation(Matrix::identity());
	//node->rotateX(MATH_DEG_TO_RAD(45.46f));

	if (state.compare("Round_Ready") == 0){
		node->setTranslation(defaultPos);
		node->setRotation(Matrix::identity());
		node->rotateX(MATH_DEG_TO_RAD(45.46f));
	}
	else if (state.compare("Overmap") == 0){
	
		stateMachine->SetCurrentState(new CameraOvermap());
	}
	else if (state.compare("Camera_Stage") == 0){
		stateMachine->SetCurrentState(new CameraStage());
	}
}

void MyCamera::PanAround(Entity* target, float speed, float radius)
{
	MyCamera::target = target;
	panSpeed = speed;
	panRadius = radius;
}

void MyCamera::PeekUp(int milliseconds)
{
	MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::PEEK_UP, &milliseconds);
}

void MyCamera::ZoomIntoTarget(int milliseconds)
{
//	int *time = new int(milliseconds); // copy
	MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::ZOOM_INTO_TARGET, &milliseconds);
}