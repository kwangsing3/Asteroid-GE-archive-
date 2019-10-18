#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <Assimp/scene.h>
struct AGE_SkeletalAnimation
{
	std::string Name;
	float mTicksPerSecond;
	double mDuration;

	AGE_SkeletalAnimation(std::string _n, double _d)
	{
		Name = _n;
		mDuration = _d;
	}
};



struct AGE_Node
{	
	aiString mName;
	/** The transformation relative to the node's parent. */
	aiMatrix4x4 mTransformation;

	/** Parent node. NULL if this node is the root node. */
	aiNode* mParent;

	/** The number of child nodes of this node. */
	unsigned int mNumChildren;

	/** The child nodes of this node. NULL if mNumChildren is 0. */
	aiNode** mChildren;
	/** The number of meshes of this node. */
	unsigned int mNumMeshes;

	/** The meshes of this node. Each entry is an index into the
	  * mesh list of the #aiScene.
	  */
	unsigned int* mMeshes;

	/** Metadata associated with this node or NULL if there is no metadata.
	  *  Whether any metadata is generated depends on the source file format. See the
	  * @link importer_notes @endlink page for more information on every source file
	  * format. Importers that don't document any metadata don't write any.
	  */
	aiMetadata* mMetaData;

	/** Constructor */
	AGE_Node() {};
	/** Destructor */
	~AGE_Node() {};
	inline
		const aiNode* FindNode(const aiString& name) const {
		return FindNode(name.data);
	}
	inline
		aiNode* FindNode(const aiString& name) {
		return FindNode(name.data);
	}
	const aiNode* FindNode(const char* name) const;
	aiNode* FindNode(const char* name);
	void addChildren(unsigned int numChildren, aiNode** children);
};




#endif // !ANIMATION_H



