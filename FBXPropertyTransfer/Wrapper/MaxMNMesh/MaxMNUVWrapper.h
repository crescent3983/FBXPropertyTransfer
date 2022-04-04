#ifndef _MAX_MN_UV_WRAPPER_H_
#define _MAX_MN_UV_WRAPPER_H_

#include <max.h>
#include "..\PropertyWrapper.h"

class MaxMNUVWrapper : public PropertyWrapper {

private:
	MNMap* _map;

public:
	MaxMNUVWrapper(MNMap* map);
	~MaxMNUVWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxMNUVWrapper::MaxMNUVWrapper(MNMap* map) : _map(map) {

}

MaxMNUVWrapper::~MaxMNUVWrapper() {

}

void MaxMNUVWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	int count = this->_map->VNum();
	if (verts.size() != count) {
		verts.resize(count);
	}
	for (int i = 0; i < count; i++) {
		Point3 value = this->_map->V(i);
		verts[i] = FbxVector4(value.x, value.y, value.z, 0.0);
	}
}

void MaxMNUVWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_map->setNumVerts(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_map->v[i] = Point3(value[0], value[1], value[2]);
	}
}

void MaxMNUVWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_map->FNum();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = this->_map->F(i)->deg;
	}
}

void MaxMNUVWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	size_t faceCount = degrees.size();
	this->_map->setNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		this->_map->F(i)->SetSize(degrees[i]);
	}
}

void MaxMNUVWrapper::GetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_map->FNum();
	indices.clear();
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_map->F(i)->deg;
		for (int j = 0; j < degree; j++) {
			indices.push_back(this->_map->F(i)->tv[j]);
		}
	}
}

void MaxMNUVWrapper::SetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_map->FNum();
	int count = 0;
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_map->F(i)->deg;
		for (int j = 0; j < degree; j++) {
			if (count >= indices.size()) {
				// count not match
				return;
			}
			this->_map->F(i)->tv[j] = indices[count++];
		}
	}
}

#endif