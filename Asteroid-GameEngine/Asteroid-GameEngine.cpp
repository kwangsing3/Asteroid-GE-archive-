#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"
#include <Window.h>
#include <model.h>     //導入模型
#include <SceneManager.h>
#include <World.h>
#include <GraphicEngine/GLDebugDrawer.h>
#include <Raycast.h>
//#include<ADD_Component.h>
#include "btBulletDynamicsCommon.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
unsigned int loadCubemap(vector<std::string> faces);


float lastX = 500.0f;
float lastY = 300.0f;
bool isClickingPivot = true;
bool _mouseLucked = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const char* glsl_version = "#version 460";
World* _cur_World;

class _Pivot : public Meshrender
{
	bool _needdebug = true;
	
public:
	btRigidBody* body[3];
	Actor* _lowwerActor;
	bool _attaching = false;
	bool Drag[3] = { false,false,false };
	bool _DragMode[3] = {false,false,false};
	_Pivot(Actor* _a)
	{
		SwitchDragMode(0);
		_actor = _a;
		_actor->meshrender = new Meshrender(_a,NONE);
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreatePivot();
		this->VertexColor = glm::vec3(1, 1, 1);
		//CreateMouseCollision();
		//transform->name = (char*)"Cube";
		//  滑鼠判定的碰撞體
		CreateMouseCollision();
	}
	void CreatePivot()
	{
		_shape = Shape::NONE;
		float PIVOTVERTICES[] = {
		0.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		0.8f,0.1f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		0.8f,-0.1f,0.0f,  1.0f,0.0f,0.0f,// X-Axis
		0.0f,0.0f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f,
		0.1f,0.8f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,0.1f,0.0f,   0.0f,1.0f,0.0f,
		-0.1f,0.8f,0.0f,  0.0f,1.0f,0.0f,// Y-Axis
		0.0f,0.0f,0.0f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		0.1f,0.0f,0.8f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		-0.1f,0.0f,0.8f,  0.0f,0.0f,1.0f,// Z-Axis
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(PIVOTVERTICES), PIVOTVERTICES, GL_STATIC_DRAW);
		glBindVertexArray(0);

		SceneManager::vec_ShaderProgram[0].use();
		SceneManager::vec_ShaderProgram[0].setInt("material.diffuse", 0);
		Worldvectices_Debug = Spacevectices_Debug = Vectices_Debug;
		SceneManager::vec_ObjectsToRender.push_back(this);
	}

	void Draw(Shader _shader) override
	{
		{
			_shader.use();
			_shader.setVec3("viewPos", Window::_editorCamera.transform.position);
			_shader.setFloat("material.shininess", 32.0f);   // 先暫時關掉燈光   確認跟燈光沒關係

			glm::mat4 projection = Window::_editorCamera.Projection;
			glm::mat4 view = Window::_editorCamera.GetViewMatrix();
			_shader.setMat4("projection", projection);
			_shader.setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
			model = glm::translate(model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
			glm::quat MyQuaternion;
			glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
			MyQuaternion = glm::quat(EulerAngles);
			glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
			model = model * RotationMatrix;
			model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
			_shader.setMat4("model", model);
			_shader.setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
			_shader.setBool("shadows", true); // enable/disable shadows by pressing 'SPACE'
			_shader.setBool("reverse_normals", false); // enable/disable shadows by pressing 'SPACE'
		}
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 36);

	}
	void Translate(glm::vec3 _pos)
	{
		this->_actor->transform->position = _pos;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Translate(this->_actor->transform->position);
	}
	void Rotate(glm::vec3 _rot)
	{
		this->_actor->transform->rotation = _rot;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Rotate(this->_actor->transform->rotation);
	}
	void Scale(glm::vec3 _sca)
	{
		this->_actor->transform->scale = _sca;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Scale(this->_actor->transform->scale);
	}
	void AttachObject(Actor* _ac)
	{
		this->_lowwerActor = _ac;
		this->_attaching = true;
		this->Translate(_ac->transform->position);
	}
	void SwitchDragMode(int i)
	{
		_DragMode[0] = false; _DragMode[1] = false; _DragMode[2] = false;
		switch (i)
		{
		case 0:
			_DragMode[0] = true;
			break;
		case 1:
			_DragMode[1] = true;
			break;
		case 2:
			_DragMode[2] = true;
			break;
		default:
			break;
		}
	}
	

private:
	int Collision_flag = 0;
	void CreateMouseCollision()
	{
		btTransform startTransform[3]; startTransform[0].setIdentity(); startTransform[1].setIdentity(); startTransform[2].setIdentity();

		glm::mat4 _pos[3];  _pos[0] = glm::mat4(1.0f); _pos[1] = glm::mat4(1.0f); _pos[2] = glm::mat4(1.0f);
		
		_pos[1] = glm::rotate(_pos[1], glm::radians(-this->_actor->transform->rotation.y), glm::vec3(0, 1, 0));
		_pos[1] = glm::rotate(_pos[1], glm::radians(this->_actor->transform->rotation.x), glm::vec3(1, 0, 0));
		_pos[1] = glm::rotate(_pos[1], glm::radians(this->_actor->transform->rotation.z), glm::vec3(0, 0, 1));
		
		_pos[0] = glm::translate(_pos[0], glm::vec3(this->_actor->transform->position.x + .5f, this->_actor->transform->position.y, this->_actor->transform->position.z));
		_pos[1] = glm::translate(_pos[1], glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y + .5f, this->_actor->transform->position.z));
		_pos[2] = glm::translate(_pos[2], glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z + .5f));

		startTransform[0].setFromOpenGLMatrix(glm::value_ptr(_pos[0])); startTransform[1].setFromOpenGLMatrix(glm::value_ptr(_pos[1])); startTransform[2].setFromOpenGLMatrix(glm::value_ptr(_pos[2]));

		//glm::vec3 _pos0(this->_actor->transform->position.x + .5f, this->_actor->transform->position.y, this->_actor->transform->position.z);
		//_pos0 = glm::rotate(this->_actor->transform->rotation.x ,glm::vec3(1,0,0)) * _pos0;

		/*btQuaternion quat;
		//quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
		quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
		
		startTransform[0].setRotation(quat);
		startTransform[1].setRotation(quat);
		startTransform[2].setRotation(quat);
		
		startTransform[0].setOrigin(btVector3(this->_actor->transform->position.x + .5f, this->_actor->transform->position.y, this->_actor->transform->position.z));
		startTransform[1].setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y + .5f, this->_actor->transform->position.z));
		startTransform[2].setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z + .5f));*/
		btVector3 localInertia(0, 0, 0);
		//create a dynamic rigidbody
		btCollisionShape* colShape0 = new btCapsuleShapeX(0.08f, 0.55f);
		btScalar mass(0);
		World::collisionShapes.push_back(colShape0);
		bool isDynamic = (mass != 0.f);
		if (isDynamic)
			colShape0->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform[0]);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape0, localInertia);
		body[0] = new btRigidBody(rbInfo);
		//body[0]->setCenterOfMassTransform(startTransform[0]);
		body[0]->_actor = this->_actor;
		Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
		body[0]->setCollisionFlags(Collision_flag);
		int _group = 1;
		int _mask = 1;
		World::dynamicsWorld->addRigidBody(body[0], _group, _mask);
		
		
		btVector3 localInertia1(0, 0, 0);
		//create a dynamic rigidbody
		btCollisionShape* colShape1 = new btCapsuleShape(0.08f, 0.55f);
		btScalar mass1(0);
		World::collisionShapes.push_back(colShape1);
		isDynamic = (mass1 != 0.f);
		if (isDynamic)
			colShape1->calculateLocalInertia(mass1, localInertia);
		btDefaultMotionState* myMotionState1 = new btDefaultMotionState(startTransform[1]);
		btRigidBody::btRigidBodyConstructionInfo rbInfo1(mass1, myMotionState1, colShape1, localInertia1);
		body[1] = new btRigidBody(rbInfo1);
		//body[1]->setCenterOfMassTransform(startTransform[1]);
		body[1]->_actor = this->_actor;
		Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
		body[1]->setCollisionFlags(Collision_flag);
		
		World::dynamicsWorld->addRigidBody(body[1], _group, _mask);
		
		
		btVector3 localInertia2(0, 0, 0);
		//create a dynamic rigidbody
		btCollisionShape* colShape2 = new btCapsuleShapeZ(0.08f, 0.55f);
		btScalar mass2(0);
		World::collisionShapes.push_back(colShape2);
		isDynamic = (mass2 != 0.f);
		if (isDynamic)
			colShape2->calculateLocalInertia(mass2, localInertia);
		btDefaultMotionState* myMotionState2 = new btDefaultMotionState(startTransform[2]);
		btRigidBody::btRigidBodyConstructionInfo rbInfo2(mass2, myMotionState2, colShape2, localInertia2);
		body[2] = new btRigidBody(rbInfo2);
		//body[2]->setCenterOfMassTransform(startTransform[2]);
		body[2]->_actor = this->_actor;
		Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
		body[2]->setCollisionFlags(Collision_flag);
		
		World::dynamicsWorld->addRigidBody(body[2], _group, _mask);
	}
	void UpdateCollision() override
	{
		World::dynamicsWorld->removeCollisionObject(body[0]);
		World::dynamicsWorld->removeCollisionObject(body[1]);
		World::dynamicsWorld->removeCollisionObject(body[2]);
		CreateMouseCollision();
	}
} ;
_Pivot* _piv;




int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	Window *_mainWindow = new Window(framebuffer_size_callback, mouse_move_callback, scroll_callback, mouse_button_callback);
	_mainWindow->DeBug_Mode = true;

	SceneManager _sceneManager;   // 我的ShaderProgram在建構函數中創建   目前需要在world的類別前宣告
	_cur_World = new World();
	//UI 初始化-------------
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); //(void)io;
		ImFontConfig font_config; font_config.OversampleH = 1; font_config.OversampleV = 1; font_config.PixelSnapH = 1;
//		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 15.0f, &font_config, io.Fonts->GetGlyphRangesChineseFull());
//		io.Fonts->Build();


		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGui::StyleColorsLight();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(_mainWindow->MainGLFWwindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	bool show_demo_window = true;
	bool show_another_window = false;



	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_MULTISAMPLE);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_CULL_FACE);               //先把反面剃除拿掉
	//glEnable(GL_FRAMEBUFFER_SRGB); //gamma校正
	// build and compile our shader zprogram
	// ------------------------------------






	
	unsigned int AxisVAO, AxisVBO;
	//  座標的點數資料
	float coordinate[] = {
		// positions         // colors
		   2.5f, 0.0f, 0.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, 0.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, 0.0f, 0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, 0.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, -0.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -0.5f, 0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f, 1.0f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, 1.0f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, -1.0f, 0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, -1.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, 1.5f,  0.7f,0.7f,0.7f, // bottom left
		  -2.5f, -0.0f, 1.5f,  0.7f,0.7f,0.7f,   // top  
		   2.5f, 0.0f, -1.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.5f, 0.0f, -1.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.5f, -0.0f, 2.0f,  0.7f,0.7f,0.7f,  // top  
		  -2.5f, -0.0f, 2.0f,  0.7f,0.7f,0.7f,  // bottom right
		   2.5f, 0.0f, -2.0f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -2.0f,  0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f,  2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,   // top  
		  2.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, -0.0f, -2.5f, 0.7f,0.7f,0.7f,   // top  

		   0.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom right
		   0.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   0.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		   0.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -0.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -0.5f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top  
		  1.0f, 0.0f, 2.5f,    0.7f,0.7f,0.7f,  // bottom right
		  1.0f, 0.0f, -2.5f,    0.7f,0.7f,0.7f,  // bottom left
		   -1.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		  -1.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		   1.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   1.5f, -0.0f, -2.5f, 0.7f,0.7f,0.7f,   // top  
		   -1.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -1.5f, 0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom left
		   2.0f, -0.0f, 2.5f,  0.7f,0.7f,0.7f,  // top  
		  2.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,  // bottom right
		  -2.0f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.0f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top  
		   2.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  2.5f, -0.0f, -2.5f,  0.7f,0.7f,0.7f,   // top 
		  -2.5f, 0.0f, 2.5f,  0.7f,0.7f,0.7f,  // bottom left
		  -2.5f, 0.0f,  -2.5f,  0.7f,0.7f,0.7f   // top 
	};

	glGenVertexArrays(1, &AxisVAO);
	glGenBuffers(1, &AxisVBO);
	glBindBuffer(GL_ARRAY_BUFFER, AxisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate), coordinate, GL_STATIC_DRAW);
	glBindVertexArray(AxisVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//---------------------------------------
	
	//記得拿掉
	SceneManager::OpenFile();//調試用函數
	//記得拿掉
	GLDebugDrawer* _deb = new GLDebugDrawer();
	_cur_World->dynamicsWorld->setDebugDrawer(_deb);
	
	_piv = new _Pivot(ADD_Component::Add_Actor());
	

	while (!Window::WindowShouldClose)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(_mainWindow->MainGLFWwindow);
		//  Game scene	
		//glBindFramebuffer(GL_FRAMEBUFFER, Window::_editorCamera.GetframeBuffer());		
		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.24f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//Draw Croodinate  基本座標(白)
		{
			SceneManager::vec_ShaderProgram[0].use();
			SceneManager::vec_ShaderProgram[0].setMat4("projection", Window::_editorCamera.Projection);
			glm::mat4 view = Window::_editorCamera.GetViewMatrix();
			SceneManager::vec_ShaderProgram[0].setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			if (WindowUI::_mode == Mode_2D)
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			else
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
			SceneManager::vec_ShaderProgram[0].setMat4("model", model);

			SceneManager::vec_ShaderProgram[0].setVec3("Color", 1, 1, 1);

			glBindVertexArray(AxisVAO);
			glDrawArrays(GL_LINES, 0, 44);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//世界的畫面刷新

		_cur_World->UpdateFrame();
		//_deb->drawLine(btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(0, 0,0),btVector3(1,0,0));

		// 正在解決MSAA 抗鋸齒  想辦法把抗鋸齒用到幀緩衝上

		
		glDisable(GL_DEPTH_TEST);
		//UI----------------------------------------------------------------------------------------------------------------------

		WindowUI::ShowMyImGUIDemoWindow(&show_demo_window, &Window::_Width, &Window::_Height, Window::_editorCamera.GetframeBuffer());


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(_mainWindow->MainGLFWwindow);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Window::WindowShouldClose = true;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			Window::_editorCamera.ProcessKeyboard(FORWARD, deltaTime);
		else
			_piv->SwitchDragMode(0);
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			Window::_editorCamera.ProcessKeyboard(UP, deltaTime);
		else
			_piv->SwitchDragMode(1);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Window::_editorCamera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
		WindowUI::Deletecur_actor(WindowUI::cur_SelectObject);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		WindowUI::Renamecur_actor(WindowUI::cur_SelectObject);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
			_piv->SwitchDragMode(2);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	if (isClickingPivot && _piv != NULL)
	{
		if (_piv->Drag[0])
		{
			if(_piv->_DragMode[0])
				_piv->Translate(glm::vec3(_piv->_actor->transform->position.x + xoffset / 200, _piv->_actor->transform->position.y, _piv->_actor->transform->position.z));
			else if (_piv->_DragMode[1])
				_piv->Rotate(glm::vec3(_piv->_actor->transform->rotation.x + xoffset / 20, _piv->_actor->transform->rotation.y, _piv->_actor->transform->rotation.z));
			else if (_piv->_DragMode[2])
				_piv->Scale(glm::vec3(_piv->_actor->transform->scale.x + xoffset / 200, _piv->_actor->transform->scale.y, _piv->_actor->transform->scale.z));
		}
		else if (_piv->Drag[1])
		{
			
			if (_piv->_DragMode[0])
				_piv->Translate(glm::vec3(_piv->_actor->transform->position.x , _piv->_actor->transform->position.y + yoffset / 200, _piv->_actor->transform->position.z));
			else if (_piv->_DragMode[1])
				_piv->Rotate(glm::vec3(_piv->_actor->transform->rotation.x, _piv->_actor->transform->rotation.y + yoffset / 20, _piv->_actor->transform->rotation.z));
			else if (_piv->_DragMode[2])
				_piv->Scale(glm::vec3(_piv->_actor->transform->scale.x, _piv->_actor->transform->scale.y + yoffset / 200, _piv->_actor->transform->scale.z));
		}
			
		else if (_piv->Drag[2])
		{
			
			if (_piv->_DragMode[0])
				_piv->Translate(glm::vec3(_piv->_actor->transform->position.x , _piv->_actor->transform->position.y, _piv->_actor->transform->position.z + xoffset / 200));
			else if (_piv->_DragMode[1])
				_piv->Rotate(glm::vec3(_piv->_actor->transform->rotation.x , _piv->_actor->transform->rotation.y, _piv->_actor->transform->rotation.z + xoffset / 20));
			else if (_piv->_DragMode[2])
				_piv->Scale(glm::vec3(_piv->_actor->transform->scale.x , _piv->_actor->transform->scale.y, _piv->_actor->transform->scale.z + xoffset / 200));
		}
			
		
	}
	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		Window::_editorCamera.ProcessMouseMovement(xoffset, yoffset);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//記得拿掉

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
		
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		
		Raycast::SetMousePosition(lastX, lastY + Window::_Height / 45);
		btCollisionWorld::ClosestRayResultCallback RayCallback(
			btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z)
		);
		RayCallback.m_collisionFilterMask = 1;
		RayCallback.m_collisionFilterGroup = 1;

		_cur_World->dynamicsWorld->rayTest(
			btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z),
			RayCallback
		);


		if (RayCallback.hasHit()) {
			std::ostringstream oss;
			oss << "mesh " << RayCallback.m_collisionObject;
			//_cur_World->dynamicsWorld.
			if (RayCallback.m_collisionObject->_actor->meshrender->_shape==NONE)
			{		
				_piv->_actor = RayCallback.m_collisionObject->_actor;
			
				_piv->Drag[0] = false; _piv->Drag[1] = false; _piv->Drag[2] = false;
				if (RayCallback.m_collisionObject == _piv->body[0])
					_piv->Drag[0] = true;
				else if (RayCallback.m_collisionObject == _piv->body[1])
					_piv->Drag[1] = true;
				else if(RayCallback.m_collisionObject == _piv->body[2])
					_piv->Drag[2] = true;
				isClickingPivot = true;
			}
			else
			{
				if(_piv!=NULL)_piv->AttachObject(RayCallback.m_collisionObject->_actor);
				WindowUI::SelectThisActor(RayCallback.m_collisionObject->_actor);
			}
			
			
		}
		else {
			WindowUI::SelectThisActor(NULL);
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		isClickingPivot = false;
		//_piv->_actor = NULL;
	}
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window::_editorCamera.ProcessMouseScroll(yoffset);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


