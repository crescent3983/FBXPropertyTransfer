#include "FBXPropertyTransfer.h"
#include <map>
#include "Utility.h"
#include "Wrapper/FbxMesh/FbxMeshWrapper.h"
#include "Wrapper/FbxMesh/FbxMeshRefWrapper.h"
#include "Wrapper/MaxMesh/MaxMeshWrapper.h"
#include "Wrapper/MaxMNMesh/MaxMNMeshWrapper.h"
#include "maxscript/maxscript.h"
#include "modstack.h"

#define PLUGIN_NAME "FBXPropertyTransfer"
#define NAME_LEN 50
#define PROPERTY_LEN 20

#define EXPORT_PRINT(f_, ...) mprintf(_M("[Export]" f_), ##__VA_ARGS__)
#define IMPORT_PRINT(f_, ...) mprintf(_M("[Import]" f_), ##__VA_ARGS__)

class FBXPropertyTransferPlugin : public FbxPlugin
{
	FBXSDK_PLUGIN_DECLARE(FBXPropertyTransferPlugin);

protected:
	explicit FBXPropertyTransferPlugin(const FbxPluginDef& pDefinition, FbxModule pFbxModule) : FbxPlugin(pDefinition, pFbxModule)
	{
	}

	// Implement kfbxmodules::FbxPlugin
	virtual bool SpecificInitialize()
	{
		return true;
	}

	virtual bool SpecificTerminate()
	{
		return true;
	}
};

FBXSDK_PLUGIN_IMPLEMENT(FBXPropertyTransferPlugin);

// FBX Interface
extern "C"
{
    //The DLL is owner of the plug-in
    static FBXPropertyTransferPlugin* sPlugin = NULL;

    //This function will be called when an application will request the plug-in
    EXPORT_DLL void FBXPluginRegistration(FbxPluginContainer& pContainer, FbxModule pFbxModule)
    {
        if( sPlugin == NULL )
        {
            //Create the plug-in definition which contains the information about the plug-in
            FbxPluginDef sPluginDef;
            sPluginDef.mName = PLUGIN_NAME;
            sPluginDef.mVersion = "1.0";

            //Create an instance of the plug-in.  The DLL has the ownership of the plug-in
            sPlugin = FBXPropertyTransferPlugin::Create(sPluginDef, pFbxModule);

            //Register the plug-in
            pContainer.Register(*sPlugin);
        }
    }

	FBXSDK_MAX_EXTENSION_DECLARE();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Internal Functions

static std::map<FbxMesh*, FbxMeshRef*> mapping;

bool ParsePropertyName(FbxNode* fbxNode, char* name, int nLen, wchar_t* wname, int wnLen,
	std::pair<MeshWrapper::MeshProperty, int>& src, std::pair<MeshWrapper::MeshProperty, int>& dst) {
	if (CheckIsMesh(fbxNode)) {
		char srcProperty[PROPERTY_LEN], dstProperty[PROPERTY_LEN];
		if (ParseArgument(fbxNode->GetName(), name, nLen, srcProperty, PROPERTY_LEN, dstProperty, PROPERTY_LEN)) {
			src = NameToProperty(srcProperty);
			dst = NameToProperty(dstProperty);

			mbstowcs(wname, name, wnLen);

			return true;
		}
		return false;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Max Extension Export Functions
bool MaxExt_ExportHandled(INode* pMaxObject)
{
	//Called for each Max object that *is going to* be translated into fbx common data.
	//Sometimes, for different reasons, this might be called more than once for the same object.
	//
	//The purpose of this function is to tell the plugin if we are going to handle the creation
	//of this Max object in this extension plugin or if we let the plugin work as usual.
	//
	//So returning TRUE means the corresponding fbx object for this particuliar Max object won't
	//be created, and we'll have to create it ourselves, so we say "we handle it". Otherwise,
	//returning FALSE will simply result in the usual behavior, which means the plugin will
	//translate the Max object into the corresponding fbx object, if it knows how-to, of course.
	return false;
}

void MaxExt_ExportBegin(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//Called before we export anything into the fbx scene.
	//
	//We could choose to create our custom data now or wait at the end of the export
	//depending if that custom data replaces objects in the fbx scene hierarchy.
}

bool MaxExt_ExportProcess(FbxObject** pOutputFbxObject, ReferenceTarget* pInputObject, FbxScene* pFbxScene)
{
    //Called during the export process of the following objects:
    //
    //Right after the FbxNode is created but before the FbxNodeAttribute.
    //The purpose of this function is to intercept the default creation of the FBX object based on the 3dsMax
    //object and replace it with this extension plugin version. If this function returns TRUE it will mean that
    //all the required manipulation has been perfomed here and no further processing is required by the exporter.
    //Inversely, if we return FALSE, we let the exporter process the data as usual.
    //
    //The proper implementation of this function requires to check what is the inputObject type
    //and based on his type, we either need to :
    //
    //   - create the corresponding FbxObject type (this is typically a custom Fbx class) when the content
    //     of pOutputFbxObject is NULL
    //
    //or
    //
    //   - create and add the appropriate FBX data to the existing pOutputFbxObject.
    //
    //Notes:
    //  1) This callback can also be called with pOutputFbxObject = NULL and pFbxScene = NULL. In this
    //     case, it should simply return true or false depending if it can handle the input object (no
    //     other action is required)
    //  2) In case 2 above, this callback is responsible for maintaining the integrity of the
    //     received FbxObject and all its existing connections.
    //
    return false;
}

void MaxExt_ExportTranslated(FbxObject* pFbxObject, INode* pMaxObject)
{
	//Called after the Max scene has been translated into an fbx scene, for each
	//object that was convertable. In other words, objects unknown or unsupported
	//by the FbxSdk won't be called in this function.
	//
	//The purpose of this function is to tell the extension plugin what is the corresponding
	//fbx object for that particuliar Max object. This is especially useful when connections
	//need to be preserved between plugin and extension plugin fbx objects.
	if(pFbxObject == NULL || pMaxObject == NULL){
		return;
	}

	FbxNode* fbxNode = FbxCast<FbxNode>(pFbxObject);

	// Parse property name
	char name[NAME_LEN];
	wchar_t wname[NAME_LEN];
	std::pair<MeshWrapper::MeshProperty, int> src, dst;
	if (!ParsePropertyName(fbxNode, name, NAME_LEN, wname, NAME_LEN, src, dst)) {
		return;
	}

	// Check modifiers in max object
	Object* ObjectPtr = pMaxObject->GetObjectRef();
	if (ObjectPtr && ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID) {
		IDerivedObject *DerivedObjectPtr = ( IDerivedObject *)(ObjectPtr);
		int modifiers = DerivedObjectPtr->NumModifiers();
		if (modifiers > 0) {
			EXPORT_PRINT("%s has modifers, it may lead to weird result(count: %d)\n", wname, modifiers);
		}
	}

	// Check property name
	if(src.first == MeshWrapper::None || dst.first == MeshWrapper::None) {
		EXPORT_PRINT("Property is unrecongnized(%s)\n", fbxNode->GetName());
		return;
	}
	fbxNode->SetName(name);

	// Check face is match
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	MeshWrapper* fbxWrapper = new FbxMeshWrapper(fbxMesh);
	PropertyWrapper* fbxProperty = fbxWrapper->GetProperty(dst.first, dst.second, true);

	MeshWrapper* maxWrapper = NULL;
	PropertyWrapper* maxProperty = NULL;

	bool deleteTri = false, deletePoly = false;
	TriObject *triObject = NULL;
	PolyObject *polyObject = NULL;

	triObject = GetTriObjectFromNode(pMaxObject, GetCOREInterface()->GetTime(), deleteTri);
	if (triObject != NULL && MeshWrapper::CheckFaceIsMatch(*fbxWrapper, MaxMeshWrapper(triObject->GetMesh()))) {
		Mesh &maxMesh = triObject->GetMesh();
		maxWrapper = new MaxMeshWrapper(maxMesh);
		maxProperty = maxWrapper->GetProperty(src.first, src.second);
	}
	else {
		polyObject = GetPolyObjectFromNode(pMaxObject, GetCOREInterface()->GetTime(), deletePoly);
		if (polyObject != NULL && MeshWrapper::CheckFaceIsMatch(*fbxWrapper, MaxMNMeshWrapper(polyObject->GetMesh()))) {
			MNMesh &maxMesh = polyObject->GetMesh();
			maxWrapper = new MaxMNMeshWrapper(maxMesh);
			maxProperty = maxWrapper->GetProperty(src.first, src.second);
		}
		else {
			EXPORT_PRINT("Can't find proper mesh or face count is not match\n");
			if (deleteTri) triObject->DeleteMe();
			if (deletePoly) polyObject->DeleteMe();
			return;
		}
	}

	// Copy Operations
	try {
		std::vector<int> map;
		if (MeshWrapper::BuildFaceMappingTable(*maxWrapper, *fbxWrapper, map)) {
			bool result = PropertyWrapper::CopyProperty(maxProperty, fbxProperty, map);
			EXPORT_PRINT("%s copy from %s%d to %s%d : %s\n", wname,
				PropertyToName(src.first), src.second,
				PropertyToName(dst.first), dst.second,
				(result ? _M("Success") : _M("Fail")));

			// tanget and binormal should both exist
			if (dst.first == MeshWrapper::Tangent) {
				if (fbxProperty) delete fbxProperty; fbxProperty = NULL;
				fbxProperty = fbxWrapper->GetProperty(MeshWrapper::Binormal, 0, true);
				PropertyWrapper::CopyProperty(maxProperty, fbxProperty, map);
			}
			else if (dst.first == MeshWrapper::Binormal) {
				if (fbxProperty) delete fbxProperty; fbxProperty = NULL;
				fbxProperty = fbxWrapper->GetProperty(MeshWrapper::Tangent, 0, true);
				PropertyWrapper::CopyProperty(maxProperty, fbxProperty, map);
			}
		}
		else {
			EXPORT_PRINT("%s: Face mapping table is built failed\n", wname);
		}
	}
	catch (...) {
		EXPORT_PRINT("There are some errors when handling %s\n", wname);
	}

	// Release resources
	if (maxWrapper) delete maxWrapper;
	if (maxProperty) delete maxProperty;
	if (fbxWrapper) delete fbxWrapper;
	if (fbxProperty) delete fbxProperty;
	if (deleteTri) triObject->DeleteMe();
	if (deletePoly) polyObject->DeleteMe();
}

void MaxExt_ExportEnd(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//Called after the scene has been fully translated into fbx. This is the last function called
	//before the extension plugin is done with the export process. Any clean-up or last minute
	//modification to the scene must be done now
	mflush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Max Extension Import Functions
bool MaxExt_ImportHandled(FbxObject* pFbxObject)
{
	//This is called by the importer everytime its about to translate the FBX object
	//into a Max object. If the extension plugin should handle the creation, then
	//it needs to answer TRUE to this call.
	return false;
}

void MaxExt_ImportBegin(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//This is called the the beginning of the import process,
	//allowing us to act on the FBX scene before its loaded from the file.
	mapping.clear();
}

bool MaxExt_ImportProcess(ReferenceTarget** pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge)
{
    //This is called by the importer everytime it is about to create one of the following 3dsMax object:
    //
    //
    //The purpose of this function is to intercept the default creation of the 3dsMax object (from the FBX object)
    //and replace it with this extension plugin version. If this function returns TRUE it will mean that
    //all the required manipulation has been perfomed here and no further processing is required by the importer.
    //Inversely, if we return FALSE, we let the importer process the data as usual.
    //
    // Note that if pOutputObject = NULL and pIsAnInstance = false and pMerge = false, this callback should simply
    // test to see if it can handle the conversionof pInputFbxObject and return true or false accordingly. No other
    // action is required.
	if(pInputFbxObject == NULL){
		return false;
	}

	FbxNode* fbxNode = FbxCast<FbxNode>(pInputFbxObject);
	if(CheckIsMesh(fbxNode)){
		FbxMesh* fbxMesh = fbxNode->GetMesh();
		mapping.insert(std::pair<FbxMesh*, FbxMeshRef*>(fbxMesh, new FbxMeshRef(fbxMesh)));
	}

    return false;
}

void MaxExt_ImportTranslated(FbxObject* pFbxObject, INode* pMaxObject)
{
	//This is called everytime an FBX object got converted into a Max object
	//while we are traversing the scene during the translation process.
	if(pFbxObject == NULL || pMaxObject == NULL){
		return;
	}

	FbxNode* fbxNode = FbxCast<FbxNode>(pFbxObject);

	// Parse property name
	char name[NAME_LEN];
	wchar_t wname[NAME_LEN];
	std::pair<MeshWrapper::MeshProperty, int> src, dst;
	if (!ParsePropertyName(fbxNode, name, NAME_LEN, wname, NAME_LEN, src, dst)) {
		return;
	}

	// Check property name
	if (src.first == MeshWrapper::None || dst.first == MeshWrapper::None) {
		IMPORT_PRINT("Property is unrecongnized(%s)\n", fbxNode->GetName());
		return;
	}
	pMaxObject->SetName(wname);

	// Check face is match
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	FbxMeshRef* ref = NULL;
	std::map<FbxMesh*, FbxMeshRef*>::iterator it = mapping.find(fbxMesh);
	if (it != mapping.end()) {
		ref = it->second;
	}
	else {
		return;
	}
	MeshWrapper* fbxWrapper = new FbxMeshRefWrapper(ref);
	PropertyWrapper* fbxProperty = fbxWrapper->GetProperty(src.first, src.second);

	MeshWrapper* maxWrapper = NULL;
	PropertyWrapper* maxProperty = NULL;

	bool deleteTri = false, deletePoly = false;
	TriObject *triObject = NULL;
	PolyObject *polyObject = NULL;

	triObject = GetTriObjectFromNode(pMaxObject, GetCOREInterface()->GetTime(), deleteTri);
	if (triObject != NULL && !deleteTri && MeshWrapper::CheckFaceIsMatch(*fbxWrapper, MaxMeshWrapper(triObject->GetMesh()))) {
		Mesh &maxMesh = triObject->GetMesh();
		maxWrapper = new MaxMeshWrapper(maxMesh);
		maxProperty = maxWrapper->GetProperty(dst.first, dst.second, true);
	}
	else {
		polyObject = GetPolyObjectFromNode(pMaxObject, GetCOREInterface()->GetTime(), deletePoly);
		if (polyObject != NULL && !deletePoly && MeshWrapper::CheckFaceIsMatch(*fbxWrapper, MaxMNMeshWrapper(polyObject->GetMesh()))) {
			MNMesh &maxMesh = polyObject->GetMesh();
			maxWrapper = new MaxMNMeshWrapper(maxMesh);
			maxProperty = maxWrapper->GetProperty(dst.first, dst.second, true);
		}
		else {
			IMPORT_PRINT("Can't find proper mesh or face count is not match\n");
			if (deleteTri) triObject->DeleteMe();
			if (deletePoly) polyObject->DeleteMe();
			return;
		}
	}

	// Copy Operations
	try {
		std::vector<int> map;
		if (MeshWrapper::BuildFaceMappingTable(*fbxWrapper, *maxWrapper, map)) {
			bool result = PropertyWrapper::CopyProperty(fbxProperty, maxProperty, map);
			IMPORT_PRINT("%s copy from %s%d to %s%d : %s\n", wname,
				PropertyToName(src.first), src.second,
				PropertyToName(dst.first), dst.second,
				(result ? _M("Success") : _M("Fail")));
		}
		else {
			IMPORT_PRINT("%s: Face mapping table is built failed\n", wname);
		}
	}
	catch (...) {
		IMPORT_PRINT("There are some errors when handling %s\n", wname);
	}

	// Release resources
	if (maxWrapper) delete maxWrapper;
	if (maxProperty) delete maxProperty;
	if (fbxWrapper) delete fbxWrapper;
	if (fbxProperty) delete fbxProperty;
	if (deleteTri) triObject->DeleteMe();
	if (deletePoly) polyObject->DeleteMe();
}

void MaxExt_ImportEnd(FbxScene* pFbxScene, INode* pMaxRootNode)
{
	//This is called at the very end of the import.
	for (std::map<FbxMesh*, FbxMeshRef*>::iterator it = mapping.begin(); it != mapping.end(); it++) {
		if(it->second){
			delete it->second;
		}
	}
	mapping.clear();
	mflush();
}