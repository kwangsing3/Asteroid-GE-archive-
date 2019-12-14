#ifndef AGE_FILEBROWSER_H
#define AGE_FILEBROWSER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <filesystem>
#include <GraphicEngine/imgui.h>
#include <AGE_Assert.hpp>

using namespace std::filesystem;

class AGE_FileBrowser
{

public:

	enum class _FileType { None, Directory, Scene, Image, Audio, Model, unKnown };
	struct AGE_FileStruct
	{
		bool beenRefresh = false;
		int _index = -1;
		std::wstring _FileName_wstring;
		std::string _FileName_string;
		_FileType _filetype = _FileType::None;
		std::wstring _path;
		std::vector<AGE_FileStruct*> _filesBelow;
		std::string GetFileType()
		{
			switch (_filetype)
			{
			case AGE_FileBrowser::_FileType::None: return "None";
				break;
			case AGE_FileBrowser::_FileType::Directory:return "Directory";
				break;
			case AGE_FileBrowser::_FileType::Scene:return "Scene";
				break;
			case AGE_FileBrowser::_FileType::Image:return "Image";
				break;
			case AGE_FileBrowser::_FileType::Audio:return "Audio";
				break;
			case AGE_FileBrowser::_FileType::Model:return "Model";
				break;
			case AGE_FileBrowser::_FileType::unKnown:return "unKnown";
				break;
			default:
				AGE_ASSERT(0); //Should never goes here
				break;
			}
		}
	};

	static std::vector<AGE_FileStruct> vec_List_in_Directory;
	static std::vector<AGE_FileStruct> vec_List_in_Detail;
	AGE_FileBrowser(std::string _path)
	{
		_CurrentDirectory_string = _path;
		std::wstring _extension;
		for (char x : _path)
			_extension += x;
		_CurrentDirectory_wstring = _extension;
		_ProjectDirectory_wstring = _CurrentDirectory_wstring;
		Inited();
	}
	AGE_FileBrowser()
	{
		_CurrentDirectory_string = "./";
		std::wstring _extension;
		for (char x : _CurrentDirectory_string)
			_extension += x;
		_CurrentDirectory_wstring = _extension;
		_ProjectDirectory_wstring = _CurrentDirectory_wstring;
		Inited();
	}
	static void ImGUIListTheBrowser();   // For ImGui to call it every frame;

private:
	static path* pathToDisplay;
	static unsigned int TextureFromFiles(const char* path, const std::string& directory);

	static unsigned int DirectoryPicdata;
	static unsigned int DocPicdata;
	//static void Refresh();
	static void Refresh(AGE_FileStruct* _filestruct);
	static std::vector<AGE_FileStruct*> Refresh_below(std::wstring _NewPath);

protected:

	static std::string WstringToString(std::wstring _input)
	{
		std::string _output;
		for (char x : _input)
			_output += x;
		return _output;
	}
	static void Inited();
	static void IfitIsaDirectory(AGE_FileStruct* _fst);
	static _FileType DefineFileType(const directory_entry& _entry);

	static std::wstring _ProjectDirectory_wstring;
	static std::wstring _CurrentDirectory_wstring;
	static std::string _CurrentDirectory_string;
	static AGE_FileStruct* Dir_currentSelect, *Dir_ProjectBase;
	static AGE_FileStruct* PreparedForCopy;


};



// refresh 的時候再去讀取當前目錄的檔案應該就可以了。













#endif // !AGE_FILEBROWSER_H
