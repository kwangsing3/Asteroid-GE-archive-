#ifndef AGE_FILEBROWSER
#define AGE_FILEBROWSER


#include <iostream>
#include <filesystem>
#include <GraphicEngine/imgui.h>

using namespace std::filesystem;

static class AGE_FileBrowser
{
private:
	static path* pathToDisplay;

protected:

	
	void Inited();
public:

	enum class _FileType { None, Directory, Scene, Image, Audio, Model, unKnown };
	struct AGE_FileStruct
	{
		std::string FileName;
		_FileType _filetype = _FileType::None;
		std::string _path;
	};
	static std::vector<AGE_FileStruct> vec_FileBrowser;
	static std::string _CurrentDirectory;

	AGE_FileBrowser(std::string _path)
	{
		pathToDisplay = new path(_path);
		_CurrentDirectory = _path;

		Inited();
	}
	static void ImGUIListTheBrowser();   // For ImGui to call it every frame;
	static void Refresh();
};



// refresh 的時候再去讀取當前目錄的檔案應該就可以了。













#endif // !AGE_FILEBROWSER
