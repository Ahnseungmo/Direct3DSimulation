#pragma once

class Device : public Singleton<Device>
{
	friend class Singleton;

private:
	Device();
	~Device();

public:
	void Clear();
	void Present();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

private:
	ID3D11Device* device;//CPU - 직렬, 로드, 연산
	ID3D11DeviceContext* deviceContext;//GPU - 병렬, 출력

	IDXGISwapChain* swapChain;//후면 버퍼 관리
	ID3D11RenderTargetView* renderTargetView;//VRAM 후면버퍼 메모리 관리
	ID3D11DepthStencilView* depthStencilView;
};