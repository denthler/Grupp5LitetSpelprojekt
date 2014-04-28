#pragma once

class Screen
{
	bool visible;

public:
	Screen() { visible = true; }
	bool IsVisible() { return visible; }
	void Visible(){ visible = true; }
	void Hide(){ visible = false; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
};