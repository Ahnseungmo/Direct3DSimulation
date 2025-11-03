#pragma once

class BinaryReader
{
public:
    BinaryReader(string filePath);
    BinaryReader(wstring filePath);
    ~BinaryReader();

    template <typename T>
    T Data();

    int Int();
    UINT UInt();
    float Float();
    string String();
    wstring WString();

    class Vector2 Vector();

    void Byte(void* data, UINT dataSize);

    bool IsFailed() { return file == INVALID_HANDLE_VALUE; }

private:
    HANDLE file;
    DWORD size = 0;
};

template<typename T>
inline T BinaryReader::Data()
{
    T temp;
    ReadFile(file, &temp, sizeof(T), &size, nullptr);

    return temp;
}
