#ifndef _MAX_UV_WRAPPER_H_
#define _MAX_UV_WRAPPER_H_

#include <fbxsdk.h>
#include <max.h>
#include "../PropertyWrapper.h"

class MaxUVWrapper : public PropertyWrapper {

private:
	MeshMap& _map;

public:
	MaxUVWrapper(MeshMap& map);
	~MaxUVWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxUVWrapper::MaxUVWrapper(MeshMap& map) : _map(map){

}

MaxUVWrapper::~MaxUVWrapper(){

}

void MaxUVWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	int count = this->_map.getNumVerts();
	if (verts.size() != count) {
		verts.resize(count);
	}
	for (int i = 0; i < count; i++) {
		UVVert value = this->_map.tv[i];
		verts[i] = FbxVector4(value[0], value[1], value[2], 0.0);
	}
}

void MaxUVWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_map.setNumVerts(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_map.tv[i] = Point3(value[0], value[1], value[2]);
	}
}

void MaxUVWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_map.getNumFaces();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = TRIMESH_FACE_VERTEX;
	}
}

void MaxUVWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	// do nothing, mesh can only be composed by triangles (3 degree)
}

void MaxUVWrapper::GetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_map.getNumFaces();
	int totalCount = faceCount * TRIMESH_FACE_VERTEX;
	if (indices.size() != totalCount) {
		indices.resize(totalCount);
	}
	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			indices[i * TRIMESH_FACE_VERTEX + j] = this->_map.tf[i].t[j];
		}
	}
}

void MaxUVWrapper::SetFaceIndexList(std::vector<int> &indices) {
	size_t faceCount = indices.size() / TRIMESH_FACE_VERTEX;
	this->_map.setNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			this->_map.tf[i].t[j] = indices[i * TRIMESH_FACE_VERTEX + j];
		}
	}
}

#endif