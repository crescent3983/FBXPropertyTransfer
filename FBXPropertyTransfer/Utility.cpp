#include "Utility.h"

std::pair<MeshWrapper::MeshProperty, int> NameToProperty(char* name) {

	MeshWrapper::MeshProperty property = MeshWrapper::None;
	int index = 0;

	if (strcmp(name, "vc") == 0) {
		property = MeshWrapper::VertexColor;
	}
	else if (strcmp(name, "n") == 0) {
		property = MeshWrapper::Normal;
	}
	else if (strcmp(name, "t") == 0) {
		property = MeshWrapper::Tangent;
	}
	else if (strcmp(name, "uv0") == 0) {
		property = MeshWrapper::UV;
		index = 0;
	}
	else if (strcmp(name, "uv1") == 0) {
		property = MeshWrapper::UV;
		index = 1;
	}
	else if (strcmp(name, "uv2") == 0) {
		property = MeshWrapper::UV;
		index = 2;
	}
	else if (strcmp(name, "uv3") == 0) {
		property = MeshWrapper::UV;
		index = 3;
	}
	else if (strcmp(name, "uv4") == 0) {
		property = MeshWrapper::UV;
		index = 4;
	}
	else if (strcmp(name, "uv5") == 0) {
		property = MeshWrapper::UV;
		index = 5;
	}
	else if (strcmp(name, "uv6") == 0) {
		property = MeshWrapper::UV;
		index = 6;
	}
	else if (strcmp(name, "uv7") == 0) {
		property = MeshWrapper::UV;
		index = 7;
	}

	return std::pair<MeshWrapper::MeshProperty, int> (property, index);
}

MSTR PropertyToName(MeshWrapper::MeshProperty property) {
	switch (property) {
		case MeshWrapper::VertexColor: return _M("Vertex Color");
		case MeshWrapper::Normal: return _M("Normal");
		case MeshWrapper::Tangent: return _M("Tangent");
		case MeshWrapper::Binormal: return _M("Binormal");
		case MeshWrapper::UV: return _M("UV");
	}
	return _M("");
}

bool ParseArgument(const char* input, char* name, size_t nameSize, char* srcProperty, size_t srcPropertySize, char* dstProperty, size_t dstPropertySize){
	int idx[3];
	int count = 0;
	int inputLen = strlen(input);
	for(int i = inputLen - 1; i >= 0; i--){
		if(input[i] == '_'){
			idx[count] = i;
			if(++count == 3){
				break;
			}
		}
	}
	// match count
	if(count != 3) return false;
	// src property limitation
	if(idx[1] - idx[2] <= 0) return false;
	// to
	if(idx[0] - idx[1] != 3 || input[idx[1]+1] != 't' || input[idx[1]+2] != 'o') return false;
	// dst property limitation
	if(idx[0] >= inputLen-1) return false;

	int len;
	len = idx[2];
	strncpy(name, input, max(len, nameSize-1));
	name[len] = '\0';

	len = idx[1] - idx[2] - 1;
	strncpy(srcProperty, input + idx[2] + 1, max(len, srcPropertySize-1));
	srcProperty[len] = '\0';

	len = inputLen - idx[0] - 1;
	strncpy(dstProperty, input + idx[0] + 1 , max(len, dstPropertySize-1));
	dstProperty[len] = '\0';

	return true;
}

//=================================
// 3ds Max Releated
//=================================

TriObject* GetTriObjectFromNode(INode *Node, TimeValue t, bool &deleteIt) {
	deleteIt = false;
	Object *pObj = Node->EvalWorldState(t).obj;
	if (pObj && pObj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {
		TriObject *pTri = (TriObject *)pObj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID,0));
		if (pObj != pTri) deleteIt = true;
		return pTri;
	} else {
		return NULL;
	}
}

PolyObject* GetPolyObjectFromNode(INode *pNode, TimeValue t, bool &deleteIt) {
	deleteIt = false;
	Object *pObj = pNode->EvalWorldState(t).obj;
	if (pObj && pObj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0))) {
		PolyObject *pTri = (PolyObject *)pObj->ConvertToType(t, Class_ID(POLYOBJ_CLASS_ID,0));
		if (pObj != pTri) deleteIt = true;
		return pTri;
	} else {
		return NULL;
	}
}

//=================================
// FBX Releated
//=================================

bool CheckIsMesh(FbxNode* fbxNode) {
	return fbxNode && fbxNode->GetNodeAttribute() && fbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh;
}
