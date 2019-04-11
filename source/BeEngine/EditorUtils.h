#ifndef __EDITOR_UTILS_H__
#define __EDITOR_UTILS_H__

class GameObject;

class EditorUtils
{
public:
	EditorUtils();

	static bool NewGameObjectButton(GameObject* parent = nullptr);
	static bool DuplicateGameObjectButton(GameObject* to_duplicate);
	static void ImportAssetButton();
};

#endif // !__EDITOR_UTILS_H__