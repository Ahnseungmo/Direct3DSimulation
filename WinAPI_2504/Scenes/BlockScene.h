#pragma once

class BlockScene : public Scene
{
public:
	BlockScene();
	~BlockScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void LoadTextures();
	void CreateBlocks();

	void Mine();
	void Build();

	Block* GetClosestBlock(Vector3* normal = nullptr);

private:
	vector<Texture*> textures;
	vector<Block*> blocks;

	queue<Block*> mineBlocks;

	Steve* steve;
	LightBuffer::Light* light;
};