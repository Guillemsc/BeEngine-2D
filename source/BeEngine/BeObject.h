#ifndef __BE_OBJECT_H__
#define __BE_OBJECT_H__

class ScriptingBridgeObject;

class BeObject
{
public:
	BeObject(ScriptingBridgeObject* bridge_object);
	~BeObject();

	void InitBeObject();
	void CleanUpBeObject();

	ScriptingBridgeObject* GetScriptingBridge() const;

private:
	ScriptingBridgeObject* bridge_object = nullptr;
};

#endif // !__BE_OBJECT_H__