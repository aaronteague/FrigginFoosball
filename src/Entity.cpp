#include "Entity.h"
#include "Dependencies\LuaBridge\LuaBridge.h"

#include "FSM.h"

std::vector<Effect*> Entity::effectList;

bool Entity::luaBinded = false;

void Entity::HandleMessage(Telegram& msg){
	stateMachine->HandleMessage(msg);
}

Entity::Entity(){
	stateMachine = new FSM(this);

	if (!luaBinded)
		luabridge::getGlobalNamespace(L)
		.beginNamespace("entity")
		.beginClass<Entity>("Entity")
		.endClass()
		.endNamespace();
	luaBinded = true;
}

Material* Entity::buildMaterial(Scene* _scene, Texture* tex, ShaderType shadeType, bool transparent, int numOfJoints)
{
	// might not need "_scene" variable
	Material* _mat = NULL;
	Texture::Sampler* sampler = NULL;
	//	Effect* effect;


	//	Node* lightNode = _scene->findNode("Lamp");
	// create the sampler
	if (shadeType != COLORED && shadeType != SHADOW){
		sampler = Texture::Sampler::create(tex);
		//	sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
		sampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);
	}

	std::string defines;
	if (transparent)
		defines += "MODULATE_ALPHA;";

	// set up various parameters
	if (shadeType == COLORED){
		defines += "VERTEX_COLOR;DIRECTIONAL_LIGHT_COUNT 1;SPECULAR";
		_mat = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", defines.c_str());
	//	_mat->getParameter("u_diffuseColor")->setValue(Vector4(1, 1, 0, 1));
		_mat->getParameter("u_specularExponent")->setValue(20.0f);
		_mat->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);
		_mat->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_WORLD_POSITION);
	}
	else if (shadeType == TEXTURED){
		defines += "DIRECTIONAL_LIGHT_COUNT 1;TEXTURE_DISCARD_ALPHA";
		_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());

	}
	else if (shadeType == TEXTURED_SHADELESS){
		defines += "";
		_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
	}
	else if (shadeType == TEXTURED_SPECULAR){
		defines += "DIRECTIONAL_LIGHT_COUNT 1;TEXTURE_DISCARD_ALPHA;SPECULAR";
		//	_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		_mat->getParameter("u_specularExponent")->setValue(100.0f);
		_mat->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);
		_mat->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_WORLD_POSITION);
	}
	else if (shadeType == TEXTURED_NORMAL){
		defines += "BUMPED;DIRECTIONAL_LIGHT_COUNT 1;TEXTURE_DISCARD_ALPHA;SPECULAR";
		//	_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		_mat = Material::create(buildEffect(shadeType, defines));
		_mat->getParameter("u_specularExponent")->setValue(9.0f);
		_mat->setParameterAutoBinding("u_worldViewMatrix", RenderState::WORLD_VIEW_MATRIX);
		_mat->setParameterAutoBinding("u_cameraPosition", RenderState::CAMERA_WORLD_POSITION);

		_mat->getParameter("u_directionalLightColor[0]")->setValue(Vector3(0.7f, 0.7f, 0.7f));
		_mat->getParameter("u_ambientColor")->setValue(Vector3(0, 0, 0));
	}
	else if (shadeType == TEXTURED_ANIMATED){
		defines += "DIRECTIONAL_LIGHT_COUNT 1;SKINNING;SKINNING_JOINT_COUNT ";
		defines += to_string(numOfJoints);
		//	_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		_mat = Material::create(buildEffect(shadeType, defines));
		_mat->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
		_mat->getParameter("u_ambientColor")->setValue(Vector3(0.7f, 0.7f, 0.7f));

	}
	else if (shadeType == TEXTURED_ANIMATED_NORMAL){
		//	defines += "TEXTURE_DISCARD_ALPHA;BUMPED;DIRECTIONAL_LIGHT_COUNT 1;SKINNING;SKINNING_JOINT_COUNT ";
		defines += "TEXTURE_DISCARD_ALPHA;DIRECTIONAL_LIGHT_COUNT 1 ";
		//	defines += to_string(numOfJoints);
		//	_mat = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		_mat = Material::create(buildEffect(shadeType, defines));
		//	_mat->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);

	}
	else if (shadeType == SHADOW){ // wont be using this
		_mat = Material::create("res/shaders/depth.vert", "res/shaders/depth.frag");
	}

	_mat->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);
	_mat->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX);
	

	_mat->getStateBlock()->setCullFace(true);
	_mat->getStateBlock()->setDepthTest(true);


	// add transparency if needed
	if (transparent){
		_mat->getStateBlock()->setBlend(true);
		if (shadeType == COLORED){
			_mat->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_COLOR);
			_mat->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_COLOR);
		}
		else{
			_mat->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
			_mat->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
		}


		_mat->getParameter("u_modulateAlpha")->setFloat(1.0f);
	}

	if (shadeType != COLORED && shadeType != SHADOW)
		_mat->getParameter("u_diffuseTexture")->setSampler(sampler);

	// get the light out from the actual scene
	//		_mat->getParameter("u_directionalLightColor[0]")->setValue(Vector3::one());
	if (shadeType != TEXTURED_NORMAL && shadeType != TEXTURED_SHADELESS){
		_mat->getParameter("u_directionalLightColor[0]")->setValue(_scene->findNode("Lamp")->getLight()->getColor());
		_mat->getParameter("u_ambientColor")->setValue(Vector3(0.3f, 0.3f, 0.3f));
	}
	
	if (shadeType != TEXTURED_SHADELESS)
		_mat->getParameter("u_directionalLightDirection[0]")->setValue(Vector3(0, -1, -1).normalize());
	else
		_mat->getParameter("u_ambientColor")->setValue(Vector3(0.8f, 0.8f, 0.8f));
	return _mat;
}
Effect* Entity::buildEffect(ShaderType shadeType, const std::string& defines)
{

	// check to see if exists
	for (unsigned int i = 0; i < effectList.size(); ++i)
	if ((std::string("res/shaders/textured.vert;res/shaders/textured.frag;") + defines).compare(effectList[i]->getId()) == 0)
		return effectList[i];

	Effect* _eft;

	// set up various parameters
	if (shadeType == TEXTURED){
		//	defines += "DIRECTIONAL_LIGHT_COUNT 1;TEXTURE_DISCARD_ALPHA";

		_eft = Effect::createFromFile("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
	}
	else if (shadeType == TEXTURED_NORMAL){
		//	defines += "BUMPED;DIRECTIONAL_LIGHT_COUNT 1;TEXTURE_DISCARD_ALPHA";
		_eft = Effect::createFromFile("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
	}
	else if (shadeType == TEXTURED_ANIMATED){
		//	defines += "DIRECTIONAL_LIGHT_COUNT 1;SKINNING;SKINNING_JOINT_COUNT ";
		//		defines += std::to_string(numOfJoints);
		_eft = Effect::createFromFile("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		//		_mat->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
	}
	else if (shadeType == TEXTURED_ANIMATED_NORMAL){
		//	defines += "TEXTURE_DISCARD_ALPHA;BUMPED;DIRECTIONAL_LIGHT_COUNT 1;SKINNING;SKINNING_JOINT_COUNT ";
		//		defines += std::to_string(numOfJoints);
		_eft = Effect::createFromFile("res/shaders/textured.vert", "res/shaders/textured.frag", defines.c_str());
		//		_mat->setParameterAutoBinding("u_matrixPalette", RenderState::MATRIX_PALETTE);
	}
	effectList.push_back(_eft);
	return _eft;
}
std::string Entity::to_string(int number)
{
	std::ostringstream os;
	os << number;
	return os.str();
}
//Vector2 Entity::screenToFieldCoordinates(int x, int y, Scene* _scene, Vector2 screenSize)
//{
//	Camera* camera = _scene->getActiveCamera();
//	Vector3 point;
//	Vector2 newPoint;
//	if (camera){
//		// get info about the position
//
//		Matrix m = Matrix::identity();
//		Vector3 min(0, 0, 0);
//		m.transformPoint(&min);
//		// unproject point into world space
//		Ray ray;
//
//		camera->pickRay(gameplay::Rectangle(screenSize.x, screenSize.y), x, y, &ray);
//
//		// find the quad's plane.  we know its normal is the quads forward vector
//		Vector3 normal = Vector3(0, 0, -1);
//
//		// to get the plane's distance from the origin, we'll find the distance from the plane defines
//		const float& a = normal.x; const float&b = normal.y; const float& c = normal.z;
//		const float d = (-a*min.x) - (b*min.y) - (c*min.z);
//		const float distance = abs(d) / sqrt(a*a + b*b + c*c);
//		Plane plane(normal, -distance);
//		// check for collision with plane.
//		float collisionDistance = ray.intersects(plane);
//		if (collisionDistance != Ray::INTERSECTS_NONE)
//		{
//			// multiply the ray's direction vector by collision distance
//			point.set(ray.getOrigin() + collisionDistance*ray.getDirection());
//
//			m.invert();
//
//
//			newPoint.set(point.x, point.y);
//
//			if (newPoint.x <= 0.0001f && newPoint.x >= -0.0001f && newPoint.y <= 0.0001f && newPoint.y >= -0.0001f)
//				GP_ERROR("the touch is a lie!");
//
//			return newPoint;
//		}
//	}
//
//	GP_ERROR("ouch, we didn't find the plane");
//	return 0;
//}

void Entity::applyNormalMap(Material* material, Texture* tex)
{
	//showing black?  Did you export it from blender to maya and to fbx after?  rotate z-axis -90 degrees
	// oh, and make sure that your texture is being mipmapped.
	Texture::Sampler* normalSampler = Texture::Sampler::create(tex);
	normalSampler->setFilterMode(Texture::NEAREST_MIPMAP_LINEAR, Texture::LINEAR);
	normalSampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);

	material->getParameter("u_normalmapTexture")->setValue(normalSampler);
}

Node* Entity::getNode()
{
	return node;
}

bool Entity::eitherMatch(const PhysicsRigidBody::CollisionPair& pair, Node* check)
{
	if (pair.objectA->getNode() == check || pair.objectB->getNode() == check)
		return true;
	else
		return false;
}