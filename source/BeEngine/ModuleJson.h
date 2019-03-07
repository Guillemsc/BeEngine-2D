#ifndef __MODULE_JSON_H__
#define __MODULE_JSON_H__

#include "Module.h"
#include "Parson\parson.h"

#include "GeometryMath.h"

#include <map>

class DataAbstraction
{
public:
	DataAbstraction();

	void Clear();

	void AddInt(const std::string& name, int val);
	void AddBool(const std::string& name, bool val);
	void AddFloat(const std::string& name, float val);
	void AddString(const std::string& name, std::string val);
	void AddFloat2(const std::string& name, float2 val);
	void AddFloat3(const std::string& name, float3 val);
	void AddFloat4(const std::string& name, float4 val);
	void AddFloat2Vector(const std::string& name, const std::vector<float2> val);

	int GetInt(const std::string& name, int def = 0);
	bool GetBool(const std::string& name, bool def = false);
	float GetFloat(const std::string& name, float def = 0.0f);
	std::string GetString(const std::string& name, std::string def = "");
	float2 GetFloat2(const std::string& name, float2 def = float2::zero);
	float3 GetFloat3(const std::string& name, float3 def = float3::zero);
	float4 GetFloat4(const std::string& name, float4 def = float4::zero);
	std::vector<float2> GetFloat2Vector(const std::string& name);

	void Serialize(JSON_Doc& doc);
	void DeSerialize(JSON_Doc& doc);

private:
	std::map<std::string, int>		           ints;
	std::map<std::string, bool>		           bools;
	std::map<std::string, float>               floats;
	std::map<std::string, std::string>         strings;
	std::map<std::string, float2>              floats2;
	std::map<std::string, float3>              floats3;
	std::map<std::string, float4>              floats4;
	std::map<std::string, std::vector<float2>> floats2_vector;
};

class JSON_Doc
{
public:
	JSON_Doc();
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	JSON_Doc(JSON_Doc& doc);
	~JSON_Doc();

	void SetString(const std::string& set, const char* str);
	void SetBool(const std::string& set, bool bo);
	void SetNumber(const std::string& set, double nu);
	void SetNumber2(const std::string& set, float2 val);
	void SetNumber3(const std::string& set, float3 val);
	void SetNumber4(const std::string& set, float4 val);
	const char* GetString(const std::string& str, const char* defaul = "");
	const bool GetBool(const std::string& bo, bool defaul = false);
	const double GetNumber(const std::string& nu, double defaul = 0);
	const float2 GetNumber2(const std::string& fl, float2 defaul = float2(0, 0));
	const float3 GetNumber3(const std::string& fl, float3 defaul = float3(0, 0, 0));
	const float4 GetNumber4(const std::string& fl, float4 defaul = float4(0, 0, 0, 0));

	void SetArray(const std::string& set);
	bool ArrayExists(const std::string& arr);
	void ClearArray(const std::string& arr);
	void RemoveArrayIndex(const std::string& arr, int index);
	void AddStringToArray(const std::string& arr, const char* str);
	void AddBoolToArray(const std::string& arr, bool bo);
	void AddNumberToArray(const std::string& arr, double set);
	void AddNumber2ToArray(const std::string& arr, float2 set);
	void AddSectionToArray(const std::string& arr);
	bool MoveToSectionFromArray(const std::string& arr, int index);
	int GetArrayCount(const std::string& set) const;
	const char* GetStringFromArray(const std::string& arr, int index);
	bool GetBoolFromArray(const std::string& arr, int index);
	double GetNumberFromArray(const std::string& arr, int index);
	float2 GetNumber2FromArray(const std::string& arr, int index);


	bool MoveToSection(const std::string& set);
	void RemoveSection(const std::string& set);
	void MoveToRoot();
	void AddSection(const std::string& set);

	JSON_Doc GetNode();

	void Clear();

	std::string GetPath();
	void Save();
	void CleanUp();

private:
	bool FindValue(const char* str, json_value_type type);
	bool FindArrayValue(const char* array, int index, json_value_type type);

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;
	std::string		 path;
};

class ModuleJson : public Module
{
public:
	ModuleJson();
	~ModuleJson();

	bool Awake();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char * path, const char* name, const char* extension);
	JSON_Doc* CreateJSON(const char * filepath);
	void UnloadJSON(JSON_Doc* path);

private:
	void UnloadAllJSONs();

private:
	std::list<JSON_Doc*> jsons;
};

#endif // !__MODULE_JSON_H__
