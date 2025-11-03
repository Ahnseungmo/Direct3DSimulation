#include "Framework.h"
#include "ModularModelExporter.h"

ModularModelExporter::ModularModelExporter(string name, string file)
    : name(name), file(file)
{
    importer = new Assimp::Importer();

    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

    UINT flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_PopulateArmatureData;
    scene = importer->ReadFile(file, flags);

    if (!scene)
    {
        string error = importer->GetErrorString();
        MessageBoxA(hWnd, error.c_str(), "Assimp Error", MB_OK);
    }
    assert(scene != nullptr);
}

ModularModelExporter::~ModularModelExporter()
{
    delete importer;
}

void ModularModelExporter::ExportMaterial()
{
    vector<Material*> materials;
    ReadMaterial(materials);
    WriteMaterial(materials);
}

void ModularModelExporter::ExportClip(string clipName)
{
    for (UINT i = 0; i < scene->mNumAnimations; i++)
    {
        Clip* clip = ReadClip(scene->mAnimations[i]);
        WriteClip(clip, clipName, i);
    }
}

void ModularModelExporter::ExportMeshPart(string partName, const vector<UINT>& meshIndices)
{
    if (meshIndices.empty()) return;

    vector<MeshData*> partMeshes;
    vector<NodeData*> partNodes;
    vector<BoneData*> partBones;
    map<string, UINT> boneMap;
    UINT boneCount = 0;

    ReadNode(scene->mRootNode, -1, -1, partNodes);

    for (UINT meshIndex : meshIndices)
    {
        aiMesh* srcMesh = scene->mMeshes[meshIndex];
        MeshData* meshData = new MeshData();
        meshData->name = srcMesh->mName.C_Str();
        meshData->materialIndex = srcMesh->mMaterialIndex;

        vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
        if (srcMesh->HasBones())
        {
            ReadBone(srcMesh, vertexWeights, boneMap, boneCount, partBones);
        }

        meshData->vertices.resize(srcMesh->mNumVertices);
        for (UINT v = 0; v < srcMesh->mNumVertices; v++)
        {
            ModelVertex vertex;
            memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

            if (srcMesh->HasTextureCoords(0))
                memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));
            if (srcMesh->HasNormals())
                memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));
            if (srcMesh->HasTangentsAndBitangents())
                memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

            if (srcMesh->HasBones())
            {
                vertexWeights[v].Normalize();
                vertex.indices.x = (float)vertexWeights[v].indices[0];
                vertex.indices.y = (float)vertexWeights[v].indices[1];
                vertex.indices.z = (float)vertexWeights[v].indices[2];
                vertex.indices.w = (float)vertexWeights[v].indices[3];
                vertex.weights.x = vertexWeights[v].weights[0];
                vertex.weights.y = vertexWeights[v].weights[1];
                vertex.weights.z = vertexWeights[v].weights[2];
                vertex.weights.w = vertexWeights[v].weights[3];
            }
            meshData->vertices[v] = vertex;
        }

        meshData->indices.resize(srcMesh->mNumFaces * 3);
        for (UINT f = 0; f < srcMesh->mNumFaces; f++)
        {
            aiFace& face = srcMesh->mFaces[f];
            for (UINT k = 0; k < face.mNumIndices; k++)
            {
                meshData->indices[f * 3 + k] = face.mIndices[k];
            }
        }
        partMeshes.push_back(meshData);
    }

    WriteMesh(partName, partMeshes, partNodes, partBones);
}

void ModularModelExporter::ReadMaterial(vector<Material*>& materials)
{
    for (UINT i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* srcMaterial = scene->mMaterials[i];
        Material* material = new Material();

        material->SetName(srcMaterial->GetName().C_Str());

        aiColor3D color;
        MaterialBuffer::Data* data = material->GetBuffer()->GetData();

        srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        data->specular = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        data->ambient = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        data->emissive = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess);

        aiString file;
        srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        material->SetDiffuseMap(Utility::ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        material->SetSpecularMap(Utility::ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
        material->SetNormalMap(Utility::ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        materials.push_back(material);
    }
}

void ModularModelExporter::WriteMaterial(const vector<Material*>& materials)
{
    string savePath = "Resources/Models/Materials/" + name + "/";
    string file = name + ".mats";

    Utility::CreateFolders(savePath);

    BinaryWriter* writer = new BinaryWriter(savePath + file);

    writer->UInt(materials.size());

    for (Material* material : materials)
    {
        string path = savePath + material->GetName() + ".mat\0";
        material->Save(path);
        writer->String(path);
        delete material;
    }

    delete writer;
}

string ModularModelExporter::CreateTexture(string file)
{
    if (file.length() == 0)
        return "";

    string fileName = Utility::GetFileNameWithoutExtension(file) + ".png";
    string path = "Resources/Textures/Model/" + name + "/" + fileName;

    Utility::CreateFolders(path);

    const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

    if (texture == nullptr)
        return "";

    if (texture->mHeight < 1)
    {
        BinaryWriter w(path);
        w.Byte(texture->pcData, texture->mWidth);
    }
    else
    {
        Image image;
        image.width = texture->mWidth;
        image.height = texture->mHeight;
        image.pixels = (uint8_t*)(texture->pcData);
        image.rowPitch = image.width * 4;
        image.slicePitch = image.width * image.height * 4;

        SaveToWICFile(image, WIC_FLAGS_NONE,
            GetWICCodec(WIC_CODEC_PNG), Utility::ToWString(path).c_str());
    }

    return path;
}

void ModularModelExporter::ReadNode(aiNode* node, int index, int parent, vector<NodeData*>& nodes)
{
    NodeData* data = new NodeData();
    data->index = index;
    data->name = node->mName.C_Str();
    data->parent = parent;
    Matrix matrix(node->mTransformation[0]);
    data->transform = XMMatrixTranspose(matrix);
    nodes.push_back(data);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadNode(node->mChildren[i], nodes.size(), index, nodes);
}

void ModularModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights, map<string, UINT>& boneMap, UINT& boneCount, vector<BoneData*>& bones)
{
    for (UINT i = 0; i < mesh->mNumBones; i++)
    {
        UINT boneIndex = 0;
        string name = mesh->mBones[i]->mName.C_Str();

        if (boneMap.count(name) == 0)
        {
            boneIndex = boneCount++;
            boneMap[name] = boneIndex;

            BoneData* boneData = new BoneData();
            boneData->name = name;
            boneData->index = boneIndex;
            Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
            boneData->offset = XMMatrixTranspose(matrix);
            bones.push_back(boneData);
        }
        else
        {
            boneIndex = boneMap[name];
        }

        for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
            vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
        }
    }
}

void ModularModelExporter::WriteMesh(string partName, const vector<MeshData*>& meshes, const vector<NodeData*>& nodes, const vector<BoneData*>& bones)
{
    string path = "Resources/Models/Meshes/" + this->name + "/" + partName + ".mesh";
    Utility::CreateFolders(path);

    BinaryWriter* writer = new BinaryWriter(path);

    writer->UInt(meshes.size());
    for (MeshData* mesh : meshes)
    {
        writer->String(mesh->name);
        writer->UInt(mesh->materialIndex);
        writer->UInt(mesh->vertices.size());
        writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());
        writer->UInt(mesh->indices.size());
        writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());
        delete mesh;
    }

    writer->UInt(nodes.size());
    for (NodeData* node : nodes)
    {
        writer->Data<int>(node->index);
        writer->String(node->name);
        writer->Data<int>(node->parent);
        writer->Data<Matrix>(node->transform);
        delete node;
    }

    writer->UInt(bones.size());
    for (BoneData* bone : bones)
    {
        writer->Data<int>(bone->index);
        writer->String(bone->name);
        writer->Data<Matrix>(bone->offset);
        delete bone;
    }

    delete writer;
}

Clip* ModularModelExporter::ReadClip(aiAnimation* animation)
{
    Clip* clip = new Clip();
    clip->name = animation->mName.C_Str();
    clip->tickPerSecond = (float)animation->mTicksPerSecond;
    clip->frameCount = (UINT)(animation->mDuration) + 1;

    vector<ClipNode> clipNodes;
    clipNodes.reserve(animation->mNumChannels);
    for (UINT i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* srcNode = animation->mChannels[i];
        ClipNode node;
        node.name = srcNode->mNodeName;
 
        KeyTransform transform;
 
        for (UINT k = 0; k < clip->frameCount; k++)
        {
            bool isFound = false;
            float t = (float)node.transforms.size();
 
            if (k < srcNode->mNumPositionKeys &&
                GameMath::NearlyEqual(srcNode->mPositionKeys[k].mTime, t))
            {
                aiVectorKey key = srcNode->mPositionKeys[k];
                memcpy_s(&transform.pos, sizeof(Float3),
                    &key.mValue, sizeof(aiVector3D));
 
                isFound = true;
            }
 
            if (k < srcNode->mNumRotationKeys &&
                GameMath::NearlyEqual(srcNode->mRotationKeys[k].mTime, t))
            {
                aiQuatKey key = srcNode->mRotationKeys[k];
 
                transform.rot.x = (float)key.mValue.x;
                transform.rot.y = (float)key.mValue.y;
                transform.rot.z = (float)key.mValue.z;
                transform.rot.w = (float)key.mValue.w;
 
                isFound = true;
            }
 
            if (k < srcNode->mNumScalingKeys &&
                GameMath::NearlyEqual(srcNode->mScalingKeys[k].mTime, t))
            {
                aiVectorKey key = srcNode->mScalingKeys[k];
                memcpy_s(&transform.scale, sizeof(Float3),
                    &key.mValue, sizeof(aiVector3D));
 
                isFound = true;
            }
 
            if (isFound)
                node.transforms.push_back(transform);
        }
 
        if (node.transforms.size() < clip->frameCount)
        {
            UINT count = clip->frameCount - node.transforms.size();
            KeyTransform keyTransform = node.transforms.back();
 
            for (UINT n = 0; n < count; n++)
            {
                node.transforms.push_back(keyTransform);
            }
        }
 
 		clipNodes.push_back(node);
    }

    ReadKeyFrame(clip, scene->mRootNode, clipNodes);

    return clip;
}

void ModularModelExporter::WriteClip(Clip* clip, string clipName, UINT index)
{
    string file = "Resources/Models/Clips/" + name + "/" + clipName + to_string(index) + ".clip";
    Utility::CreateFolders(file);

    BinaryWriter* writer = new BinaryWriter(file);
    writer->String(clip->name);
    writer->Data<UINT>(clip->frameCount);
    writer->Data<float>(clip->tickPerSecond);

    writer->Data<UINT>(clip->keyFrames.size());
    for (KeyFrame* keyFrame : clip->keyFrames)
    {
        writer->String(keyFrame->boneName);
        writer->Data<UINT>(keyFrame->transforms.size());
        writer->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());
        delete keyFrame;
    }

    delete clip;
    delete writer;
}

void ModularModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
    KeyFrame* keyFrame = new KeyFrame();
    keyFrame->boneName = node->mName.C_Str();
    keyFrame->transforms.reserve(clip->frameCount);
    for (UINT i = 0; i < clip->frameCount; i++)
    {
        ClipNode* clipNode = nullptr;
        for (ClipNode& temp : clipNodes)
        {
            if (temp.name == node->mName)
            {
                clipNode = &temp;
                break;
            }
        }

        KeyTransform keyTransform;
        if (clipNode == nullptr)
        {
            Matrix transform(node->mTransformation[0]);
            transform = XMMatrixTranspose(transform);

            Vector3 S, R, T;
            XMMatrixDecompose(S.GetValue(), R.GetValue(), T.GetValue(), transform);
            keyTransform.scale = S;
            XMStoreFloat4(&keyTransform.rot, R);
            keyTransform.pos = T;
        }
        else
        {
            keyTransform = clipNode->transforms[i];
        }

        keyFrame->transforms.push_back(keyTransform);
    }

    clip->keyFrames.push_back(keyFrame);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}