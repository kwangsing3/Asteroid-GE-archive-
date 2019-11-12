#ifndef AGE_FILEBROWSER
#define AGE_FILEBROWSER


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <GraphicEngine/imgui.h>


using namespace std::filesystem;

static class AGE_FileBrowser
{
private:
	static path * pathToDisplay;
	static unsigned int TextureFromFiles(const char* path, const std::string& directory);
	
	static unsigned int DirectoryPicdata;
	static unsigned int DocPicdata;
	static void Refresh();
	static void Refresh_Detail(std::wstring _NewPath);


protected:
	static void Inited();
	static std::wstring _CurrentDirectory;
public:

	enum class _FileType { None, Directory, Scene, Image, Audio, Model, unKnown };
	struct AGE_FileStruct
	{
		std::wstring FileName;
		_FileType _filetype = _FileType::None;
		std::wstring _path;
	};

	static std::vector<AGE_FileStruct> vec_List_in_Directory;
	static std::vector<AGE_FileStruct> vec_List_in_Detail;




	AGE_FileBrowser(std::string _path)
	{
		std::wstring _extension;
		for (char x : _path)
			_extension += x;

		//pathToDisplay = new path(_path);
		_CurrentDirectory = _extension;	
		
		Inited();
	}
	static void ImGUIListTheBrowser();   // For ImGui to call it every frame;

};



// refresh ���ɭԦA�hŪ����e�ؿ����ɮ����ӴN�i�H�F�C













#endif // !AGE_FILEBROWSER
