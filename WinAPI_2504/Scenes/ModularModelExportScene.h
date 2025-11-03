#pragma once

class ModularModelExporter;

class ModularModelExportScene : public Scene
{
public:
	ModularModelExportScene();
	~ModularModelExportScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	ModularModelExporter* exporter = nullptr;
	vector<bool> meshCheckStates;
};