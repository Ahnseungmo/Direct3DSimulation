#include "Framework.h"

Material::Material(wstring shaderFile)
{
    vertexShader = Shader::AddVS(shaderFile);
    pixelShader = Shader::AddPS(shaderFile);

    materialBuffer = new MaterialBuffer();

	diffuseMap = Texture::Add(L"Resources/Textures/Color/White.png", L"DM");
	specularMap = Texture::Add(L"Resources/Textures/Color/White.png", L"SM");
	normalMap = Texture::Add(L"Resources/Textures/Color/White.png", L"NM");

	char path[128];
	GetCurrentDirectoryA(128, path);
	projectPath = path;
}

Material::~Material()
{
	delete materialBuffer;
}

void Material::Edit()
{
	string title = name + "_Material";

	if (ImGui::TreeNode(title.c_str()))
	{
		char str[128];
		strcpy_s(str, 128, editName.c_str());
		ImGui::InputText("Name", str, 128);
		editName = str;

		if (ImGui::Button("Edit"))
		{
			if (editName.length() > 0)
				name = editName;			
		}

		ImGui::ColorEdit4("mDiffuse", (float*)&materialBuffer->GetData()->diffuse);
		ImGui::ColorEdit4("mSpecular", (float*)&materialBuffer->GetData()->specular);
		ImGui::ColorEdit4("mAmbient", (float*)&materialBuffer->GetData()->ambient);
		ImGui::ColorEdit4("mEmissive", (float*)&materialBuffer->GetData()->emissive);

		ImGui::SliderFloat("Shininess", &materialBuffer->GetData()->shininess, 1, 30);

		SelectMap("DM", Diffuse);
		ImGui::SameLine();
		UnselectMap(Diffuse);

		SelectMap("SM", Specular);
		ImGui::SameLine();
		UnselectMap(Specular);

		SelectMap("NM", Normal);
		ImGui::SameLine();
		UnselectMap(Normal);

		SaveDialog();
		LoadDialog();

		ImGui::TreePop();
	}	
}

void Material::Save(string path)
{
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* material = document->NewElement("Material");
	material->SetAttribute("Name", name.c_str());
	document->InsertFirstChild(material);

	tinyxml2::XMLElement* shader = document->NewElement("Shader");
	string shaderFile;
	if (vertexShader)
		shaderFile = Utility::ToString(vertexShader->GetFile());
	shader->SetAttribute("Vertex", shaderFile.c_str());
	if (pixelShader)
		shaderFile = Utility::ToString(pixelShader->GetFile());
	shader->SetAttribute("Pixel", shaderFile.c_str());
	material->InsertFirstChild(shader);

	tinyxml2::XMLElement* texture = document->NewElement("Texture");

	tinyxml2::XMLElement* map = document->NewElement("DiffuseMap");
	string textureFile = Utility::ToString(diffuseMap->GetFile());
	map->SetAttribute("Diffuse", textureFile.c_str());
	texture->InsertEndChild(map);
	map = document->NewElement("SpecularMap");
	textureFile = Utility::ToString(specularMap->GetFile());
	map->SetAttribute("Specular", textureFile.c_str());
	texture->InsertEndChild(map);
	map = document->NewElement("NormalMap");
	textureFile = Utility::ToString(normalMap->GetFile());
	map->SetAttribute("Normal", textureFile.c_str());
	texture->InsertEndChild(map);
	material->InsertEndChild(texture);

	tinyxml2::XMLElement* property = document->NewElement("Property");
	tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
	diffuse->SetAttribute("R", materialBuffer->GetData()->diffuse.x);
	diffuse->SetAttribute("G", materialBuffer->GetData()->diffuse.y);
	diffuse->SetAttribute("B", materialBuffer->GetData()->diffuse.z);
	diffuse->SetAttribute("A", materialBuffer->GetData()->diffuse.w);
	property->InsertEndChild(diffuse);

	tinyxml2::XMLElement* specular = document->NewElement("Specular");
	specular->SetAttribute("R", materialBuffer->GetData()->specular.x);
	specular->SetAttribute("G", materialBuffer->GetData()->specular.y);
	specular->SetAttribute("B", materialBuffer->GetData()->specular.z);
	specular->SetAttribute("A", materialBuffer->GetData()->specular.w);
	property->InsertEndChild(specular);

	tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
	ambient->SetAttribute("R", materialBuffer->GetData()->ambient.x);
	ambient->SetAttribute("G", materialBuffer->GetData()->ambient.y);
	ambient->SetAttribute("B", materialBuffer->GetData()->ambient.z);
	ambient->SetAttribute("A", materialBuffer->GetData()->ambient.w);
	property->InsertEndChild(ambient);

	tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
	emissive->SetAttribute("R", materialBuffer->GetData()->emissive.x);
	emissive->SetAttribute("G", materialBuffer->GetData()->emissive.y);
	emissive->SetAttribute("B", materialBuffer->GetData()->emissive.z);
	emissive->SetAttribute("A", materialBuffer->GetData()->emissive.w);
	property->InsertEndChild(emissive);

	property->SetAttribute("Shininess", materialBuffer->GetData()->shininess);
	property->SetAttribute("HasNormalMap", materialBuffer->GetData()->hasNormalMap);

	material->InsertEndChild(property);

	document->SaveFile(path.c_str());

	delete document;
}

void Material::Load(string path)
{
	this->file = path;

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	document->LoadFile(file.c_str());
	tinyxml2::XMLElement* material = document->FirstChildElement();
	name = material->Attribute("Name");

	tinyxml2::XMLElement* shader = material->FirstChildElement();
	wstring shaderFile = Utility::ToWString(shader->Attribute("Vertex"));
	vertexShader = Shader::AddVS(shaderFile);
	shaderFile = Utility::ToWString(shader->Attribute("Pixel"));
	pixelShader = Shader::AddPS(shaderFile);

	tinyxml2::XMLElement* texture = shader->NextSiblingElement();
	tinyxml2::XMLElement* map = texture->FirstChildElement();
	wstring textureFile = Utility::ToWString(map->Attribute("Diffuse"));
	SetDiffuseMap(textureFile);
	map = map->NextSiblingElement();
	textureFile = Utility::ToWString(map->Attribute("Specular"));
	SetSpecularMap(textureFile);
	map = map->NextSiblingElement();
	textureFile = Utility::ToWString(map->Attribute("Normal"));
	SetNormalMap(textureFile);

	tinyxml2::XMLElement* property = texture->NextSiblingElement();
	tinyxml2::XMLElement* diffuse = property->FirstChildElement();
	materialBuffer->GetData()->diffuse.x = diffuse->FloatAttribute("R");
	materialBuffer->GetData()->diffuse.y = diffuse->FloatAttribute("G");
	materialBuffer->GetData()->diffuse.z = diffuse->FloatAttribute("B");
	materialBuffer->GetData()->diffuse.w = diffuse->FloatAttribute("A");

	tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
	materialBuffer->GetData()->specular.x = specular->FloatAttribute("R");
	materialBuffer->GetData()->specular.y = specular->FloatAttribute("G");
	materialBuffer->GetData()->specular.z = specular->FloatAttribute("B");
	materialBuffer->GetData()->specular.w = specular->FloatAttribute("A");

	tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
	materialBuffer->GetData()->ambient.x = ambient->FloatAttribute("R");
	materialBuffer->GetData()->ambient.y = ambient->FloatAttribute("G");
	materialBuffer->GetData()->ambient.z = ambient->FloatAttribute("B");
	materialBuffer->GetData()->ambient.w = ambient->FloatAttribute("A");

	tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
	materialBuffer->GetData()->emissive.x = emissive->FloatAttribute("R");
	materialBuffer->GetData()->emissive.y = emissive->FloatAttribute("G");
	materialBuffer->GetData()->emissive.z = emissive->FloatAttribute("B");
	materialBuffer->GetData()->emissive.w = emissive->FloatAttribute("A");

	materialBuffer->GetData()->shininess = property->FloatAttribute("Shininess");
	materialBuffer->GetData()->hasNormalMap = property->IntAttribute("HasNormalMap");

	delete document;
}

void Material::SetColor(float r, float g, float b, float a)
{
	materialBuffer->GetData()->diffuse = { r, g, b, a };
}

void Material::SetShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
	pixelShader = Shader::AddPS(file);
}

void Material::SetVertexShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
}

void Material::SetPixelShader(wstring file)
{
	pixelShader = Shader::AddPS(file);
}

void Material::Set()
{
	if (diffuseMap)
		diffuseMap->PSSet(0);

	if (specularMap)
		specularMap->PSSet(1);

	if (normalMap)
		normalMap->PSSet(2);

	materialBuffer->SetPS(0);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetDiffuseMap(wstring file)
{
	if(file.size() > 0)
		diffuseMap = Texture::Add(file);
	else
		diffuseMap = Texture::Add(L"Resources/Textures/Color/White.png", L"DM");
}

void Material::SetSpecularMap(wstring file)
{
	if (file.size() > 0)
		specularMap = Texture::Add(file);
	else
		specularMap = Texture::Add(L"Resources/Textures/Color/White.png", L"SM");	
}

void Material::SetNormalMap(wstring file)
{
	if (file.size() > 0)
	{
		normalMap = Texture::Add(file);
		materialBuffer->GetData()->hasNormalMap = 1;
	}
	else
	{
		normalMap = Texture::Add(L"Resources/Textures/Color/White.png", L"NM");
		materialBuffer->GetData()->hasNormalMap = 0;
	}

}

void Material::SelectMap(string name, MapType mapType)
{
	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
	ImGui::SetWindowFontScale(1.0f);
	ImGui::SameLine();

	void* textureID = nullptr;

	switch (mapType)
	{
	case Material::Diffuse:
		textureID = (ImTextureID*)diffuseMap->GetSRV();
		break;
	case Material::Specular:
		textureID = (ImTextureID*)specularMap->GetSRV();
		break;
	case Material::Normal:
		textureID = (ImTextureID*)normalMap->GetSRV();
		break;
	default:
		break;
	}

	if (ImGui::ImageButton(name.c_str(), (ImTextureID)textureID, ImVec2(50, 50)))
	{
		DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga,.dds");
	}

	if (DIALOG->Display(this->name + name))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectPath.length() + 1, file.length());

			switch (mapType)
			{
			case Material::Diffuse:
				SetDiffuseMap(Utility::ToWString(file));
				break;
			case Material::Specular:
				SetSpecularMap(Utility::ToWString(file));
				break;
			case Material::Normal:
				SetNormalMap(Utility::ToWString(file));
				break;
			default:
				break;
			}
		}

		DIALOG->Close();
	}
}

void Material::UnselectMap(MapType mapType)
{
	void* textureID = Texture::Add(L"Resources/Textures/UI/Cancel.png")->GetSRV();

	string key;
	switch (mapType)
	{
	case Material::Diffuse:
		key = "DMCancel";
		break;
	case Material::Specular:
		key = "SMCancel";
		break;
	case Material::Normal:
		key = "NMCancel";
		break;
	}

	if (ImGui::ImageButton(key.c_str(), (ImTextureID)textureID, ImVec2(20, 20)))
	{
		switch (mapType)
		{
		case Material::Diffuse:
			SetDiffuseMap(L"");
			break;
		case Material::Specular:
			SetSpecularMap(L"");
			break;
		case Material::Normal:
			SetNormalMap(L"");
			break;
		default:
			break;
		}
	}
}

void Material::SaveDialog()
{
	string key = "Save";

	if (ImGui::Button("Save"))
	{
		if (file.empty())
			Save("Resources/Materials/" + name + ".mat");
		else
			Save(file);
	}

	ImGui::SameLine();

	if (ImGui::Button("SaveAs"))
		DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat");

	if (DIALOG->Display(key.c_str()))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectPath.size() + 1, file.size());

			Save(file);
		}

		DIALOG->Close();
	}
}

void Material::LoadDialog()
{
	string key = "Load";

	if (ImGui::Button("Load"))
	{
		DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat");
	}

	if (DIALOG->Display(key.c_str()))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectPath.size() + 1, file.size());

			Load(file);
		}

		DIALOG->Close();
	}
}
