/*#ifndef PIVOT_H
#define PIVOT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <Units/Actor.h>
class Pivot
{

	unsigned int PivotVAO, PivotVBO;
	glm::vec3 VertexColor;
public:
	Actor* _actor;
	btRigidBody* body;
	Pivot()
	{
		_actor = new Actor();  // 條適用  記得拿掉
		//transform = new Transform();
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
		glGenVertexArrays(1, &PivotVAO);
		glGenBuffers(1, &PivotVBO);
		glBindBuffer(GL_ARRAY_BUFFER, PivotVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(PIVOTVERTICES), PIVOTVERTICES, GL_STATIC_DRAW);
		glBindVertexArray(PivotVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Bullet Collision
		btVector3 localInertia(0, 0, 0);
		//create a dynamic rigidbody
		btCollisionShape* colShape = new btCapsuleShape(2, 1);
		btScalar mass(0);
		World::collisionShapes.push_back(colShape);
		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		btQuaternion quat;
		quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
		quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
		startTransform.setRotation(quat);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);
		startTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

		body = new btRigidBody(rbInfo);
		body->setCenterOfMassTransform(startTransform);

		int f = body->getCollisionFlags();
		body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

		int _group = 1;
		int _mask = 1;

		World::dynamicsWorld->addRigidBody(body, _group, _mask);

	}
	void Draw(Shader _shader)
	{
		_shader.use();
		_shader.setVec3("viewPos", Window::_editorCamera.transform.position);
		glm::mat4 projection = Window::_editorCamera.Projection;
		glm::mat4 view = Window::_editorCamera.GetViewMatrix();
		_shader.setMat4("projection", projection);
		_shader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
		glm::vec3 _pos(0, 0, 0);
		model = glm::translate(model, _pos);
		glm::quat MyQuaternion;
		glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
		MyQuaternion = glm::quat(EulerAngles);
		glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
		model = model * RotationMatrix;
		model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		_shader.setMat4("model", model);
		_shader.setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);

		glBindVertexArray(PivotVAO);

		glDrawArrays(GL_LINES, 0, 36);
		glBindVertexArray(0);
	}

} _pivot;
*/
#endif // !PIVOT_H
