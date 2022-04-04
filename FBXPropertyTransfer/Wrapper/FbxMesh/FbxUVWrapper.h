#ifndef _FBX_UV_WRAPPER_H_
#define _FBX_UV_WRAPPER_H_

#include <fbxsdk.h>
#include "FbxPropertyTemplate.h"

class FbxUVWrapper : public FbxPropertyTemplate<FbxVector2> {

private:
	FbxVector4 ValueToVec4(FbxVector2 value) override {
		return FbxVector4(value[0], value[1], 0, 0);
	}

	FbxVector2 Vec4ToValue(FbxVector4 value) override {
		return FbxVector2(value[0], value[1]);
	}

public:
	FbxUVWrapper(FbxLayerElementUV* uv, std::vector<int>& degrees) : FbxPropertyTemplate<FbxVector2>(uv, degrees) {

	}
};

#endif