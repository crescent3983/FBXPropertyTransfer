#ifndef _MESH_WRAPPER_H_
#define _MESH_WRAPPER_H_

#include "PropertyWrapper.h"
#include <memory>

class MeshWrapper {

public:
	enum MeshProperty {
		VertexColor = 0,
		Normal,
		Tangent,
		Binormal,
		UV,

		None = 255
	};

	virtual PropertyWrapper* GetProperty(MeshProperty property, int index = 0, bool createIfNotExist = false) = 0;
	virtual void DeleteProperty(MeshProperty property, int index = 0) = 0;

	virtual int GatFaceCount() = 0;
	virtual int GatFaceDegree(int faceIndex) = 0;
	virtual int GatFaceIndex(int faceIndex, int degreeIndex) = 0;

	static bool CheckFaceIsMatch(MeshWrapper& meshA, MeshWrapper& meshB) {
		if (meshA.GatFaceCount() != meshB.GatFaceCount()) {
			return false;
		}
		int count = meshA.GatFaceCount();
		for (int i = 0; i < count; i++) {
			if (meshA.GatFaceDegree(i) != meshB.GatFaceDegree(i)) {
				return false;
			}
		}
		return true;
	}

	static bool BuildFaceMappingTable(MeshWrapper& src, MeshWrapper& dst, std::vector<int>& map) {
		if (src.GatFaceCount() != dst.GatFaceCount()) {
			return false;
		}
		// precalculate size
		int faceCount = src.GatFaceCount();
		int total = 0;
		for (int i = 0; i < faceCount; i++) {
			total += src.GatFaceDegree(i);
		}
		map.resize(total);

		int faceIndexCount = 0;
		for (int i = 0; i < faceCount; i++) {
			int degree = src.GatFaceDegree(i);
			for (int j = 0; j < degree; j++) {
				int id = src.GatFaceIndex(i, j);
				int k;
				for (k = 0; k < degree; k++) {
					if (dst.GatFaceIndex(i, k) == id) {
						map[faceIndexCount + j] = faceIndexCount + k;
						break;
					}
				}
				// can't find a match
				if (k == degree) {
					return false;
				}
			}
			faceIndexCount += degree;
		}
		return true;
	}
};

#endif