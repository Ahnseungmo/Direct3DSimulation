#pragma once

class TutorialScene : public Scene
{
public:
	TutorialScene();
	~TutorialScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	
	MatrixBuffer* worldBuffer;
	
	vector<VertexColor> vertices;
	vector<UINT> indices;
};