
#include <AGE_FileBrowser.h>
#include <string>
#include <AGE_Assert.h>


#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

path* AGE_FileBrowser::pathToDisplay;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_FileBrowser;
std::string AGE_FileBrowser::_CurrentDirectory;
unsigned int AGE_FileBrowser::DirectoryPicdata;



AGE_FileBrowser::_FileType DefineFileType(const directory_entry& _entry)
{
	AGE_ASSERT(_entry.exists());  // if file isn't exists or got a wrong or ghost file. 
	std::string _extension =  _entry.path().extension().string();
	
	// 或許可以用音源庫來定義說是不是音樂檔  再來做其他分類
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
	// Load current Directory
	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		AGE_FileStruct _newFile;

		_newFile.FileName = entry.path().filename().string();
		_newFile._filetype = entry.is_directory()? _FileType::Directory : DefineFileType(entry);
		_newFile._path = entry.path().string();
		AGE_FileBrowser::vec_FileBrowser.push_back(_newFile);
	}
	//Load Image
	
	DirectoryPicdata = TextureFromFiles("icon-file.png","D:/Desktop/Asteroid-GameEngine/Asteroid-GameEngine/Texture");


	return;
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
			ImGui::Text(_CurrentDirectory.c_str());
			
			ImGui::EndChild();

			// Child 1: no border, enable horizontal scrollbar
			{
				ImGuiWindowFlags window_flags2 =0;
				//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetWindowWidth() *30/100, ImGui::GetWindowHeight()- 180), false, window_flags2);
				for (int i = 0; i < vec_FileBrowser.size(); i++)
				{
					//ImGui::PushID(i);
					//int frame_padding = -1 + i;     // -1 = uses default padding
					if (vec_FileBrowser[i]._filetype == _FileType::Directory)
					{
						if (ImGui::TreeNode(vec_FileBrowser[i].FileName.c_str()))
						{
							for (const auto& entry : directory_iterator(vec_FileBrowser[i]._path))
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

							}


							ImGui::TreePop();
						}
					}
					/*else
					{
						

						if (ImGui::Selectable(vec_FileBrowser[i].FileName.c_str(), selection))
						{

						}
					}*/

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("DND_DEMO_CELL", &vec_FileBrowser[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																							// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
						ImGui::Text("Swap %s", vec_FileBrowser[i].FileName.c_str());
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
				for (int i = 0; i < vec_FileBrowser.size(); i++)
				{
				//	ImGui::PushID(i);
					 // -1 = uses default padding

					ImGui::ImageButton((void*)DirectoryPicdata, button_sz, ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (i + 1 < vec_FileBrowser.size() && next_button_x2 < window_visible_x2)
						ImGui::SameLine();
					
		
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("DND_DEMO_CELL", &vec_FileBrowser[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																							// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
						ImGui::Text("Swap %s", vec_FileBrowser[i].FileName.c_str());
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



void AGE_FileBrowser::Refresh()
{
	if (pathToDisplay != NULL) delete(pathToDisplay);
	pathToDisplay = new path(_CurrentDirectory);
	if (!pathToDisplay->is_absolute())
	{
		AGE_PRINTCONSLE("Got ghost directory path");
	}



	return;
}