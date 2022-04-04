#ifndef _MAX_MESH_WRAPPER_H_
#define _MAX_MESH_WRAPPER_H_

#include <max.h>
#include "../MeshWrapper.h"

class MaxMeshWrapper : public MeshWrapper {

private:
	Mesh& _mesh;

public:
	MaxMeshWrapper(Mesh& mesh);
	~MaxMeshWrapper();

	PropertyWrapper* GetProperty(MeshProperty property, int index = 0, bool createIfNotExist = false) override;
	void DeleteProperty(MeshProperty property, int index = 0) override;

	int GatFaceCount() override;
	int GatFaceDegree(int faceIndex) override;
	int GatFaceIndex(int faceIndex, int degreeIndex) override;
};

#endif