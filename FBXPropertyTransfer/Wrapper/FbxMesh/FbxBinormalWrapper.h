#ifndef _FBX_BINORMAL_WRAPPER_H_
#define _FBX_BINORMAL_WRAPPER_H_

#include <fbxsdk.h>
#include "FbxPropertyTemplate.h"

class FbxBinormalWrapper : public FbxPropertyTemplate<FbxVector4> {

private:
	FbxVector4 ValueToVec4(FbxVector4 value) override {
		return value;
	}

	FbxVector4 Vec4ToValue(FbxVector4 value) override {
		return value;
	}

public:
	FbxBinormalWrapper(FbxLayerElementBinormal* binormal, std::vector<int>& degrees) : FbxPropertyTemplate<FbxVector4>(binormal, degrees) {

	}
};

#endif