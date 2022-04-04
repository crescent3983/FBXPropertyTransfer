#ifndef _FBX_MESH_WRAPPER_H_
#define _FBX_MESH_WRAPPER_H_

#include <fbxsdk.h>
#include "../MeshWrapper.h"

class FbxMeshWrapper : public MeshWrapper {

private:
	FbxMesh* _mesh;
	std::vector<int> _degrees;

public:
	FbxMeshWrapper(FbxMesh* mesh);
	~FbxMeshWrapper();

	PropertyWrapper* GetProperty(MeshProperty property, int index = 0, bool createIfNotExist = false) override;
	void DeleteProperty(MeshProperty property, int index = 0) override;

	int GatFaceCount() override;
	int GatFaceDegree(int faceIndex) override;
	int GatFaceIndex(int faceIndex, int degreeIndex) override;
};

#endif