#ifndef _MAX_MN_NORMAL_WRAPPER_H_
#define _MAX_MN_NORMAL_WRAPPER_H_

#include <max.h>
#include <MeshNormalSpec.h>
#include "../PropertyWrapper.h"

class MaxMNNormalWrapper : public PropertyWrapper {

private:
	MNNormalSpec* _normal;

public:
	MaxMNNormalWrapper(MNNormalSpec* normal);
	~MaxMNNormalWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxMNNormalWrapper::MaxMNNormalWrapper(MNNormalSpec* normal) : _normal(normal) {

}

MaxMNNormalWrapper::~MaxMNNormalWrapper() {

}

void MaxMNNormalWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	int count = this->_normal->GetNumNormals();
	if (verts.size() != count) {
		verts.resize(count);
	}
	for (int i = 0; i < count; i++) {
		Point3 value = this->_normal->Normal(i);
		verts[i] = FbxVector4(value.x, value.y, value.z, 0.0);
	}
}

void MaxMNNormalWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_normal->SetNumNormals(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_normal->Normal(i) = Point3(value[0], value[1], value[2]);
		this->_normal->SetNormalExplicit(i, true);
	}
}

void MaxMNNormalWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_normal->GetNumFaces();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = this->_normal->Face(i).GetDegree();
	}
}

void MaxMNNormalWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	size_t faceCount = degrees.size();
	this->_normal->SetNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		this->_normal->Face(i).SetDegree(degrees[i]);
	}
}

void MaxMNNormalWrapper::GetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_normal->GetNumFaces();
	indices.clear();
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_normal->Face(i).GetDegree();
		for (int j = 0; j < degree; j++) {
			indices.push_back(this->_normal->GetNormalIndex(i, j));
		}
	}
}

void MaxMNNormalWrapper::SetFaceIndexList(std::vector<int> &indices) {
	int faceCount = this->_normal->GetNumFaces();
	int count = 0;
	for (int i = 0; i < faceCount; i++) {
		int degree = this->_normal->Face(i).GetDegree();
		for (int j = 0; j < degree; j++) {
			if (count >= indices.size()) {
				// count not match
				return;
			}
			this->_normal->SetNormalIndex(i, j, indices[count++]);
		}
	}
}

#endif