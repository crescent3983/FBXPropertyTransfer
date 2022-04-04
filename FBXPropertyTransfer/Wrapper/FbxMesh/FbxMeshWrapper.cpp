#include "FbxMeshWrapper.h"
#include "FbxVertexColorWrapper.h"
#include "FbxNormalWrapper.h"
#include "FbxBinormalWrapper.h"
#include "FbxTangentWrapper.h"
#include "FbxUVWrapper.h"

FbxMeshWrapper::FbxMeshWrapper(FbxMesh* mesh) : _mesh(mesh) {
	int count = this->_mesh->GetPolygonCount();
	this->_degrees.resize(count);
	for (int i = 0; i < count; i++) {
		this->_degrees[i] = this->_mesh->GetPolygonSize(i);
	}
}

FbxMeshWrapper::~FbxMeshWrapper(){

}

PropertyWrapper* FbxMeshWrapper::GetProperty(MeshProperty property, int index, bool createIfNotExist) {
	switch(property){
		case MeshWrapper::VertexColor:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				FbxLayerElementVertexColor *vertexColor = layer->GetVertexColors();
				if(vertexColor == NULL && createIfNotExist){
					vertexColor = (FbxLayerElementVertexColor*)layer->CreateLayerElementOfType(FbxLayerElement::eVertexColor);
					if(vertexColor) vertexColor->SetMappingMode(FbxLayerElement::eByPolygonVertex);
				}
				if(vertexColor){
					return new FbxVertexColorWrapper(vertexColor, this->_degrees);
				}
				break;
			}
		case MeshWrapper::Normal:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				FbxLayerElementNormal *normal = layer->GetNormals();
				if(normal == NULL && createIfNotExist){
					normal = (FbxLayerElementNormal*)layer->CreateLayerElementOfType(FbxLayerElement::eNormal);
					if(normal) normal->SetMappingMode(FbxLayerElement::eByPolygonVertex);
				}
				if(normal){
					return new FbxNormalWrapper(normal, this->_degrees);
				}
				break;
			}
		case MeshWrapper::Tangent:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				FbxLayerElementTangent *tangnet = layer->GetTangents();
				if(tangnet == NULL && createIfNotExist){
					tangnet = (FbxLayerElementTangent*)layer->CreateLayerElementOfType(FbxLayerElement::eTangent);
					if(tangnet) tangnet->SetMappingMode(FbxLayerElement::eByPolygonVertex);
				}
				if(tangnet){
					return new FbxTangentWrapper(tangnet, this->_degrees);
				}
				break;
			}
		case MeshWrapper::Binormal:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				FbxLayerElementBinormal *binormal = layer->GetBinormals();
				if(binormal == NULL && createIfNotExist){
					binormal = (FbxLayerElementBinormal*)layer->CreateLayerElementOfType(FbxLayerElement::eBiNormal);
					if(binormal) binormal->SetMappingMode(FbxLayerElement::eByPolygonVertex);
				}
				if(binormal){
					return new FbxBinormalWrapper(binormal, this->_degrees);
				}
				break;
			}
		case MeshWrapper::UV:
			{
				FbxLayerElementUV *uv = this->_mesh->GetElementUV(index);
				if(uv == NULL && createIfNotExist){
					char name[10];
					while(this->_mesh->GetElementUVCount() <= index){
						sprintf_s(name, "UVSet%d", this->_mesh->GetElementUVCount());
						uv = this->_mesh->CreateElementUV(name);
						if(uv) uv->SetMappingMode(FbxLayerElement::eByPolygonVertex);
					}
				}
				if(uv){
					return new FbxUVWrapper(uv, this->_degrees);
				}
				break;
			}
	}
	return NULL;
}

void FbxMeshWrapper::DeleteProperty(MeshProperty property, int index) {
	switch(property){
		case FbxMeshWrapper::VertexColor:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				layer->SetVertexColors(NULL);
				break;
			}
		case FbxMeshWrapper::Normal:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				layer->SetNormals(NULL);
				break;
			}
		case FbxMeshWrapper::Tangent:
			{
				FbxLayer *layer = this->_mesh->GetLayer(0);
				layer->SetTangents(NULL);
				break;
			}
		case FbxMeshWrapper::UV:
			{
				FbxLayerElementUV *uv = this->_mesh->GetElementUV(index);
				if(uv){
					this->_mesh->RemoveElementUV(uv);
				}
				break;
			}
	}
}

int FbxMeshWrapper::GatFaceCount() {
	return this->_mesh->GetPolygonCount();
}

int FbxMeshWrapper::GatFaceDegree(int faceIndex) {
	return this->_mesh->GetPolygonSize(faceIndex);
}

int FbxMeshWrapper::GatFaceIndex(int faceIndex, int degreeIndex) {
	return this->_mesh->GetPolygonVertex(faceIndex, degreeIndex);
}