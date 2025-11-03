#pragma once

class Utility
{
public:
	static string ToString(wstring str);
	static wstring ToWString(string str);

	static vector<string> SplitString(string origin, string tok, const bool& includeLast = true);
	static void Replace(string& str, const string& comp, const string& rep);

	static bool ExistDirectory(string file);

	static wstring GetExtension(wstring file);
	static string GetFileName(string file);
	static string GetFileNameWithoutExtension(string file);
	
	static void CreateFolders(string file);
};