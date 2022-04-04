#ifndef _3DS_FBX_EXTENSION_H_
#define _3DS_FBX_EXTENSION_H_

#if defined(WIN32) || defined(WIN64)
	#define EXPORT_DLL __declspec(dllexport)
#else
	#define EXPORT_DLL
#endif

#include <max.h>

#include <fbxsdk.h>

#define FBXSDK_MAX_EXTENSION_DECLARE()\
	EXPORT_DLL bool MaxExt_IsExtension(){return true;}\
	\
	EXPORT_DLL bool MaxExt_ExportHandled(INode* pMaxObject);\
	EXPORT_DLL void MaxExt_ExportBegin(FbxScene* pFbxScene, INode* pMaxRootNode);\
    EXPORT_DLL bool MaxExt_ExportProcess(FbxObject** pOutputFbxObject, ReferenceTarget* pMaxObject, FbxScene* pFbxScene);\
	EXPORT_DLL void MaxExt_ExportTranslated(FbxObject* pFbxObject, INode* pMaxObject);\
	EXPORT_DLL void MaxExt_ExportEnd(FbxScene* pFbxScene, INode* pMaxRootNode);\
	\
	EXPORT_DLL bool MaxExt_ImportHandled(FbxObject* pFbxObject);\
	EXPORT_DLL void MaxExt_ImportBegin(FbxScene* pFbxScene, INode* pMaxRootNode);\
    EXPORT_DLL bool MaxExt_ImportProcess(ReferenceTarget** pOutputObject, FbxObject* pInputFbxObject, bool pIsAnInstance, bool pMerge);\
	EXPORT_DLL void MaxExt_ImportTranslated(FbxObject* pFbxObject, INode* pMaxObject);\
	EXPORT_DLL void MaxExt_ImportEnd(FbxScene* pFbxScene, INode* pMaxRootNode);\

#endif
