#ifndef _FBX_TANGENT_WRAPPER_H_
#define _FBX_TANGENT_WRAPPER_H_

#include <fbxsdk.h>
#include "FbxPropertyTemplate.h"

class FbxTangentWrapper : public FbxPropertyTemplate<FbxVector4> {

private:
	FbxVector4 ValueToVec4(FbxVector4 value) override {
		return value;
	}

	FbxVector4 Vec4ToValue(FbxVector4 value) override {
		return value;
	}

public:
	FbxTangentWrapper(FbxLayerElementTangent* tangent, std::vector<int>& degrees) : FbxPropertyTemplate<FbxVector4>(tangent, degrees) {

	}
};

#endif