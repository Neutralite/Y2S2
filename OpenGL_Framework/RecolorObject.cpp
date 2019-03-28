#include "RecolorObject.h"

RecolorObject::RecolorObject()
{
	TT = TYPE_Recolor;
}

RecolorObject::~RecolorObject()
{
}

void RecolorObject::draw()
{
	if (getMesh() && getMaterial() && !HIDE)
	{
		getMaterial()->bind();
		getMaterial()->sendUniforms();
		getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		getShader()->sendUniform("newColor", objectColor);
		int i = 0;
		for (Texture* texture : *(getTextures()))
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		getMesh()->draw();
		for (Texture* texture : *(getTextures()))
		{
			texture->unbind(--i);
		}
	}
}
