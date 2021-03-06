
#include <AGE_FileBrowser.hpp>
#include <string>
#include <AGE_Assert.hpp>
#include <AGE_FileSystem.hpp>
#include <codecvt>
#include <Window.hpp>


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

extern unsigned int Window_Width;
extern unsigned int Window_Height;


path* AGE_FileBrowser::pathToDisplay;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_List_in_Directory;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_List_in_Detail;
std::wstring AGE_FileBrowser::_CurrentDirectory_wstring;
std::wstring AGE_FileBrowser::_ProjectDirectory_wstring;
std::string AGE_FileBrowser::_CurrentDirectory_string;
unsigned int AGE_FileBrowser::DirectoryPicdata;
unsigned int AGE_FileBrowser::DocPicdata;
AGE_FileBrowser::AGE_FileStruct* AGE_FileBrowser::Dir_currentSelect, *AGE_FileBrowser::Dir_ProjectBase;
AGE_FileBrowser::AGE_FileStruct* AGE_FileBrowser::PreparedForCopy = NULL;
static bool enable_drag = true, draging = false;

int currentIndex = 0;

std::wstring stringToWstring(const std::string& t_str)
{
	//setup converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return converter.from_bytes(t_str);
}
AGE_FileBrowser::_FileType AGE_FileBrowser::DefineFileType(const directory_entry& _entry)
{
	AGE_ASSERT(_entry.exists());  // if file isn't exists or got a wrong or ghost file. 
	
	std::string _extension(WstringToString(_entry.path().extension().wstring()));

	if (!_extension.compare(".AstGamEng")) // if equal will return 0, otherwise 1
	{
		return AGE_FileBrowser::_FileType::Scene;
	}
	else if (!_extension.compare(".jpg") || !_extension.compare(".png") || !_extension.compare(".gif"))
	{
		return AGE_FileBrowser::_FileType::Image;
	}
	else if (!_extension.compare(".mp3") || !_extension.compare(".mp4"))
	{
		return AGE_FileBrowser::_FileType::Audio;
	}
	else if (!_extension.compare(".fbx") || !_extension.compare(".obj") || !_extension.compare(".dae"))
	{
		return AGE_FileBrowser::_FileType::Model;
	}
	else
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

	if (!AGE_LIB::FileSystem::exists(filename.c_str()))
	{
		AGE_PRINTCONSLE("Can't find"+ filename);
		return 0;
	}

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

	if (_CurrentDirectory_wstring.empty()) _CurrentDirectory_wstring = stringToWstring("./");

	//Load Image
	DirectoryPicdata = TextureFromFiles("icon-dir.png","Texture");
	DocPicdata = TextureFromFiles("icon-doc.png", "Texture");

	if (pathToDisplay != NULL) delete(pathToDisplay);
	pathToDisplay = new path(_CurrentDirectory_wstring);
	if (!AGE_LIB::FileSystem::exists(*pathToDisplay))
		AGE_PRINTCONSLE("Got ghost directory path");

	if (!AGE_FileBrowser::vec_List_in_Detail.empty()) AGE_FileBrowser::vec_List_in_Detail.clear();
	if (!AGE_FileBrowser::vec_List_in_Directory.empty()) AGE_FileBrowser::vec_List_in_Directory.clear();
	// Load  Directory first

	Dir_currentSelect = new AGE_FileStruct();

	//_newFile.beenRefresh = true;
	Dir_currentSelect->_FileName_wstring = pathToDisplay->filename().wstring();
	Dir_currentSelect->_index = currentIndex;
	Dir_currentSelect->_FileName_string = WstringToString(Dir_currentSelect->_FileName_wstring);
	Dir_currentSelect->_filetype = _FileType::Directory;
	Dir_currentSelect->_path = AGE_LIB::FileSystem::absolute(*pathToDisplay);
	Dir_currentSelect->_filesBelow = Refresh_below(_ProjectDirectory_wstring);
	Dir_ProjectBase = Dir_currentSelect;
	

	return;
}

std::vector<AGE_FileBrowser::AGE_FileStruct*> AGE_FileBrowser::Refresh_below(std::wstring _NewPath) //第二層的讀取
{
	std::vector<AGE_FileStruct*> _result;
	path _pa(_NewPath);

	for (const auto& entry : directory_iterator(_pa))
	{
		AGE_ASSERT(entry.exists());
		if (!entry.is_directory()) continue;
		AGE_FileStruct* _newFile = new AGE_FileStruct();
		_newFile->_FileName_wstring = entry.path().filename().wstring();

		_newFile->_FileName_string = WstringToString(_newFile->_FileName_wstring);
		_newFile->_filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
		_newFile->_path = entry.path().wstring();
		_result.push_back(_newFile);
	}

	for (const auto& entry : directory_iterator(_pa))
	{
		AGE_ASSERT(entry.exists());
		if (entry.is_directory()) continue;
		AGE_FileStruct* _newFile = new AGE_FileStruct();
		_newFile->_FileName_wstring = entry.path().filename().wstring();

		_newFile->_FileName_string = WstringToString(_newFile->_FileName_wstring);
		_newFile->_filetype = entry.is_directory() ? _FileType::Directory : DefineFileType(entry);
		_newFile->_path = AGE_LIB::FileSystem::absolute(entry.path());
		_result.push_back(_newFile);
	}


	//避免繼續往下一個資料夾下去讀取
	return _result;
}
void AGE_FileBrowser::Refresh(AGE_FileStruct* _filestruct) // Only Directory can call this
{
	_filestruct->beenRefresh = true;
	AGE_ASSERT(_filestruct->_filetype == _FileType::Directory); // if it trigger means something really wrong
	
	if (!_filestruct->_filesBelow.empty())_filestruct->_filesBelow.clear();
	_filestruct->_filesBelow = Refresh_below(_filestruct->_path);
}


void AGE_FileBrowser::IfitIsaDirectory(AGE_FileStruct* _fst)
{
	
	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.

		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (selection_mask & (1 << _fst->_index))
		node_flags |= ImGuiTreeNodeFlags_Selected;
	 
	if (_fst->_filetype == _FileType::Directory)
	{
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::Image((void*)(uintptr_t)DirectoryPicdata, ImVec2(10, 10));
		ImGui::SameLine(0.0f, spacing);
	}
	// Items 0..2 are Tree Node
	bool node_open = (_fst->_filetype == _FileType::Directory)? ImGui::TreeNodeEx( _fst->_FileName_string.c_str(), node_flags) : ImGui::Selectable(_fst->_FileName_string.c_str());
	if (ImGui::BeginPopupContextItem(_fst->_FileName_string.c_str()))
	{
		ImGui::MenuItem("Copy", "Ctrl+C");
		ImGui::MenuItem("Paste", "Ctrl+V");
		ImGui::MenuItem("Delete", "delete");
		ImGui::MenuItem("Cancel");
		ImGui::MenuItem("Information");
		ImGui::EndPopup();
	}
	if (ImGui::IsItemClicked())
	{
		_CurrentDirectory_wstring = _fst->_FileName_wstring;
		_CurrentDirectory_string = _fst->_FileName_string;
		node_clicked = _fst->_index;

		if(_fst->_filetype==_FileType::Directory)Dir_currentSelect = _fst;
	}
		
	
	
	if (node_open)
	{
		if (_fst->_filetype == _FileType::Directory && !_fst->beenRefresh)
			Refresh(_fst);
		
		for (auto& _element : _fst->_filesBelow)
		{
			if (_element->_filetype == _FileType::Directory)
			{
				IfitIsaDirectory(_element);
			}
			else
			{
				static bool _enable = false;
				if (ImGui::Selectable(_element->_FileName_string.c_str()), _enable)
				{
					
				}
			}
		}
		if(_fst->_filetype == _FileType::Directory)
			ImGui::TreePop();
	}	
	
	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
		{
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		}	
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
		{
			selection_mask = (1 << node_clicked);           // Click to single-select
		}
	}
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("TreenodeDragDrop", _fst, sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																			// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
		ImGui::Text("Swap %s", _fst->_FileName_string.c_str());
		draging = true;
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TreenodeDragDrop"))
		{
			IM_ASSERT(payload->DataSize == sizeof(AGE_FileStruct));
			AGE_FileStruct payload_n = *(AGE_FileStruct*)payload->Data;
			AGE_PRINTCONSLE(_fst->_FileName_string.c_str());
		}
		draging = false;
		ImGui::EndDragDropTarget();
	}
	ImGui::PopStyleVar();
	



}


//#define FILESYSTEMDONOTHING




void AGE_FileBrowser::ImGUIListTheBrowser()
{
	static bool popevent_rightclick = false;
	static bool popevent_delete = false;
	//static bool earlyout = false;
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
			float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			if (ImGui::ArrowButton("##left", ImGuiDir_Left)) 
			{
				path _cache(Dir_currentSelect->_path);
				Dir_currentSelect->_path = _cache.parent_path().wstring();
				Dir_currentSelect->beenRefresh = false;
			}	ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { }	ImGui::SameLine(0.0f, spacing);
			
            
			ImGui::Text(WstringToString(Dir_currentSelect->_path).c_str());
			
			ImGui::EndChild();

			// Child 1: no border, enable horizontal scrollbar
			{
				ImGuiWindowFlags window_flags2 =0;
				//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetWindowWidth() *30/100, ImGui::GetWindowHeight()- 180), false, window_flags2);
				for (int i = 0; i < Dir_ProjectBase->_filesBelow.size(); i++)
				{
					IfitIsaDirectory(Dir_ProjectBase->_filesBelow[i]);							
				}
				
				ImGui::EndChild();
			}

			ImGui::SameLine();

			// Child 2: rounded border
			{
				ImGuiWindowFlags window_flags3 = 0 | 0;
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildRight", ImVec2(0, ImGui::GetWindowHeight()-180), true, window_flags3);
				//static int line = 10;
				//ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

				if (!Dir_currentSelect->beenRefresh) 
					Refresh(Dir_currentSelect);

				ImGuiStyle& style = ImGui::GetStyle();
				ImVec2 button_sz(64, 64);
				float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

				for (int i = 0; i < Dir_currentSelect->_filesBelow.size(); i++)
				{
					ImGui::PushID(i);
					 // -1 = uses default padding
					ImGui::BeginGroup();
					
					if (ImGui::ImageButton((void*)(Dir_currentSelect->_filesBelow[i]->_filetype == _FileType::Directory ? (uintptr_t)DirectoryPicdata : (uintptr_t)DocPicdata), button_sz, ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
					{
						switch (Dir_currentSelect->_filesBelow[i]->_filetype)
						{
						case _FileType::None:
							break;
						case _FileType::Directory: 
							/*Dir_currentSelect = Dir_currentSelect->_filesBelow[i];
							Dir_currentSelect->beenRefresh = false;
							
							//early out
							ImGui::EndGroup();
							ImGui::PopID();
							ImGui::EndChild(); 
							ImGui::PopStyleVar();
							ImGui::EndTabItem();
							ImGui::EndTabBar();
							return;
							*/

							break;
						case _FileType::Scene:
							break;
						case _FileType::Image:
							break;
						case _FileType::Audio:
							break;
						case _FileType::Model: 
							break;
						case _FileType::unKnown:
							break;
						default:
							AGE_ASSERT(0); //Should never goes here
							break;
						}
					}
					/*if (ImGui::Button(Dir_currentSelect->_filesBelow[i]->_FileName_string.c_str(), button_sz))
					{
						AGE_PRINTCONSLE("Belongs to: " + Dir_currentSelect->_filesBelow[i]->_FileName_string);
					}*/
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
					{
						ImGui::SetDragDropPayload("PictureIcon", Dir_currentSelect->_filesBelow[i], sizeof(AGE_FileStruct));        // Set payload to carry the index of our item (could be anything)
																							// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
						ImGui::Text("Swap %s", Dir_currentSelect->_filesBelow[i]->_FileName_string.c_str());
						draging = true;
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PictureIcon"))    // 指被拖起來的那個物件
						{
							IM_ASSERT(payload->DataSize == sizeof(AGE_FileStruct));
							AGE_FileStruct payload_n = *(AGE_FileStruct*)payload->Data;
							AGE_PRINTCONSLE(Dir_currentSelect->_filesBelow[i]->_FileName_string.c_str());
							AGE_LIB::FileSystem::Move(payload_n._path, Dir_currentSelect->_filesBelow[i]->_path);
						}
						draging = false;
						ImGui::EndDragDropTarget();
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip(Dir_currentSelect->_filesBelow[i]->GetFileType().c_str());
						
					}

					if (ImGui::BeginPopupContextItem("icon menu"))
					{
						popevent_rightclick = true;
						if (ImGui::MenuItem("Copy", "Ctrl+C")) { PreparedForCopy = Dir_currentSelect->_filesBelow[i]; };
						if (PreparedForCopy != NULL)
						{
							if (ImGui::MenuItem("Paste", "Ctrl+V"))
							{
								AGE_LIB::FileSystem::Copy(PreparedForCopy->_path);
								PreparedForCopy = NULL;
							}
						}
						else
						{
							if (ImGui::MenuItem("Paste", "Ctrl+V",false,false))
							{
								AGE_ASSERT(0);
							}
						}
						if (ImGui::Selectable("Delete")) popevent_delete = true;
							
						ImGui::Selectable("Cancel");
						ImGui::EndPopup();
					}

					if (popevent_delete)
					{
						ImGui::OpenPopup("###Delete?");
					}
					if (ImGui::BeginPopupModal("###Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))         
					{
						popevent_delete = false;
						ImGui::Text("It can't be redo");
						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(120, 0)))
						{
							AGE_LIB::FileSystem::Remove(Dir_currentSelect->_filesBelow[i]->_path);
							ImGui::CloseCurrentPopup();
							popevent_delete = false;
							Refresh(Dir_currentSelect);
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  popevent_delete = false; }
						ImGui::EndPopup();
					}
				
					
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 64);
					if(i < Dir_currentSelect->_filesBelow.size())
						ImGui::Text(Dir_currentSelect->_filesBelow[i]->_FileName_string.c_str(), 64);
					ImGui::PopTextWrapPos();
					
					ImGui::EndGroup();

					if (i + 1 < Dir_currentSelect->_filesBelow.size() && next_button_x2 < window_visible_x2)
						ImGui::SameLine();
					ImGui::PopID();
					//ImGui::SameLine();


				
				}
				
				ImGui::EndChild();

				ImGui::PopStyleVar();


				

				if (!popevent_rightclick && ImGui::BeginPopupContextItem("empty menu") )
				{
				
					if (ImGui::BeginMenu("Add", "add")) 
					{

						ImGui::EndMenu(); 
					}
					/*if (ImGui::MenuItem("Copy", "Ctrl+C")) { PreparedForCopy = Dir_currentSelect->_filesBelow[i]; };*/
					if (PreparedForCopy != NULL)
					{
						if (ImGui::MenuItem("Paste", "Ctrl+V"))
						{
							
							AGE_LIB::FileSystem::Copy(PreparedForCopy->_path);
							PreparedForCopy = NULL;
							Refresh(Dir_currentSelect);
						}
					}
					else
					{
						if (ImGui::MenuItem("Paste", "Ctrl+V", false, false))
						{
							AGE_ASSERT(0);
						}
					}
					
					if (ImGui::MenuItem("Refresh", "F5")) 
					{
						Refresh(Dir_currentSelect);
					}
					ImGui::MenuItem("Cancel");
					ImGui::EndPopup();

				}
				popevent_rightclick = false;


			


			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	//Main Background--------------------------------------------------------------------------------------
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(Window_Width, Window_Height), ImGuiCond_Once);
	{
		if (!ImGui::Begin("Main Background", &enable_drag, draging? (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_AlwaysUseWindowPadding): (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_AlwaysUseWindowPadding|ImGuiWindowFlags_NoInputs)))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		/*if (ImGui::ImageButton((void*)0, ImVec2(100, 100),ImVec2(0, 0), ImVec2(1, 1),1,ImVec4(0,0,0,0), ImVec4(0, 0, 0, 0)))
		{

		}*/
		ImGui::Image((void*)0, ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(0, 0, 0, 0));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PictureIcon"))
			{
				IM_ASSERT(payload->DataSize == sizeof(AGE_FileBrowser::AGE_FileStruct));
				AGE_FileBrowser::AGE_FileStruct payload_n = *(AGE_FileBrowser::AGE_FileStruct*)payload->Data;
				AGE_PRINTCONSLE(payload_n._FileName_string.c_str());
				switch (payload_n._filetype)
				{
				case _FileType::None: 
					break;
				case _FileType::Directory:
					break;
				case _FileType::Scene:
					break;
				case _FileType::Image:
					break;
				case _FileType::Audio:
					break;
				case _FileType::Model: Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(NULL,WstringToString(payload_n._path));
					break;
				case _FileType::unKnown:
					break;
				default:
					AGE_ASSERT(0); //Should never goes here
					break;
				}
				
			}
			draging = false;
			ImGui::EndDragDropTarget();
		}

	
		ImGui::End();

	}





	

	return;
}



