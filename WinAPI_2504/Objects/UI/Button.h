#pragma once

class Button : public Rect
{
private:
	enum State
	{
		NORMAL,
		OVER,
		DOWN
	};

public:
	Button(Vector2 center, Vector2 size);
	~Button();

	void Update();
	void Render(HDC hdc);

	void SetBrush(COLORREF normalColor, COLORREF overColor, COLORREF downColor);

	void SetText(wstring text) { this->text = text; }	
	void SetEvent(function<void()> onClick) { this->onClick = onClick; }	
	void SetIntEvent(function<void(int)> onClickInt) { this->onClickInt = onClickInt; }
	void SetIntParameter(int intParameter) { this->intParameter = intParameter; }
	void SetObjectEvent(function<void(void*)> onClickObject) { this->onClickObject = onClickObject; }
	void SetObjectParameter(void* objectParameter) { this->objectParameter = objectParameter; }

private:	
	wstring text = L"";
	State state = NORMAL;
	
	int intParameter = 0;
	void* objectParameter = nullptr;
	bool isMouseDown = false;

	HBRUSH hNormalBrush = nullptr;
	HBRUSH hOverBrush = nullptr;
	HBRUSH hDownBrush = nullptr;

	HBRUSH hSelectBrush = nullptr;

	HFONT hFont = nullptr;

	function<void()> onClick = nullptr;
	function<void(int)> onClickInt = nullptr;
	function<void(void*)> onClickObject = nullptr;
};