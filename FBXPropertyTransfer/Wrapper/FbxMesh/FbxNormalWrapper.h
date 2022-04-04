#ifndef _FBX_NORMAL_WRAPPER_H_
#define _FBX_NORMAL_WRAPPER_H_

#include <fbxsdk.h>
#include "FbxPropertyTemplate.h"

class FbxNormalWrapper : public FbxPropertyTemplate<FbxVector4> {

private:
	FbxVector4 ValueToVec4(FbxVector4 value) override {
		return value;
	}

	FbxVector4 Vec4ToValue(FbxVector4 value) override {
		return value;
	}

public:
	FbxNormalWrapper(FbxLayerElementNormal* normal, std::vector<int>& degrees) : FbxPropertyTemplate<FbxVector4>(normal, degrees) {

	}
};

#endif