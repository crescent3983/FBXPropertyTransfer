#ifndef _MAX_VERTEX_COLOR_WRAPPER_H_
#define _MAX_VERTEX_COLOR_WRAPPER_H_

#include <fbxsdk.h>
#include <max.h>
#include "../PropertyWrapper.h"

class MaxVertexColorWrapper : public PropertyWrapper {

private:
	MeshMap& _colorMap;
	MeshMap& _alphaMap;

public:
	MaxVertexColorWrapper(MeshMap& colorMap, MeshMap& alphaMap);
	~MaxVertexColorWrapper();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

MaxVertexColorWrapper::MaxVertexColorWrapper(MeshMap& colorMap, MeshMap& alphaMap) : _colorMap(colorMap), _alphaMap(alphaMap) {

}

MaxVertexColorWrapper::~MaxVertexColorWrapper(){

}

void MaxVertexColorWrapper::GetVertexList(std::vector<FbxVector4> &verts) {
	verts.clear();
	int faceCount = this->_colorMap.getNumFaces();
	int count = 0;
	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			int colorID = this->_colorMap.tf[i].t[j];
			int alphaID = this->_alphaMap.tf[i].t[j];
			Point3 color = this->_colorMap.tv[colorID];
			Point3 alpha = this->_alphaMap.tv[alphaID];
			verts.push_back(FbxVector4(color[0], color[1], color[2], alpha[0]));
		}
	}
}

void MaxVertexColorWrapper::SetVertexList(std::vector<FbxVector4> &verts) {
	size_t count = verts.size();
	this->_colorMap.setNumVerts(count);
	this->_alphaMap.setNumVerts(count);
	for (size_t i = 0; i < count; i++) {
		FbxVector4 value = verts[i];
		this->_colorMap.tv[i] = Point3(value[0], value[1], value[2]);
		this->_alphaMap.tv[i] = Point3(value[3], 0.0, 0.0);
	}
}

void MaxVertexColorWrapper::GetFaceDegreeList(std::vector<int> &degrees) {
	int count = this->_colorMap.getNumFaces();
	if (degrees.size() != count) {
		degrees.resize(count);
	}
	for (int i = 0; i < count; i++) {
		degrees[i] = TRIMESH_FACE_VERTEX;
	}
}

void MaxVertexColorWrapper::SetFaceDegreeList(std::vector<int> &degrees) {
	// do nothing, mesh can only be composed by triangles (3 degree)
}

void MaxVertexColorWrapper::GetFaceIndexList(std::vector<int> &indices) {
	indices.clear();
	int faceCount = this->_colorMap.getNumFaces();
	int index = 0;
	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			indices.push_back(index++);
		}
	}
}

void MaxVertexColorWrapper::SetFaceIndexList(std::vector<int> &indices) {
	size_t faceCount = indices.size() / TRIMESH_FACE_VERTEX;
	this->_colorMap.setNumFaces(faceCount);
	this->_alphaMap.setNumFaces(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		for (int j = 0; j < TRIMESH_FACE_VERTEX; j++) {
			int index = indices[i * TRIMESH_FACE_VERTEX + j];
			this->_colorMap.tf[i].t[j] = index;
			this->_alphaMap.tf[i].t[j] = index;
		}
	}
}

#endif