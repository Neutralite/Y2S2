#include "Section.h"
#include "Object.h"

Section::Section()
{
	NW = nullptr;
	N = nullptr;
	NE = nullptr;
	E = nullptr;
	SE = nullptr;
	S = nullptr;
	SW = nullptr;
	W = nullptr;
	compact.clear();
	for (int i = 0; i < 8; i++)
	{
		compact.push_back(nullptr);
	}
	objectsOnFace = new std::vector<Object*>;
}

Section::Section(Section * _NW, Section * _N, Section * _NE, Section * _E, Section * _SE, Section * _S, Section * _SW, Section * _W)
{
	NW = _NW;
	N = _N;
	NE = _NE;
	E = _E;
	SE = _SE;
	S = _S;
	SW = _SW;
	W = _W;
	compact.clear();
	compact.push_back(_NW);
	compact.push_back(_N);
	compact.push_back(_NE);
	compact.push_back(_E);
	compact.push_back(_SE);
	compact.push_back(_S);
	compact.push_back(_SW);
	compact.push_back(_W);
	objectsOnFace = new std::vector<Object*>;
}

Section::Section(std::vector<Section*> _COMPACT)
{
	compact = _COMPACT;
	NW = compact[0];
	N = compact[1];
	NE = compact[2];
	E = compact[3];
	SE = compact[4];
	S = compact[5];
	SW = compact[6];
	W = compact[7];
	objectsOnFace = new std::vector<Object*>;
}

Section::~Section()
{
}

Section * Section::getNW()
{
	return NW;
}

Section * Section::getN()
{
	return N;
}

Section * Section::getNE()
{
	return NE;
}

Section * Section::getE()
{
	return E;
}

Section * Section::getSE()
{
	return SE;
}

Section * Section::getS()
{
	return S;
}

Section * Section::getSW()
{
	return SW;
}

Section * Section::getW()
{
	return W;
}

void Section::setNW(Section * SECT)
{
	NW = SECT;
}

void Section::setN(Section * SECT)
{
	N = SECT;
}

void Section::setNE(Section * SECT)
{
	NE = SECT;
}

void Section::setE(Section * SECT)
{
	E = SECT;
}

void Section::setSE(Section * SECT)
{
	SE = SECT;
}

void Section::setS(Section * SECT)
{
	S = SECT;
}

void Section::setSW(Section * SECT)
{
	SW = SECT;
}

void Section::setW(Section * SECT)
{
	W = SECT;
}

void Section::setNear(Section* _NW, Section* _N, Section* _NE, Section* _E, Section* _SE, Section* _S, Section* _SW, Section* _W)
{
	NW = _NW;
	N = _N;
	NE = _NE;
	E = _E;
	SE = _SE;
	S = _S;
	SW = _SW;
	W = _W;
	compact.clear();
	compact.push_back(_NW);
	compact.push_back(_N);
	compact.push_back(_NE);
	compact.push_back(_E);
	compact.push_back(_SE);
	compact.push_back(_S);
	compact.push_back(_SW);
	compact.push_back(_W);
}

void Section::setNear(std::vector<Section*> sects)
{
	compact = sects;
	NW = compact[0];
	N = compact[1];
	NE = compact[2];
	E = compact[3];
	SE = compact[4];
	S = compact[5];
	SW = compact[6];
	W = compact[7];
}

std::vector<Section*>* Section::getCompact()
{
	return &compact;
}

void Section::setEdgeBleed(bool B)
{
	edgeBleed = B;
}

bool Section::doesBleed()
{
	return edgeBleed;
}

void Section::setPlane(Object * _OBJ)
{
	plane = _OBJ;
}

Object * Section::getPlane()
{
	return plane;
}

void Section::setOnFace(Object * _OBJ)
{
	objectsOnFace->push_back(_OBJ);
}

void Section::removeFromFace(Object * _OBJ)
{
	for (int i = objectsOnFace->size() - 1; i >= 0; i--)
		if ((*objectsOnFace)[i] == _OBJ)
			objectsOnFace->erase(objectsOnFace->begin() + i);
}

std::vector<Object*>* Section::getObjectsOnFace()
{
	return objectsOnFace;
}

unsigned int Section::getNumObjOnFace()
{
	return objectsOnFace->size();
}

Object * Section::getObjectOnFace(int INDEX)
{
	return (*objectsOnFace)[INDEX];
}

void Section::setSize(float _sizeX, float _sizeY, float _posX, float _posY)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	posX = _posX;
	posY = _posY;
}

void Section::setFilePath(std::string FP)
{
	filePath = FP;
}

std::string Section::getFilePath()
{
	return filePath;
}
