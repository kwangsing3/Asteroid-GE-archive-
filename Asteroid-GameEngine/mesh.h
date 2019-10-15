#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>
#include <shader_m.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
class Meshrender;
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

struct VertexBoneData
{
	unsigned int IDs[4] = { 0,0,0,0 };
	float Weight[4] = { 0,0,0,0 };
	VertexBoneData()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weight, 0, sizeof(Weight));
	};


	void AddBoneData(unsigned int _index, float _flo)
	{
		for (unsigned int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++)
		{
			if (Weight[i] == 0.0f) {
				IDs[i] = _index;
				Weight[i] = _flo;
				return;
			}
		}
	}
};
struct BoneData
{
	std::string Name;

	aiMatrix4x4 _OffsetMat4;
	aiMatrix4x4 FinalTransform;
	BoneData(std::string _N, aiMatrix4x4 _Mat4, aiMatrix4x4 _fin)
	{
		Name = _N; _OffsetMat4 = _Mat4; FinalTransform = _fin;
	}
};



class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;
	vector<BoneData*> vec_BonesData;
	vector<VertexBoneData> _bonsVertex;
	
	aiMatrix4x4 m_GlobalInverseTransform;
	
	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<VertexBoneData> _bsv, vector<BoneData*> _BonesData, aiMatrix4x4 _mat4)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->_bonsVertex = _bsv;
		this->vec_BonesData = _BonesData;
		this->m_GlobalInverseTransform = _mat4;
		
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	// render the mesh
	void Draw(Shader shader, const aiScene* _sc)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		
		shader.setBool("Has_Bone", vec_BonesData.size()>0);



		aiMatrix4x4 Identity;
		float TicksPerSecond = (float)(_sc->mAnimations[0]->mTicksPerSecond != 0 ? _sc->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = glfwGetTime()* TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)_sc->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, _sc->mRootNode, Identity, _sc);
		for (unsigned int i = 0; i < vec_BonesData.size(); i++)
		{
			shader.setMat4("boneTransform[" + std::to_string(i) + "]", aiMatrix4x4ToGlm(vec_BonesData[i]->FinalTransform));
		}
		
		


		// draw mesh
		glBindVertexArray(VAO);
		//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 1);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  Render data  */
	unsigned int VBO, EBO, m_Buffers;
	
	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &m_Buffers);
		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_bonsVertex[0]) * _bonsVertex.size(), &_bonsVertex[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, IDs));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, Weight));
		
		
		glBindVertexArray(0);
	}


	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform, const aiScene* _sce);


	glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
	int findIndex(std::vector<BoneData*> _vecB,std::string name)
	{
		for (unsigned int i = 0; i < _vecB.size(); i++)
		{
			if (_vecB[i]->Name == name)
			{
				return i;
			}
		}

		return -1;
	}

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}


	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}

		

		return 0;
	}

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		

		return 0;
	}

	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		

		return 0;
	}
	aiMatrix4x4 InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
	{
		aiMatrix4x4 m;
		m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
		m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
		return m;
	}
	aiMatrix4x4 InitTranslationTransform(float x, float y, float z)
	{
		aiMatrix4x4 m;
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		return m;
	}
};
#endif