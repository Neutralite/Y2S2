#define _CRT_SECURE_NO_WARNINGS

#include "Text.h"
#include "ResourceManager.h"
#include <iostream>

Text::Text()
{
	TT = TransformType::TYPE_Text;
}

Text::~Text()
{
}

bool Text::init(std::string _font, std::string _material, std::string shader)
{
	if (!_isInit)
	{
		//std::cout << "PASS 1" << std::endl;
		material = rm::getMaterial(_material);
		//std::cout << "PASS 2" << std::endl;
		material->shader = rm::getShader(shader);
		_M_QUAD = rm::getMesh("TEXT_MESH");

		std::ifstream txtFile;
		txtFile.open("../assets/GameText/" + _font + ".txt");
		std::string dataparse;

		std::getline(txtFile, dataparse);
		XYWH.resize(256);

		sscanf(dataparse.c_str(), "%d %d", &tX, &tY);

		//std::cout << dataparse << std::endl;
		//std::cout << tX << ", " << tY << std::endl;
		unsigned char subStep;

		unsigned int _X, _Y, _W, _H;

		while (std::getline(txtFile, dataparse))
		{
			//std::cout << "PAUSE" << std::endl;
			sscanf(dataparse.c_str(), "%c %d %d %d %d", &subStep, &_Y, &_X, &_H, &_W);
			//std::cout << subStep << std::endl;

			XYWH[subStep]
				= vec4((float)_X, (float)_Y, (float)_W, (float)_H);
			//std::cout << XYWH[subStep] << std::endl;
			//dataparse.clear();
		}

		_isInit = true;

		txtFile.close();

		
	}

	return true;
}

void Text::setMessage(std::string _message)
{
	message = _message;
	float messageWidth = 0.f;
	for (unsigned int i = 0; i < _message.size() - 1; i++)
	{
		messageWidth += (XYWH[_message.at(i)].z / XYWH[_message.at(i)].w + XYWH[_message.at(i + 1)].z / XYWH[_message.at(i + 1)].w);
	}
	//messageWidth *= 0.5f;
	individualPos.clear();
	posOffset.clear();
	tS.clear();
	colorShift.clear();
	float letterLoc = -messageWidth * 0.5f;
	for (unsigned int i = 0; i < _message.size(); i++)
	{
		individualPos.push_back(vec3(letterLoc, 0, 0));
		posOffset.push_back(vec3(0.f));
		colorShift.push_back(baseColor);
		tS.push_back(vec3(1.f));
		if (i < _message.size() - 1)
		{
			letterLoc += (XYWH[_message.at(i)].z / XYWH[_message.at(i)].w + XYWH[_message.at(i + 1)].z / XYWH[_message.at(i + 1)].w);
		}
	}
	wordLength = messageWidth;
}

void Text::draw()
{
	if (_M_QUAD && material && !HIDE)
	{
		material->bind();
		material->sendUniforms();
		material->shader->sendUniform("uModel", getLocalToWorld());
		material->shader->sendUniform("uTextSize", vec2((float)tX, (float)tY));
		material->shader->sendUniform("aScale", aS);
		int i = 0;
		for (Texture* texture : material->textures)
		{
			texture->bind(i++);
		}
		for (unsigned int j = 0; j < message.size(); j++)
		{
			material->shader->sendUniform("uTextPos", individualPos[j] + posOffset[j]);
			material->shader->sendUniform("uTexDimensions", XYWH[message.at(j)]);
			material->shader->sendUniform("xScale", XYWH[message.at(j)].z / XYWH[message.at(j)].w);
			material->shader->sendUniform("TotScale", tS[j]);
			material->shader->sendUniform("colorShift", colorShift[j]);
			_M_QUAD->draw();
		}
		for (Texture* texture : material->textures)
		{
			texture->unbind(--i);
		}
	}
}

void Text::update(float dt)
{
	totalUpdateCounter += dt;
	Transform::update(dt);
}

unsigned int Text::messageSize()
{
	return message.size();
}
