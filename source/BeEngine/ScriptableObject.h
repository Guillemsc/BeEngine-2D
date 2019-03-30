#ifndef __SCRIPTING_OBJECT_H__
#define __SCRIPTING_OBJECT_H__

#include "Globals.h"
#include "ScriptingBridgeObject.h"

class ScriptableObject
{
	friend class ModuleScripting;

public:
	ScriptableObject(ScriptingBridgeObject* scripting_brige_object);
	~ScriptableObject();

	void StartScriptableObject();
	void CleanUpScriptableObject();

	ScriptingBridgeObject* GetScriptingBridge() const;
	
private:
	ScriptingBridgeObject* scripting_bridge = nullptr;

};

#endif // !__SCRIPTING_OBJECT_H__