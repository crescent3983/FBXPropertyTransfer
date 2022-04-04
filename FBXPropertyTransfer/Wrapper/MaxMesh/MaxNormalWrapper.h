#ifndef _MAX_NORMAL_WRAPPER_H_
#define _MAX_NORMAL_WRAPPER_H_

#include <fbxsdk.h>
#include <max.h>
#include <MeshNormalSpec.h>
#include "../PropertyWrapper.h"

class MaxNormalWrapper : public PropertyWrapper {

private:
	MeshNormalSpec* _normal;

public:
	MaxNormalWrapper(MeshNormalSpec* normal);
	~MaxNormalWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxNormalWrapper::MaxNormalWrapper(MeshNormalSpec* normal) : _normal(normal){

}

MaxNormalWrapper::~MaxNormalWrapper(){

}

void MaxNormalWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	int count = this->_normal->GetNumNormals();
	if (verts.size() != count) {
		verts.resize(count);
	}
	for (int i = 0; i < count; i++) {
		Point3 value = this->_normal->Normal(i);
		verts[i] = FbxVector4(value.x, value.y, value.z, 0.0);
	}
}

void MaxNormalWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_normal->SetNumNormals(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_normal->Normal(i) = Point3(value[0], value[1], value[2]);
		this->_normal->SetNormalExplicit(i, true);
	}
}

void MaxNormalWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_normal->GetNumFaces();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = TRIMESH_FACE_VERTEX;
	}
}

void MaxNormalWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	// do nothing, mesh can only be composed by triangles (3 degree)
}

void MaxNormalWrapper::GetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_normal->GetNumFaces();
	int totalCount = faceCount * TRIMESH_FACE_VERTEX;
	if (indices.size() != totalCount) {
		indices.resize(totalCount);
	}
	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			indices[i * TRIMESH_FACE_VERTEX + j] = this->_normal->GetNormalIndex(i, j);
		}
	}
}

void MaxNormalWrapper::SetFaceIndexList(std::vector<int> &indices) {
	size_t faceCount = indices.size() / TRIMESH_FACE_VERTEX;
	this->_normal->SetNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			this->_normal->SetNormalIndex(i, j, indices[i * TRIMESH_FACE_VERTEX + j]);
		}
	}
}

#endif