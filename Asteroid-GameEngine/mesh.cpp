#include <mesh.h>
#include <Units/Actor.h>
#include <Component/Meshrender.h>






void Mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode * pNode, const aiMatrix4x4 & ParentTransform, const aiScene * _sce)
{
	string NodeName(pNode->mName.data);

	int BoneIndex = findIndex(vec_BonesData, NodeName);
	const aiAnimation* pAnimation = _sce->mAnimations[0];
	aiMatrix4x4 NodeTransformation(pNode->mTransformation);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
	
	aiMatrix4x4 BindPosMatrix = ParentTransform * NodeTransformation;
	//aiMatrix4x4 AnimationMatrix;
	if (pNodeAnim) 
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		aiMatrix4x4 ScalingM;
		ScalingM = InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		aiMatrix4x4 TranslationM;
		TranslationM = InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}


	if (BoneIndex != -1)
	{
		
		vec_BonesData[BoneIndex]->FinalTransform = BindPosMatrix.Inverse() * AnimationMatrix;
	}
	

	//aiMatrix4x4 _mat = ParentTransform * NodeTransformation;   
	//也沒有計算動畫的話 CurrentBoneWorldMatrix * _mat.Inverse() 是到現在成功的例子

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) 
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], AnimationMatrix, _sce);
	}
	
	
	
}
