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
	std::string folder_name;
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

	std::string GetWorkingDirectory();
	std::string GetExecutableFilePath();

	// Files
	bool FileExists(const std::string& filepath); 
	bool FileDelete(const std::string& filepath); 
	bool FileRename(const std::string& filepath, const std::string& new_name, bool check_name_collision = false, std::string& new_filepath = std::string()); 
	bool FileCopyPaste(const std::string& filepath, const std::string& new_path, bool check_name_collision = false, std::string &resultant_path = std::string()); 
	bool FileSave(const std::string& path, const std::string& name, const std::string& extension, const char * file_content, uint size); 
	bool FileRead(const std::string& filepath, std::string& returned_file_data);
	bool FileRead(const std::string& filepath, char*& returned_file_data, uint &data_size);

	// Folders
	bool FolderExists(const std::string& path); 
	bool FolderCreate(const std::string& path, const std::string& name, bool check_name_collision = false, std::string& new_folderpath = std::string());
	bool FolderDelete(const std::string& folderpath); 
	bool FolderCopyPaste(const std::string& folderpath, const std::string& folderpath_destination, bool check_name_collision = false, std::string& new_folderpath = std::string());
	std::string FolderParent(const std::string& folder); 
	bool FolderRename(const std::string& folderpath, const std::string& new_name, bool check_name_collision = false, std::string& new_folderpath = std::string()); 

	DecomposedFilePath DecomposeFilePath(const std::string& file_path); 

	std::vector<std::string> GetFilesAndFoldersInPath(const std::string& path, const std::string& extension = "");
	std::vector<std::string> GetFoldersInPath(const std::string& path);
	std::vector<std::string> GetFilesInPath(const std::string& path, const std::string& extension = "");
	std::vector<std::string> GetFilesInPathAndChilds(const std::string& path, const std::string& extension = "");

	Folder GetFilesAndFoldersTree(const std::string& path);

	bool FilePathInsideFolder(const std::string& file_path, const std::string& folder_path);
	std::string SubstractFolder(const std::string& folder, const std::string& folder_to_substract);

	std::string SelectFolderDialog(bool& canceled);
	std::string SelectFileDilog(bool& canceled, const char* filter[]);

	std::string GetFileNameOnNameCollision(const std::string& filepath); 
	std::string GetFolderNameOnNameCollision(const std::string& path); 

private:
	// Same name file renaming
	std::string NewNameForNameCollision(const char* filename);
	int GetNameCollisionNumber(const char* filename);
	std::string SetNameCollisionNumber(const char* filename, int number);
};

#endif // !__MODULE_FILE_SYSTEM_H__