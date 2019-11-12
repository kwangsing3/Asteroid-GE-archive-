
#include <AGE_FileBrowser.h>
#include <string>
#include <AGE_Assert.h>
#include <comdef.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

path* AGE_FileBrowser::pathToDisplay;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_List_in_Directory;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_List_in_Detail;
std::wstring AGE_FileBrowser::_CurrentDirectory;
unsigned int AGE_FileBrowser::DirectoryPicdata;
unsigned int AGE_FileBrowser::DocPicdata;


AGE_FileBrowser::_FileType DefineFileType(const directory_entry& _entry)
{
	AGE_ASSERT(_entry.exists());  // if file isn't exists or got a wrong or ghost file. 


	
	std::string _extension;
	for (char x : _entry.path().extension().wstring())
		_extension += x;


	//std::string _extension ( _entry.path().extension().wstring().begin(), _entry.path().extension().wstring().end());
	
	// ©Î³\¥i¥H¥Î­µ·½®w¨Ó©w¸q»¡¬O¤£¬O­µ¼ÖÀÉ  ¦A¨Ó°µ¨ä¥L¤ÀÃþ
	if (_extension == ".AstGamEng")
	{
		return AGE_FileBrowser::_FileType::Scene;
	}
	else if (_extension == ".jpg"||".png"||".gif")
	{
		return AGE_FileBrowser::_FileType::Image;
	}
	else if (_extension == ".mp3" || ".mp4" || ".avi")
	{
		return AGE_FileBrowser::_FileType::Audio;
	}
	else if (_extension == ".fbx" || ".obj" || ".dae")
	{
		return AGE_FileBrowser::_FileType::Model;
	}

	return AGE_FileBrowser::_FileType::unKnown;
}
inline const char* ToString(AGE_FileBrowser::_FileType v)
{
	switch (v)
	{
	case AGE_FileBrowser::_FileType::None:return "None";
		break;
	case AGE_FileBrowser::_FileType::Directory: return "Directory";
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
	default:return "unKnown";
		break;
	}
}
unsigned int AGE_FileBrowser::TextureFromFiles(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
void AGE_FileBrowser::Inited()
{
	//Load Image
	DirectoryPicdata = TextureFromFiles("icon-dir.png","D:/Desktop/Asteroid-GameEngine/Asteroid-GameEngine/Texture");
	DocPicdata = TextureFromFiles("icon-doc.png", "D:/Desktop/Asteroid-GameEngine/Asteroid-GameEngine/Texture");

	Refresh();
	

	return;
}





void AGE_FileBrowser::Refresh()
{
	if (pathToDisplay != NULL) delete(pathToDisplay);
	pathToDisplay = new path(_CurrentDirectory);
	if (!pathToDisplay->is_absolute())  AGE_PRINTCONSLE("Got ghost directory path");


	if (!AGE_FileBrowser::vec_List_in_Detail.empty()) AGE_FileBrowser::vec_List_in_Detail.clear();
	if (!AGE_FileBrowser::vec_List_in_Directory.empty()) AGE_FileBrowser::vec_List_in_Directory.clear();
	// Load  Directory first
	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		if (!entry.is_directory()) continue;
		AGE_FileStruct _newFile;

		_newFile.FileName = entry.path().filename().wstring();
		_newFile._filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
		_newFile._path = entry.path().wstring();
		AGE_FileBrowser::vec_List_in_Directory.push_back(_newFile);
		AGE_FileBrowser::vec_List_in_Detail.push_back(_newFile);
	}
	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		if (!entry.exists())  AGE_PRINTCONSLE("unknown situation");
		if (entry.is_directory()) continue;
		AGE_FileStruct _newFile;
		_newFile.FileName = entry.path().filename().wstring();
		_newFile._filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
		_newFile._path = entry.path().wstring();
		AGE_FileBrowser::vec_List_in_Detail.push_back(_newFile);
	}


	return;
}
void AGE_FileBrowser::Refresh_Detail(std::wstring _NewPath)    //現在的情況是只要是True他就會一直讀取
{
	_CurrentDirectory = _NewPath;

	if (!AGE_FileBrowser::vec_List_in_Detail.empty()) AGE_FileBrowser::vec_List_in_Detail.clear();
	if (pathToDisplay != NULL) delete(pathToDisplay);
	pathToDisplay = new path(_CurrentDirectory);
	if (!pathToDisplay->is_absolute())  AGE_PRINTCONSLE("Got ghost directory path");


	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		if (!entry.is_directory()) continue;
		AGE_FileStruct _newFile;

		_newFile.FileName = entry.path().filename().wstring();
		_newFile._filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
		_newFile._path = entry.path().wstring();
		AGE_FileBrowser::vec_List_in_Detail.push_back(_newFile);
	}
	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		if (!entry.exists())  AGE_PRINTCONSLE("unknown situation");
		if (entry.is_directory()) continue;

		
			AGE_FileStruct _newFile;
			_newFile.FileName = entry.path().filename().wstring();
			_newFile._filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
			_newFile._path = entry.path().wstring();
			AGE_FileBrowser::vec_List_in_Detail.push_back(_newFile);
	
	}



}
void AGE_FileBrowser::ImGUIListTheBrowser()
{
	//DirectoryPicdata = TextureFromFile("icon-file.png", "D:/Desktop/Asteroid-GameEngine/Asteroid-GameEngine/Texture");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("FileBorwser", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("FileBorwser"))
		{
			ImGuiWindowFlags window_flags =ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
			ImGui::BeginChild("ChildTop", ImVec2(0, 30), true, window_flags);
			if (ImGui::Button("Add New")) {}   ImGui::SameLine();
			if (ImGui::Button("Add Import")) {}   ImGui::SameLine();
			if (ImGui::Button("Save All")) {}   
			ImGui::EndChild();
			ImGui::BeginChild("ChildTop2", ImVec2(0, 30), true, window_flags);
			if (ImGui::Button("Left")) {}  ImGui::SameLine();
			if (ImGui::Button("Right")) {}  ImGui::SameLine();

			std::string _extension;
			for (char x : _CurrentDirectory)
				_extension += x;

			ImGui::Text(_extension.c_str());
			
			ImGui::EndChild();

			// Child 1: no border, enable horizontal scrollbar
			{
				ImGuiWindowFlags window_flags2 =0;
				//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetWindowWidth() *30/100, ImGui::GetWindowHeight()- 180), false, window_flags2);
				for (int i = 0; i < vec_List_in_Directory.size(); i++)
				{
					//ImGui::PushID(i);
					//int frame_padding = -1 + i;     // -1 = uses default padding
					if (vec_List_in_Directory[i]._filetype == _FileType::Directory)
					{
						
					
						if (ImGui::TreeNode(_bstr_t(vec_List_in_Directory[i].FileName.c_str())))
						{
							Refresh_Detail(vec_List_in_Directory[i]._path);
							/*for (const auto& entry : directory_iterator(vec_FileBrowser[i]._path))
							{
								static bool selection = false;
								
								
								if (ImGui::Selectable(entry.path().filename().string().c_str(), selection))
								{
									
								}
								if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
								{
									ImGui::SetDragDropPayload("DND_DEMO_CELL", &vec_FileBrowser[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																										// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
									ImGui::Text("Swap %s", entry.path().filename().string().c_str());
									ImGui::EndDragDropSource();
								}
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
									{
										IM_ASSERT(payload->DataSize == sizeof(AGE_FileStruct));
										AGE_FileStruct payload_n = *(AGE_FileStruct*)payload->Data;
										AGE_PRINTCONSLE(payload_n.FileName);
									}
									ImGui::EndDragDropTarget();
								}

							}*/
							ImGui::TreePop();
						}
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("DND_DEMO_CELL", &vec_List_in_Directory[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																							// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
						ImGui::Text("Swap %s", vec_List_in_Directory[i].FileName.c_str());
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
						{
							IM_ASSERT(payload->DataSize == sizeof(AGE_FileStruct));
							AGE_FileStruct payload_n = *(AGE_FileStruct*)payload->Data;
							AGE_PRINTCONSLE(_bstr_t(vec_List_in_Detail[i].FileName.c_str()));
						}
						ImGui::EndDragDropTarget();
					}
				//	ImGui::PopID();
					//ImGui::SameLine();
					
				}
				
				ImGui::EndChild();

				//ImGui::PopStyleVar();
			}

			ImGui::SameLine();
			//ImGui::Columns(3);
			// Child 2: rounded border
			{
				ImGuiWindowFlags window_flags3 = 0 | 0;
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildRight", ImVec2(0, ImGui::GetWindowHeight()-180), true, window_flags3);
				//static int line = 10;
				//ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

				ImGuiStyle& style = ImGui::GetStyle();
				ImVec2 button_sz(64, 64);
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

				for (int i = 0; i < vec_List_in_Detail.size(); i++)
				{
				//	ImGui::PushID(i);
					 // -1 = uses default padding
					ImGui::BeginGroup();

					ImGui::ImageButton((void*)(vec_List_in_Detail[i]._filetype==_FileType::Directory? DirectoryPicdata:DocPicdata) , button_sz, ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
					
					
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
				
					
		
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("pic", &vec_List_in_Detail[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																							// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
						ImGui::Text("Swap %s", vec_List_in_Detail[i].FileName.c_str());
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("pic"))
						{
							IM_ASSERT(payload->DataSize == sizeof(AGE_FileStruct));
							AGE_FileStruct payload_n = *(AGE_FileStruct*)payload->Data;
							AGE_PRINTCONSLE(_bstr_t(vec_List_in_Detail[i].FileName.c_str()));
						}
						ImGui::EndDragDropTarget();
					}

					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 64);
					ImGui::Text(_bstr_t(vec_List_in_Detail[i].FileName.c_str()), 64);
					ImGui::PopTextWrapPos();


					
					
					ImGui::EndGroup();
					if (i + 1 < vec_List_in_Detail.size() && next_button_x2 < window_visible_x2)
						ImGui::SameLine();
				//	ImGui::PopID();
					//ImGui::SameLine();
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	

	return;
}



