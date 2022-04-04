#ifndef _PROPERTY_WRAPPER_H_
#define _PROPERTY_WRAPPER_H_

#include <fbxsdk.h>
#include <max.h>
#include <vector>

#define TRIMESH_FACE_VERTEX 3

class PropertyWrapper {

public:
	virtual void GetVertexList(std::vector<FbxVector4> &verts) = 0;
	virtual void SetVertexList(std::vector<FbxVector4> &verts) = 0;

	virtual void GetFaceDegreeList(std::vector<int> &degrees) = 0;
	virtual void SetFaceDegreeList(std::vector<int> &degrees) = 0;

	virtual void GetFaceIndexList(std::vector<int> &indices) = 0;
	virtual void SetFaceIndexList(std::vector<int> &indices) = 0;

	static bool CopyProperty(PropertyWrapper* src, PropertyWrapper* dst, std::vector<int> &map) {
		if (src == NULL || dst == NULL) {
			return false;
		}

		// Set Vertex
		std::vector<FbxVector4> verts;
		src->GetVertexList(verts);
		dst->SetVertexList(verts);

		// Set Face
		std::vector<int> degrees;
		src->GetFaceDegreeList(degrees);
		dst->SetFaceDegreeList(degrees);
		std::vector<int> indices;
		src->GetFaceIndexList(indices);

		// Remapping index
		if (map.size() == indices.size()) {
			std::vector<int> remapped(indices.size());
			for (int i = 0; i < indices.size(); i++) {
				remapped[map[i]] = indices[i];
			}
			dst->SetFaceIndexList(remapped);
		}
		else {
			dst->SetFaceIndexList(indices);
		}

		return true;
	}
};

#endif