#pragma once
#include <vector>
#include "TextureLayer.h"

class Object;
class Section
{
public:
	Section();
	Section(Section* _NW, Section* _N, Section* _NE, Section* _E, Section* _SE, Section* _S, Section* _SW, Section* _W);
	Section(std::vector<Section*>_COMPACT);
	~Section();
	
	Section* getNW();
	Section* getN();
	Section* getNE();
	Section* getE();
	Section* getSE();
	Section* getS();
	Section* getSW();
	Section* getW();

	void setNW(Section* SECT);
	void setN(Section* SECT);
	void setNE(Section* SECT);
	void setE(Section* SECT);
	void setSE(Section* SECT);
	void setS(Section* SECT);
	void setSW(Section* SECT);
	void setW(Section* SECT);
	void setNear(Section* _NW, Section* _N, Section* _NE, Section* _E, Section* _SE, Section* _S, Section* _SW, Section* _W);
	void setNear(std::vector<Section*> sects);

	std::vector<Section*>* getCompact();

	void setEdgeBleed(bool B);
	bool doesBleed();

	void setPlane(Object* _OBJ);
	Object* getPlane();

	void setOnFace(Object* _OBJ);
	void removeFromFace(Object* _OBJ);
	std::vector<Object*>* getObjectsOnFace();
	unsigned int getNumObjOnFace();
	Object* getObjectOnFace(int INDEX);

	void setSize(float _sizeX, float _sizeY, float _posX, float _posY);

	void setFilePath(std::string FP);
	std::string getFilePath();
private:
	Section* NW;
	Section* N;
	Section* NE;
	Section* E;
	Section* SE;
	Section* S;
	Section* SW;
	Section* W;
	Object* plane;
	std::vector<Section*> compact;
	float sizeX;
	float sizeY;
	float posX;
	float posY;
	bool edgeBleed = false;
	std::vector<Object*>* objectsOnFace;
	std::string filePath;
};