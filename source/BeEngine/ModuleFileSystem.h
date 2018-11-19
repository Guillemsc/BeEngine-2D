#ifndef __MODULE_FILE_SYSTEM_H__
#define __MODULE_FILE_SYSTEM_H__

#include "Module.h"
#include <filesystem>
#include <functional>
#include "ModuleThreadTask.h"

class Event;

struct DecomposedFilePath
{
	std::string file_name;
	std::string file_extension;
	std::string file_extension_lower_case;
	std::string path;
	std::string file_path;
	bool its_folder = false;
};

class Folder
{
public:
	std::vector<DecomposedFilePath> files;
	std::string folder_path;
	std::string folder_name;
	std::vector<Folder> folders;

	bool valid = false;
};

class FileSystem : public Module
{
public:
	FileSystem();
	~FileSystem();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	void OnEvent(Event* ev);

	std::string GetWorkingDirectory();

	std::string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = false);
	bool FileCopyPaste(const char* filepath, const char* new_path, bool overwrite, std::string &resultant_path);
	bool FileCopyPaste(const char* filepath, const char* new_path, bool overwrite);
	void FileCopyPasteWithNewName(const char* filepath, const char* new_path, const char* new_name);
	bool FileDelete(const char* filepath);
	bool FolderDelete(const char* folderpath);
	bool FileSave(const char * path, const char * file_content, const char * name, const char * extension, int size);
	std::vector<std::string> GetFilesAndFoldersInPath(const char* path, const char* extension = "");
	std::vector<std::string> GetFoldersInPath(const char* path);
	std::vector<std::string> GetFilesInPath(const char* path, const char* extension = "");
	std::vector<std::string> GetFilesInPathAndChilds(const char* path);

	Folder GetFilesAndFoldersTree(const char* path);

	bool FileExists(const char* path, const char* name, const char* extension = "");
	bool FileExists(const char* filepath);
	bool FileRename(const char* filepath, const char* new_name, bool check_name_collision = false, std::string& new_filepath = std::string());
	bool FolderRename(const char* filepath, const char* new_name);
	bool FolderExists(const char* path);
	std::string FileRenameOnNameCollision(const char* path, const char* name, const char* extension);

	DecomposedFilePath DecomposeFilePath(std::string file_path);

	bool FilePathInsideFolder(std::string file_path, std::string folder_path);

	std::string SelectFolderDialog(bool& canceled);
	std::string SelectFileDilog(bool& canceled, const char* filter[]);

	// Example file.ex -> .ex
	std::string GetFileExtension(const char* file_name);

	// Example file.ex -> file
	std::string GetFilenameWithoutExtension(const char* file_name);

	// Example C:/user/folder/file.ex -> file.ex
	std::string GetFileNameFromFilePath(const char* file_path);

	// Example C:/user/folder/file.ex -> C:/user/folder/
	std::string GetPathFromFilePath(const char* file_path);

	// Example C:/user/folder/ -> folder
	std::string GetFolderNameFromPath(const char* path);

	// Example C:/user/folder/ -> C:/user/
	std::string GetParentFolder(const char* folder_path);

private:
	// Same name file renaming
	std::string NewNameForFileNameCollision(const char* filename);
	int GetFileNameNumber(const char* filename);
	std::string SetFileNameNumber(const char* filename, int number);

	Folder GetFoldersRecursive(const char* path);
};

#endif // !__MODULE_FILE_SYSTEM_H__