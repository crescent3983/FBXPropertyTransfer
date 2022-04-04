#ifndef _MAX_MN_VERTEX_COLOR_WRAPPER_H_
#define _MAX_MN_VERTEX_COLOR_WRAPPER_H_

#include <fbxsdk.h>
#include <max.h>
#include "../PropertyWrapper.h"

class MaxMNVertexColorWrapper : public PropertyWrapper {

private:
	MNMap* _colorMap;
	MNMap* _alphaMap;

public:
	MaxMNVertexColorWrapper(MNMap* colorMap, MNMap* alphaMap);
	~MaxMNVertexColorWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxMNVertexColorWrapper::MaxMNVertexColorWrapper(MNMap* colorMap, MNMap* alphaMap) : _colorMap(colorMap), _alphaMap(alphaMap) {

}

MaxMNVertexColorWrapper::~MaxMNVertexColorWrapper() {

}

void MaxMNVertexColorWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	verts.clear();
	int faceCount = this->_colorMap->FNum();
	int count = 0;
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_colorMap->F(i)->deg;
		for (int j = 0; j < degree; j++) {
			int colorID = this->_colorMap->F(i)->tv[j];
			int alphaID = this->_alphaMap->F(i)->tv[j];
			Point3 color = this->_colorMap->V(colorID);
			Point3 alpha = this->_alphaMap->V(alphaID);
			verts.push_back(FbxVector4(color[0], color[1], color[2], alpha[0]));
		}
	}
}

void MaxMNVertexColorWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_colorMap->setNumVerts(count);
	this->_alphaMap->setNumVerts(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_colorMap->v[i] = Point3(value[0], value[1], value[2]);
		this->_alphaMap->v[i] = Point3(value[3], 0.0, 0.0);
	}
}

void MaxMNVertexColorWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_colorMap->FNum();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = this->_colorMap->F(i)->deg;
	}
}

void MaxMNVertexColorWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	size_t faceCount = degrees.size();
	this->_colorMap->setNumFaces(faceCount);
	this->_alphaMap->setNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		int degree = degrees[i];
		this->_colorMap->F(i)->SetSize(degree);
		this->_alphaMap->F(i)->SetSize(degree);
	}
}

void MaxMNVertexColorWrapper::GetFaceIndexList(std::vector<int> &indices) {
	indices.clear();
	int faceCount = this->_colorMap->FNum();
	int index = 0;
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_colorMap->F(i)->deg;
		for (int j = 0; j < degree; j++) {
			indices.push_back(index++);
		}
	}
}

void MaxMNVertexColorWrapper::SetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_colorMap->FNum();
	int count = 0;
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_colorMap->F(i)->deg;
		for (int j = 0; j < degree; j++) {
			if (count >= indices.size()) {
				// count not match
				return;
			}
			int index = indices[count++];
			this->_colorMap->F(i)->tv[j] = index;
			this->_alphaMap->F(i)->tv[j] = index;
		}
	}
}

#endif