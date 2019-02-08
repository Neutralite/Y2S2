#pragma once
#include "Texture.h"

class TextureLayer
{
public:
	TextureLayer() {};
	~TextureLayer() {};
	Texture* getTexture() { return texture; };
	void setTexture(Texture* _T) { texture = _T; }

	float getOffsetX() { return offsetX; }
	float getOffsetY() { return offsetY; }
	void setOffsetX(float _X) { offsetX = _X; }
	void setOffsetY(float _Y) { offsetY = _Y; }

	float getTransparency() { return transparency; }
	void setTransparency(float _T) { transparency = _T; }

	unsigned int getDepth() { return depth; }
	void setDepth(unsigned int _UI) { depth = _UI; }

	std::string getTextureName() { return textureName; }
	void setTextureName(std::string _TXNAME) { textureName = _TXNAME; }

private:
	Texture* texture;
	std::string textureName;
	float transparency = 1.f;
	float offsetX = 0.f;
	float offsetY = 0.f;
	unsigned int depth = 0;
};