#ifndef _FBX_MESH_REF_WRAPPER_H_
#define _FBX_MESH_REF_WRAPPER_H_

#include <fbxsdk.h>
#include "../../Utility.h"
#include "../MeshWrapper.h"

class FbxMeshRefWrapper : public MeshWrapper {

private:
	FbxMeshRef* _mesh;
	std::vector<int> _degrees;

public:
	FbxMeshRefWrapper(FbxMeshRef* mesh);
	~FbxMeshRefWrapper();

	PropertyWrapper* GetProperty(MeshProperty property, int index = 0, bool createIfNotExist = false) override;
	void DeleteProperty(MeshProperty property, int index = 0) override;

	int GatFaceCount() override;
	int GatFaceDegree(int faceIndex) override;
	int GatFaceIndex(int faceIndex, int degreeIndex) override;
};

#endif