#ifndef _MAX_MNMESH_WRAPPER_H_
#define _MAX_MNMESH_WRAPPER_H_

#include <max.h>
#include "../MeshWrapper.h"

class MaxMNMeshWrapper : public MeshWrapper {

private:
	MNMesh& _mesh;

public:
	MaxMNMeshWrapper(MNMesh& mesh);
	~MaxMNMeshWrapper();

	PropertyWrapper* GetProperty(MeshProperty property, int index = 0, bool createIfNotExist = false) override;
	void DeleteProperty(MeshProperty property, int index = 0) override;

	int GatFaceCount() override;
	int GatFaceDegree(int faceIndex) override;
	int GatFaceIndex(int faceIndex, int degreeIndex) override;
};

#endif