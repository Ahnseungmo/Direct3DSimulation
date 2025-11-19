#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CENTER_X (SCREEN_WIDTH >> 1)
#define CENTER_Y (SCREEN_HEIGHT >> 1)

#define CENTER Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f)

#define PI 3.141592f

#define MAX_LIGHT 16
#define MAX_FRAME 256
#define MAX_BONE 256
#define MAX_INSTANCE 128

#define FOR(n) for(int i = 0 ; i < n ; i++)

#define DELTA Timer::Get()->GetElapsedTime()
#define SCENE SceneManager::Get()

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define CAM Environment::Get()->GetMainCamera()

#define DIALOG ImGuiFileDialog::Instance()

#include <windows.h>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <queue>
#include <iterator>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

//FMOD
#include <FMOD/fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mtd.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMMATRIX Matrix;
typedef XMVECTOR Vector4;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;

#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/Input.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/Audio.h"

#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"

#include "Framework/Device/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"

#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"
#include "Framework/Buffer/GlobalBffer.h"

#include "Framework/Stats/RasterizerState.h"
#include "Framework/Stats/DepthStencilState.h"
#include "Framework/Stats/BlendState.h"
#include "Framework/Stats/SamplerState.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/DepthStencil.h"
#include "Framework/Render/RenderTarget.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Refraction.h"
#include "Framework/Environment/Shadow.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModularModelExporter.h"

#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Plane.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Grid.h"

#include "Objects/Collider/Collider.h"
#include "Objects/Collider/BoxCollider.h"
#include "Objects/Collider/SphereCollider.h"
#include "Objects/Collider/CapsuleCollider.h"

#include "Objects/Model/Model.h"
#include "Objects/Model/ModelAnimator.h"
#include "Objects/Model/ModelInstancing.h"
#include "Objects/Model/ModelAnimatorInstancing.h"

#include "Objects/Manger/DataManager.h"
#include "Objects/Manger/EventManager.h"

#include "Objects/Landscape/Skybox.h"
#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/Water.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Sprite.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Snow.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/ParticleSystem.h"

#include "Objects/Game/GameBall.h"
#include "Objects/Game/Block.h"
#include "Objects/Game/Steve.h"
#include "Objects/Game/Sword.h"
#include "Objects/Game/InteriorObject.h"
#include "Objects/Game/InteriorManager.h"

#include "Objects/Game/Room.h"
#include "Objects/Game/RoomManager.h"

#include "Objects/Game/ItemPreset.h"
#include "Objects/Game/Item.h"
#include "Objects/Game/ItemManager.h"

#include "Objects/Game/TableSet.h"


#include "Objects/Character/Naruto.h"

#include "Scenes/Scene.h"
#include "Framework/Manager/SceneManager.h"
#include "Framework/Manager/GameManager.h"

extern HWND hWnd;
extern Vector3 mousePos;