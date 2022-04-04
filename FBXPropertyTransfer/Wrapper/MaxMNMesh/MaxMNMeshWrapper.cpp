#include "MaxMNMeshWrapper.h"
#include "MaxMNVertexColorWrapper.h"
#include "MaxMNNormalWrapper.h"
#include "MaxMNUVWrapper.h"

#define VERTEX_COLOR_MAPCHANNEL 0
#define ALPHA_MAPCHANNEL -2

MaxMNMeshWrapper::MaxMNMeshWrapper(MNMesh& mesh) : _mesh(mesh) {

}

MaxMNMeshWrapper::~MaxMNMeshWrapper() {

}

PropertyWrapper* MaxMNMeshWrapper::GetProperty(MeshProperty property, int index, bool createIfNotExist) {
	switch (property) {
	case MeshWrapper::VertexColor:
	{
		int idx = VERTEX_COLOR_MAPCHANNEL;
		if (this->_mesh.MNum() < idx + 1 && createIfNotExist) {
			this->_mesh.SetMapNum(idx + 1);
		}
		if (this->_mesh.MNum() > idx) {
			if (this->_mesh.M(idx)->GetFlag(MN_DEAD)) {
				this->_mesh.InitMap(idx);
			}
			if (this->_mesh.M(ALPHA_MAPCHANNEL)->GetFlag(MN_DEAD)) {
				this->_mesh.InitMap(ALPHA_MAPCHANNEL);
			}
			return new MaxMNVertexColorWrapper(this->_mesh.M(idx), this->_mesh.M(ALPHA_MAPCHANNEL));
		}
		break;
	}
	case MeshWrapper::Normal:
	{
		MNNormalSpec *normal = this->_mesh.GetSpecifiedNormals();
		if ((normal == NULL) || (normal != NULL && !normal->GetFlag(MNNORMAL_NORMALS_BUILT))) {
			this->_mesh.SpecifyNormals();
			normal = this->_mesh.GetSpecifiedNormals();
		}
		if (normal) {
			normal->CheckNormals();
			return new MaxMNNormalWrapper(normal);
		}
		break;
	}
	case MeshWrapper::UV:
	{
		int idx = index + 1;
		if (this->_mesh.MNum() < idx + 1 && createIfNotExist) {
			this->_mesh.SetMapNum(idx + 1);
		}
		if (idx >= 0 && this->_mesh.MNum() > idx) {
			if (this->_mesh.M(idx)->GetFlag(MN_DEAD)) {
				this->_mesh.InitMap(idx);
			}
			return new MaxMNUVWrapper(this->_mesh.M(idx));
		}
		break;
	}
	}
	return NULL;
}

void MaxMNMeshWrapper::DeleteProperty(MeshProperty property, int index) {
	switch (property) {
		case MeshWrapper::VertexColor:
		{
			int idx = 0;
			if (this->_mesh.MNum() > idx) {
				this->_mesh.M(idx)->ClearAndFree();
			}
			break;
		}
		case MeshWrapper::Normal:
		{
			this->_mesh.ClearSpecifiedNormals();
			break;
		}
		case MeshWrapper::UV:
		{
			int idx = index + 1;
			if (this->_mesh.MNum() > idx) {
				this->_mesh.M(idx)->ClearAndFree();
			}
			break;
		}
	}
}

int MaxMNMeshWrapper::GatFaceCount() {
	return this->_mesh.FNum();
}

int MaxMNMeshWrapper::GatFaceDegree(int faceIndex) {
	return this->_mesh.F(faceIndex)->deg;
}

int MaxMNMeshWrapper::GatFaceIndex(int faceIndex, int degreeIndex) {
	return this->_mesh.F(faceIndex)->vtx[degreeIndex];
}