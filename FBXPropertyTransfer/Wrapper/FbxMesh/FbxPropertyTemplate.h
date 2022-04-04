#ifndef _FBX_PROPERTY_TEMPLATE_H_
#define _FBX_PROPERTY_TEMPLATE_H_

#include <fbxsdk.h>
#include "../PropertyWrapper.h"

template<typename T>
class FbxPropertyTemplate : public PropertyWrapper {

private:
	std::vector<int>& _degrees;
	FbxLayerElementTemplate<T>* _property;
	virtual FbxVector4 ValueToVec4(T value) = 0;
	virtual T Vec4ToValue(FbxVector4 value) = 0;

public:
	FbxPropertyTemplate(FbxLayerElementTemplate<T>* property, std::vector<int>& degrees);
	~FbxPropertyTemplate();

	void GetVertexList(std::vector<FbxVector4> &verts) override;
	void SetVertexList(std::vector<FbxVector4> &verts) override;

	void GetFaceDegreeList(std::vector<int> &degrees) override;
	void SetFaceDegreeList(std::vector<int> &degrees) override;

	void GetFaceIndexList(std::vector<int> &indices) override;
	void SetFaceIndexList(std::vector<int> &indices) override;
};

template <typename T>
FbxPropertyTemplate<T>::FbxPropertyTemplate(FbxLayerElementTemplate<T>* property, std::vector<int>& degrees) : _property(property), _degrees(degrees) {

}
	
template <typename T>
FbxPropertyTemplate<T>::~FbxPropertyTemplate(){

}

template <typename T>
void FbxPropertyTemplate<T>::GetVertexList(std::vector<FbxVector4> &verts) {
	FbxLayerElementArrayTemplate<T> &src = this->_property->GetDirectArray();
	int count = src.GetCount();
	if (verts.size() != count) {
		verts.resize(count);
	}
	for (int i = 0; i < count; i++) {
		verts[i] = ValueToVec4(src.GetAt(i));
	}
}

template <typename T>
void FbxPropertyTemplate<T>::SetVertexList(std::vector<FbxVector4> &verts) {
	FbxLayerElementArrayTemplate<T> &dst = this->_property->GetDirectArray();
	size_t count = verts.size();
	if (dst.GetCount() != count) {
		dst.Resize(count);
	}
	for (size_t i = 0; i < count; i++) {
		dst.SetAt(i, Vec4ToValue(verts[i]));
	}
}

template <typename T>
void FbxPropertyTemplate<T>::GetFaceDegreeList(std::vector<int> &degrees) {
	degrees = this->_degrees;
}

template <typename T>
void FbxPropertyTemplate<T>::SetFaceDegreeList(std::vector<int> &degrees) {
	// can't change directly, should be the same as mesh polygon size
}

template <typename T>
void FbxPropertyTemplate<T>::GetFaceIndexList(std::vector<int> &indices) {
	if (this->_property->GetReferenceMode() == FbxLayerElement::eDirect) {
		indices.clear();
		int count = 0;
		for (int i = 0; i < this->_degrees.size(); i++) {
			for (int j = 0; j < this->_degrees[i]; j++) {
				indices.push_back(count++);
			}
		}
	}
	else if (this->_property->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		FbxLayerElementArrayTemplate<int> &src = this->_property->GetIndexArray();
		int count = src.GetCount();
		if (indices.size() != count) {
			indices.resize(count);
		}
		for (int i = 0; i < count; i++) {
			indices[i] = src.GetAt(i);
		}
	}
}

template <typename T>
void FbxPropertyTemplate<T>::SetFaceIndexList(std::vector<int> &indices) {
	bool direct = true;
	for (int i = 0; i < indices.size(); i++) {
		if (indices[i] != i) {
			direct = false;
			break;
		}
	}
	if (direct) {
		this->_property->SetReferenceMode(FbxLayerElement::eDirect);
		return;
	}
	if (this->_property->GetReferenceMode() != FbxLayerElement::eIndexToDirect) {
		this->_property->SetReferenceMode(FbxLayerElement::eIndexToDirect);
	}
	FbxLayerElementArrayTemplate<int> &dst = this->_property->GetIndexArray();
	size_t count = indices.size();
	if (dst.GetCount() != count) {
		dst.Resize(count);
	}
	for (size_t i = 0; i < count; i++) {
		dst.SetAt(i, indices[i]);
	}
}

#endif