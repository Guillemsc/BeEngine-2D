#ifndef __SCRIPTING_OBJECT_H__
#define __SCRIPTING_OBJECT_H__

#include "Globals.h"

class ScriptingObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingObject();
	~ScriptingObject();
	
	virtual void Start(){}
	virtual void CleanUp(){}

protected:
	bool loaded = false;
	bool ready_to_use = false;
};

#endif // !__SCRIPTING_OBJECT_H__