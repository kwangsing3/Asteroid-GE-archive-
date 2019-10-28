#ifndef SKELETALMESH_H
#define SKELETALMESH_H
#include <AGE_Mesh.h>
#include <assimp/scene.h>
class aiScene;
struct Vertex_hasBone
{
	unsigned int IDs[4] = { 0,0,0,0 };
	float Weight[4] = { 0,0,0,0 };
	Vertex_hasBone()
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
	glm::mat4 FinalTransform;
	BoneData(std::string _N, aiMatrix4x4 _Mat4)
	{
		Name = _N; _OffsetMat4 = _Mat4; FinalTransform = glm::mat4(1.0f);
	}
};

class SkeletalMesh: public Mesh  //傳入已經分配好的數值來製作SkeletalMesh
{
protected:

	glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

private:
	unsigned int VBO, EBO, m_Buffers;
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec_VertexWithBones[0]) * vec_VertexWithBones.size(), &vec_VertexWithBones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex_hasBone), (void*)offsetof(Vertex_hasBone, IDs));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_hasBone), (void*)offsetof(Vertex_hasBone, Weight));


		glBindVertexArray(0);
	}
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4 ParentTransform, const aiScene* _sce);
	int findIndex(std::vector<BoneData*> _vecB, std::string name);
	const aiScene* _aiScene;

public:
	std::vector<Vertex_hasBone> vec_VertexWithBones;
	std::vector<BoneData*> vec_BonesData;
	std::vector<aiAnimation*> _Animation;
	
	SkeletalMesh(std::vector<Vertex> vert, std::vector<unsigned int> indi, std::vector<Texture> _Text, std::vector<Vertex_hasBone> vec_Vert, std::vector<BoneData*> _BonD, const aiScene* _sce)
	{
		this->vertices = vert;
		this->indices = indi;
		this->_Textures = _Text;
		this->vec_VertexWithBones = vec_Vert;
		this->vec_BonesData = _BonD;
		this->_aiScene = _sce;
		Has_Bone = !this->vec_BonesData.empty();
		setupMesh();
	}

	void Draw(Shader* shader);
};


#endif // !SKELETALMESH_H



