#ifndef _MESH_UTILITY_H_
#define _MESH_UTILITY_H_

#include "Wrapper/MeshWrapper.h"
#include <max.h>
#include <fbxsdk.h>

#define MAX_ELEMENT_COUNT 10

template <typename T>
bool CopyElementArray(FbxLayerElementArrayTemplate<T>& source, FbxLayerElementArrayTemplate<T>& target) {
	if (source.IsEqual(target)) {
		return true;
	}
	target.Clear();
	target.Resize(source.GetCount());
	for (int i = 0; i < source.GetCount(); i++) {
		target.SetAt(i, source.GetAt(i));
	}
	return true;
}

template <typename T>
bool CopyElement(FbxLayerElementTemplate<T>* source, FbxLayerElementTemplate<T>* target) {
	if (source && target) {
		if (source->GetReferenceMode() != target->GetReferenceMode()) {
			target->SetReferenceMode(source->GetReferenceMode());
		}
		if (source->GetMappingMode() != target->GetMappingMode()) {
			target->SetMappingMode(source->GetMappingMode());
		}
		bool result = true;
		if (target->GetReferenceMode() != FbxLayerElement::eDirect) {
			result = CopyElementArray<int>(source->GetIndexArray(), target->GetIndexArray());
		}
		return result && CopyElementArray<T>(source->GetDirectArray(), target->GetDirectArray());
	}
	return false;
}

typedef struct fbxMeshRef{
	FbxMesh* _rawMesh;
	int tangentCount;
	FbxLayerElementTangent* tangents[MAX_ELEMENT_COUNT];
	int normalCount;
	FbxLayerElementNormal* normals[MAX_ELEMENT_COUNT];
	int binormalCount;
	FbxLayerElementBinormal* binormals[MAX_ELEMENT_COUNT];
	int vertexColorCount;
	FbxLayerElementVertexColor* vertexColors[MAX_ELEMENT_COUNT];
	int uvCount;
	FbxLayerElementUV* uvs[MAX_ELEMENT_COUNT];

	fbxMeshRef(FbxMesh* mesh) {
		if(mesh == NULL){
			tangentCount = normalCount = binormalCount = vertexColorCount = uvCount = 0;
			return;
		}
		this->_rawMesh = mesh;
		tangentCount = mesh->GetElementTangentCount();
		normalCount = mesh->GetElementNormalCount();
		binormalCount = mesh->GetElementBinormalCount();
		vertexColorCount = mesh->GetElementVertexColorCount();
		uvCount = mesh->GetElementUVCount();

		for(int i = 0; i < tangentCount; i++){
			tangents[i] = FbxLayerElementTangent::Create(NULL, "");
			CopyElement<FbxVector4>(mesh->GetElementTangent(i), tangents[i]);
		}
		for(int i = 0; i < normalCount; i++){
			normals[i] = FbxLayerElementNormal::Create(NULL, "");
			CopyElement<FbxVector4>(mesh->GetElementNormal(i), normals[i]);
		}
		for(int i = 0; i < binormalCount; i++){
			binormals[i] = FbxLayerElementBinormal::Create(NULL, "");
			CopyElement<FbxVector4>(mesh->GetElementBinormal(i), binormals[i]);
		}
		for(int i = 0; i < vertexColorCount; i++){
			vertexColors[i] = FbxLayerElementVertexColor::Create(NULL, "");
			CopyElement<FbxColor>(mesh->GetElementVertexColor(i), vertexColors[i]);
		}
		for(int i = 0; i < uvCount; i++){
			uvs[i] = FbxLayerElementUV::Create(NULL, "");
			CopyElement<FbxVector2>(mesh->GetElementUV(i), uvs[i]);
		}
	}

	~fbxMeshRef() {
		for (int i = 0; i < tangentCount; i++) {
			if (tangents[i] != NULL) {
				tangents[i]->Destroy();
				tangents[i] = NULL;
			}
		}
		for (int i = 0; i < normalCount; i++) {
			if (normals[i] != NULL) {
				normals[i]->Destroy();
				normals[i] = NULL;
			}
		}
		for (int i = 0; i < binormalCount; i++) {
			if (binormals[i] != NULL) {
				binormals[i]->Destroy();
				binormals[i] = NULL;
			}
		}
		for (int i = 0; i < vertexColorCount; i++) {
			if (vertexColors[i] != NULL) {
				vertexColors[i]->Destroy();
				vertexColors[i] = NULL;
			}
		}
		for (int i = 0; i < uvCount; i++) {
			if (uvs[i] != NULL) {
				uvs[i]->Destroy();
				uvs[i] = NULL;
			}
		}
	}

} FbxMeshRef;

bool ParseArgument(const char* input, char* name, size_t nameSize, char* srcProperty, size_t srcPropertySize, char* dstProperty, size_t dstPropertySize);
std::pair<MeshWrapper::MeshProperty, int> NameToProperty(char* name);
MSTR PropertyToName(MeshWrapper::MeshProperty property);
TriObject* GetTriObjectFromNode(INode *Node, TimeValue t, bool &deleteIt);
PolyObject* GetPolyObjectFromNode(INode *pNode, TimeValue t, bool &deleteIt);
bool CheckIsMesh(FbxNode* fbxNode);

#endif