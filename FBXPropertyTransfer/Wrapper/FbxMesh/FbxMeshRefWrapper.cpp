#include "FbxMeshRefWrapper.h"
#include "FbxVertexColorWrapper.h"
#include "FbxNormalWrapper.h"
#include "FbxBinormalWrapper.h"
#include "FbxTangentWrapper.h"
#include "FbxUVWrapper.h"

FbxMeshRefWrapper::FbxMeshRefWrapper(FbxMeshRef* mesh) : _mesh(mesh) {
	int count = this->_mesh->_rawMesh->GetPolygonCount();
	this->_degrees.resize(count);
	for (int i = 0; i < count; i++) {
		this->_degrees[i] = this->_mesh->_rawMesh->GetPolygonSize(i);
	}
}

FbxMeshRefWrapper::~FbxMeshRefWrapper(){

}

PropertyWrapper* FbxMeshRefWrapper::GetProperty(MeshProperty property, int index, bool createIfNotExist) {
	switch(property){
		case MeshWrapper::VertexColor:
			{
				if(this->_mesh->vertexColorCount > 0){
					return new FbxVertexColorWrapper(this->_mesh->vertexColors[0], this->_degrees);
				}
				break;
			}
		case MeshWrapper::Normal:
			{
				if(this->_mesh->normalCount > 0){
					return new FbxNormalWrapper(this->_mesh->normals[0], this->_degrees);
				}
				break;
			}
		case MeshWrapper::Tangent:
			{
				if(this->_mesh->tangentCount > 0){
					return new FbxTangentWrapper(this->_mesh->tangents[0], this->_degrees);
				}
				break;
			}
		case MeshWrapper::Binormal:
			{
				if(this->_mesh->binormalCount > 0){
					return new FbxBinormalWrapper(this->_mesh->binormals[0], this->_degrees);
				}
				break;
			}
		case MeshWrapper::UV:
			{
				if(index >= 0 && this->_mesh->uvCount > index){
					return new FbxUVWrapper(this->_mesh->uvs[index], this->_degrees);
				}
				break;
			}
	}
	return NULL;
}

void FbxMeshRefWrapper::DeleteProperty(MeshProperty property, int index) {
	// do nothing...
}

int FbxMeshRefWrapper::GatFaceCount() {
	return this->_mesh->_rawMesh->GetPolygonCount();
}

int FbxMeshRefWrapper::GatFaceDegree(int faceIndex) {
	return this->_mesh->_rawMesh->GetPolygonSize(faceIndex);
}

int FbxMeshRefWrapper::GatFaceIndex(int faceIndex, int degreeIndex) {
	return this->_mesh->_rawMesh->GetPolygonVertex(faceIndex, degreeIndex);
}