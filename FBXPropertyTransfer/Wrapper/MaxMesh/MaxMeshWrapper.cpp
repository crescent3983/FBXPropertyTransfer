#include "MaxMeshWrapper.h"
#include "MaxVertexColorWrapper.h"
#include "MaxNormalWrapper.h"
#include "MaxUVWrapper.h"

#define VERTEX_COLOR_MAPCHANNEL 0
#define ALPHA_MAPCHANNEL -2

MaxMeshWrapper::MaxMeshWrapper(Mesh& mesh) : _mesh(mesh) {

}

MaxMeshWrapper::~MaxMeshWrapper(){

}

PropertyWrapper* MaxMeshWrapper::GetProperty(MeshProperty property, int index, bool createIfNotExist) {
	switch(property){
		case MeshWrapper::VertexColor:
			{
				int idx = VERTEX_COLOR_MAPCHANNEL;
				if(this->_mesh.getNumMaps() < idx+1 && createIfNotExist){
					this->_mesh.setNumMaps(idx+1, TRUE);	
				}
				if(this->_mesh.getNumMaps() > idx){
					this->_mesh.setMapSupport(idx);
					this->_mesh.setMapSupport(ALPHA_MAPCHANNEL);
					return new MaxVertexColorWrapper(this->_mesh.Map(idx), this->_mesh.Map(ALPHA_MAPCHANNEL));
				}
				break;
			}
		case MeshWrapper::Normal:
			{
				MeshNormalSpec *normal = this->_mesh.GetSpecifiedNormals();
				if((normal == NULL) || (normal != NULL && !normal->GetFlag(MESH_NORMAL_NORMALS_BUILT))){
					this->_mesh.SpecifyNormals();
					normal = this->_mesh.GetSpecifiedNormals();
				}
				if(normal){
					normal->CheckNormals();
					return new MaxNormalWrapper(normal);
				}
				break;
			}
		case MeshWrapper::UV:
			{
				int idx = index + 1;
				if(this->_mesh.getNumMaps() < idx+1 && createIfNotExist){
					this->_mesh.setNumMaps(idx+1, TRUE);
				}
				if(idx >= 0 && this->_mesh.getNumMaps() > idx){
					this->_mesh.setMapSupport(idx);
					return new MaxUVWrapper(this->_mesh.Map(idx));
				}
				break;
			}
	}
	return NULL;
}

void MaxMeshWrapper::DeleteProperty(MeshProperty property, int index) {
	switch(property){
		case MeshWrapper::VertexColor:
			{
				int idx = 0;
				if(this->_mesh.getNumMaps() > idx){
					this->_mesh.Map(idx).Clear();
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
				if(this->_mesh.getNumMaps() > idx){
					this->_mesh.Map(idx).Clear();
				}
				break;
			}
	}
}

int MaxMeshWrapper::GatFaceCount() {
	return this->_mesh.getNumFaces();
}

int MaxMeshWrapper::GatFaceDegree(int faceIndex) {
	return TRIMESH_FACE_VERTEX;
}

int MaxMeshWrapper::GatFaceIndex(int faceIndex, int degreeIndex) {
	return this->_mesh.faces[faceIndex].getVert(degreeIndex);
}