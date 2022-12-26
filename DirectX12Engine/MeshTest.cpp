//-----------------------------------------------------------------------------
// File : Mesh.cpp
// Desc : Mesh Module.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------
#define NOMINMAX
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "MeshTest.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <codecvt>
#include <cassert>


namespace {

    //-----------------------------------------------------------------------------
    //      UTF-8������ɕϊ����܂�.
    //-----------------------------------------------------------------------------
    std::string ToUTF8(const std::wstring& value)
    {
        auto length = WideCharToMultiByte(
            CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
        auto buffer = new char[length];

        WideCharToMultiByte(
            CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

        std::string result(buffer);
        delete[] buffer;
        buffer = nullptr;

        return result;
    }


    class MeshLoader
    {
        //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */
//=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

    public:
        //=========================================================================
        // public variables.
        //=========================================================================
        /* NOTHING */

        //=========================================================================
        // public methods.
        //=========================================================================
        MeshLoader();
        ~MeshLoader();

        bool Load(
            const wchar_t* filename,
            std::vector<Mesh>& meshes,
            std::vector<Material>& materials);

    private:
        //=========================================================================
        // private variables.
        //=========================================================================
        /* NOTHING */

        //=========================================================================
        // private methods.
        //=========================================================================
        void ParseMesh(Mesh& dstMesh, const aiMesh* pSrcMesh);
        void ParseMaterial(Material& dstMaterial, const aiMaterial* pSrcMaterial);

    };


    //-----------------------------------------------------------------------------
    //      �R���X�g���N�^�ł�.
    //-----------------------------------------------------------------------------
    MeshLoader::MeshLoader()
    { /* DO_NOTHING */
    }

    //-----------------------------------------------------------------------------
    //      �f�X�g���N�^�ł�.
    //-----------------------------------------------------------------------------
    MeshLoader::~MeshLoader()
    { /* DO_NOTHING */
    }

    //-----------------------------------------------------------------------------
//      ���b�V�������[�h���܂�.
//-----------------------------------------------------------------------------
    bool MeshLoader::Load
    (
        const wchar_t* filename,
        std::vector<Mesh>& meshes,
        std::vector<Material>& materials
    )
    {
        if (filename == nullptr)
        {
            return false;

        }
        // wchar_t ���� char�^(UTF-8)�ɕϊ����܂�.

        Assimp::Importer importer;
        int flag = 0;

        flag |= aiProcess_Triangulate;
        flag |= aiProcess_PreTransformVertices;
        flag |= aiProcess_CalcTangentSpace;
        flag |= aiProcess_GenSmoothNormals;
        flag |= aiProcess_GenUVCoords;
        flag |= aiProcess_RemoveRedundantMaterials;
        flag |= aiProcess_OptimizeMeshes;
        // wchar_t ���� char�^(UTF-8)�ɕϊ����܂�.
        auto path = ToUTF8(filename);

        // �t�@�C����ǂݍ���.
        auto pScene = importer.ReadFile(path, flag);

        //�`�F�b�N
        if (pScene == nullptr)
        {
            return false;
        }

        //���b�V���̃��������m��
        meshes.clear();
        meshes.resize(pScene->mNumMeshes);
        //���b�V���f�[�^��ϊ�
        for (size_t i = 0; i < meshes.size(); ++i)
        {
            const auto pMesh = pScene->mMeshes[i];
            ParseMesh(meshes[i], pMesh);
        }

        //�}�e���A���̃��������m��
        materials.clear();
        materials.resize(pScene->mNumMaterials);

        //�}�e���A���f�[�^��ϊ�
        for (size_t i = 0; i < materials.size(); ++i)
        {
            const auto pMaterial = pScene->mMaterials[i];
            ParseMaterial(materials[i], pMaterial);
        }

        //�s�v�ɂȂ����̂ŃN���A
        pScene = nullptr;

        return true;

    }

    //-----------------------------------------------------------------------------
//      ���b�V���f�[�^����͂��܂�.
//-----------------------------------------------------------------------------
    void MeshLoader::ParseMesh(Mesh& dstMesh, const aiMesh* pSrcMesh)
    {
        dstMesh.MaterialId = pSrcMesh->mMaterialIndex;
        aiVector3D zero3D(0.0f, 0.0f, 0.0f);

        //���_�f�[�^�̃��������m��
        dstMesh.Vertices.resize(pSrcMesh->mNumVertices);

        for (auto i = 0u; i < pSrcMesh->mNumVertices; ++i)
        {
            auto pPosition = &(pSrcMesh->mVertices[i]);
            auto pNormal = &(pSrcMesh->mNormals[i]);

            auto pTexCoord = (pSrcMesh->HasTextureCoords(0)) ? &(pSrcMesh->mTextureCoords[0][i]) : &zero3D;
            auto pTangent = (pSrcMesh->HasTangentsAndBitangents()) ? &(pSrcMesh->mTangents[i]) : &zero3D;


            dstMesh.Vertices[i] = MeshVertex(
                DirectX::XMFLOAT3(pPosition->x, pPosition->y, pPosition->z),
                DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z),
                DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y),
                DirectX::XMFLOAT3(pTangent->x, pTangent->y, pTangent->z)
            );
        }

        //���_�C���f�b�N�X�̃��������m��
        dstMesh.Indices.resize(pSrcMesh->mNumFaces * 3);

        for (auto i = 0u; i < pSrcMesh->mNumFaces; ++i)
        {
            const auto& face = pSrcMesh->mFaces[i];
            assert(face.mNumIndices == 3);
            dstMesh.Indices[i * 3 + 0] = face.mIndices[0];
            dstMesh.Indices[i * 3 + 1] = face.mIndices[1];
            dstMesh.Indices[i * 3 + 2] = face.mIndices[2];
        }



        //���_�o�b�t�@�̐���
        {
            auto size = sizeof(MeshVertex) * m_Meshes[0].Vertices.size();
            auto vertices = m_Meshes[0].Vertices.data();

            // �q�[�v�v���p�e�B.
            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type = D3D12_HEAP_TYPE_UPLOAD;
            prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop.CreationNodeMask = 1;
            prop.VisibleNodeMask = 1;

            //���\�[�X�̐ݒ�
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment = 0;
            desc.Width = size;
            desc.Height = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags = D3D12_RESOURCE_FLAG_NONE;


            // ���\�[�X�𐶐�.
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(dstMesh.m_pVB.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            // �}�b�s���O����.
            void* ptr = nullptr;
            hr = dstMesh.m_pVB->Map(0, nullptr, &ptr);
            if (FAILED(hr))
            {
                return false;
            }

            // ���_�f�[�^���}�b�s���O��ɐݒ�.
            memcpy(ptr, vertices, size);

            // �}�b�s���O����.
            dstMesh.m_pVB->Unmap(0, nullptr);

            // ���_�o�b�t�@�r���[�̐ݒ�.
            dstMesh.m_VBV.BufferLocation = m_pVB->GetGPUVirtualAddress();
            dstMesh.m_VBV.SizeInBytes = static_cast<UINT>(size);
            dstMesh.m_VBV.StrideInBytes = static_cast<UINT>(sizeof(MeshVertex));


        }

        // �C���f�b�N�X�o�b�t�@�̐���.
        {
            auto size = sizeof(uint32_t) * m_Meshes[0].Indices.size();
            auto indices = m_Meshes[0].Indices.data();

            // �q�[�v�v���p�e�B.
            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type = D3D12_HEAP_TYPE_UPLOAD;
            prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop.CreationNodeMask = 1;
            prop.VisibleNodeMask = 1;

            // ���\�[�X�̐ݒ�.
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment = 0;
            desc.Width = size;
            desc.Height = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags = D3D12_RESOURCE_FLAG_NONE;

            // ���\�[�X�𐶐�.
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(dstMesh.m_pIB.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            // �}�b�s���O����.
            void* ptr = nullptr;
            hr = dstMesh.m_pIB->Map(0, nullptr, &ptr);
            if (FAILED(hr))
            {
                return false;
            }

            // �C���f�b�N�X�f�[�^���}�b�s���O��ɐݒ�.
            memcpy(ptr, indices, size);

            // �}�b�s���O����.
            dstMesh.m_pIB->Unmap(0, nullptr);

            // �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�.
            dstMesh.m_IBV.BufferLocation = dstMesh.m_pIB->GetGPUVirtualAddress();
            dstMesh.m_IBV.Format = DXGI_FORMAT_R32_UINT;
            dstMesh.m_IBV.SizeInBytes = static_cast<UINT>(size);
        }
    }

    //---------------------------------------------------------------------------- -
        //      �}�e���A���f�[�^����͂��܂�.
        //-----------------------------------------------------------------------------
    void MeshLoader::ParseMaterial(Material& dstMaterial, const aiMaterial* pSrcMaterial)
    {
        //�g�U���ː���
        aiColor3D color(0.0f, 0.0f, 0.0f);

        if (pSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            dstMaterial.Diffuse.x = color.r;
            dstMaterial.Diffuse.y = color.g;
            dstMaterial.Diffuse.z = color.b;
        }
        else
        {
            dstMaterial.Diffuse.x = 0.5f;
            dstMaterial.Diffuse.y = 0.5f;
            dstMaterial.Diffuse.z = 0.5f;
        }

        //���ʔ��ː���
        {
            aiColor3D color(0.0f, 0.0f, 0.0f);

            if (pSrcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
            {
                dstMaterial.Specular.x = color.r;
                dstMaterial.Specular.y = color.g;
                dstMaterial.Specular.z = color.b;
            }
            else
            {
                dstMaterial.Specular.x = 0.0f;
                dstMaterial.Specular.y = 0.0f;
                dstMaterial.Specular.z = 0.0f;

            }
        }

        //���ʔ��ˋ��x
        {
            auto shininess = 0.0f;
            if (pSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
            {
                dstMaterial.Shininess = shininess;
            }
            else
            {
                dstMaterial.Shininess = 0.0f;
            }
        }


        // �f�B�t���[�Y�}�b�v.
        {
            aiString path;
            if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
            {
                dstMaterial.DiffuseMap = std::string(path.C_Str());
            }
            else
            {
                dstMaterial.DiffuseMap.clear();
            }
        }
    }



};

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
#define FMT_FLOAT3      DXGI_FORMAT_R32G32B32_FLOAT
#define FMT_FLOAT2      DXGI_FORMAT_R32G32_FLOAT
#define APPEND          D3D12_APPEND_ALIGNED_ELEMENT
#define IL_VERTEX       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA


const D3D12_INPUT_LAYOUT_DESC MeshVertex::InputLayout = {
    MeshVertex::InputElements,
    MeshVertex::InputElementCount
};
static_assert(sizeof(MeshVertex) == 44, "Vertex struct/layout mismatch");

#undef FMT_FLOAT3
#undef FMT_FLOAT2
#undef APPEND
#undef IL_VERTEX

//-----------------------------------------------------------------------------
//      ���b�V�������[�h���܂�.
//-----------------------------------------------------------------------------
bool LoadMesh
(
    const wchar_t* filename,
    std::vector<Mesh>& meshes,
    std::vector<Material>& materials
)
{
    MeshLoader loader;
    return loader.Load(filename, meshes, materials);
}