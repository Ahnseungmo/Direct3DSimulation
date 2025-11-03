#include "Framework.h"

Button::Button(Vector2 center, Vector2 size)
	: Rect(center, size)
{
	hFont = CreateFont(20, 0, 0, 0, FW_BOLD, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"배달의민족 꾸불림 TTF");
}

Button::~Button()
{
	DeleteObject(hFont);
	if (hNormalBrush)
		DeleteObject(hNormalBrush);
	if (hOverBrush)
		DeleteObject(hOverBrush);
	if (hDownBrush)
		DeleteObject(hDownBrush);
}

void Button::Update()
{
	if (!isActive) return;

	if (IsCollisionPoint(mousePos))
	{
		state = OVER;

		if (Input::Get()->IsKeyPress(VK_LBUTTON))
		{
			state = DOWN;
			isMouseDown = true;
		}

		if (Input::Get()->IsKeyUp(VK_LBUTTON) && isMouseDown)
		{
			isMouseDown = false;

			if (onClick)
			{
				onClick();
			}

			if (onClickInt)
			{
				onClickInt(intParameter);
			}

			if (onClickObject)
			{
				onClickObject(objectParameter);
			}
		}
	}
	else
	{
		state = NORMAL;
	}

	switch (state)
	{
	case Button::NORMAL:
		hSelectBrush = hNormalBrush;
		break;
	case Button::OVER:
		hSelectBrush = hOverBrush;
		break;
	case Button::DOWN:
		hSelectBrush = hDownBrush;
		break;
	}
}

void Button::Render(HDC hdc)
{
	if (!isActive) return;
	
	HBRUSH defaultBrush = (HBRUSH)SelectObject(hdc, hSelectBrush);

	Rect::Render(hdc);

	SelectObject(hdc, defaultBrush);
	
	RECT rect = { Left(), Top(), Right(), Bottom() };

	HFONT defalutFont = (HFONT)SelectObject(hdc, hFont);

	DrawText(hdc, text.c_str(), text.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, defalutFont);
}

void Button::SetBrush(COLORREF normalColor, COLORREF overColor, COLORREF downColor)
{
	if (hNormalBrush)
		DeleteObject(hNormalBrush);
	hNormalBrush = CreateSolidBrush(normalColor);

	if (hOverBrush)
		DeleteObject(hOverBrush);
	hOverBrush = CreateSolidBrush(overColor);

	if (hDownBrush)
		DeleteObject(hDownBrush);
	hDownBrush = CreateSolidBrush(downColor);
}
