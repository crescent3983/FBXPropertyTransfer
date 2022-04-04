#ifndef _FBX_VERTEX_COLOR_WRAPPER_H_
#define _FBX_VERTEX_COLOR_WRAPPER_H_

#include <fbxsdk.h>
#include "FbxPropertyTemplate.h"

class FbxVertexColorWrapper : public FbxPropertyTemplate<FbxColor> {

private:
	FbxVector4 ValueToVec4(FbxColor value) override {
		return FbxVector4(value[0], value[1], value[2], value[3]);
	}

	FbxColor Vec4ToValue(FbxVector4 value) override {
		return FbxColor(value);
	}

public:
	FbxVertexColorWrapper(FbxLayerElementVertexColor* vertexColor, std::vector<int>& degrees) : FbxPropertyTemplate<FbxColor>(vertexColor, degrees) {

	}
};

#endif