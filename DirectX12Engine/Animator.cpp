#include "Animator.h"
/*
//Tick処理
//今後複数のモデルを動かす際はfor文を使う（そりゃそうだ）
void Animator::CalculateBoneAnimation(const aiScene* pScene, float fTimeInSeconds,const aiNode* pNode)
{
	/*void ProcessBoneNode(const aiAnimation * p_animation,
	const aiScene* pScene, 
		const aiNode* node,
		const DirectX::XMMATRIX& ParentNodeTransform);
		

	
	const aiAnimation* pAnimation = m_model.m_pScene->mAnimations[m_model.mcurrentAnimIndex];
	float TicksPerSecond = (FLOAT)(pAnimation->mTicksPerSecond != 0
		? pAnimation->mTicksPerSecond
		: 25.0f);

	float TimeInTicks = fTimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, static_cast<float>(pAnimation->mDuration));
	DirectX::XMMATRIX Identity = DirectX::XMMatrixIdentity();
	m_model.ProcessBoneNode(pAnimation, m_model.m_pScene, m_model.m_pScene->mRootNode, , DirectX::XMMatrixIdentity());
}


if (VK_SPACE == n16KeyCode)
{// ﾇﾐｻｻｶｯｻｭﾐﾐ
	if (g_stMeshData.m_paiModel->HasAnimations())
	{
	
	g_stMeshData.m_nCurrentAnimIndex = ++g_stMeshData.m_nCurrentAnimIndex % g_stMeshData.m_paiModel->mNumAnimations;
	



FLOAT AnimationTime = fmod(TimeInTicks, (FLOAT)pAnimation->mDuration);



FLOAT TicksPerSecond = (FLOAT)(pAnimation->mTicksPerSecond != 0
		? pAnimation->mTicksPerSecond
		: 25.0f);

	FLOAT TimeInTicks = fTimeInSeconds * TicksPerSecond;
*/
