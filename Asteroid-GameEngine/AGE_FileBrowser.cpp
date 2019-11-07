


#include <AGE_FileBrowser.h>
#include <string>
#include <AGE_Assert.h>




path* AGE_FileBrowser::pathToDisplay;
std::vector<AGE_FileBrowser::AGE_FileStruct> AGE_FileBrowser::vec_FileBrowser;
std::string AGE_FileBrowser::_CurrentDirectory;

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

void AGE_FileBrowser::Inited()
{
	for (const auto& entry : directory_iterator(*pathToDisplay))
	{
		AGE_FileStruct _newFile;

		_newFile.FileName = entry.path().filename().string();
		_newFile._filetype = entry.is_directory()? _FileType::Directory : DefineFileType(entry);
		_newFile._path = entry.path().string();
		vec_FileBrowser.push_back(_newFile);
	}
	return;
}

void AGE_FileBrowser::ImGUIListTheBrowser()
{
	for (const auto& browserelemt : vec_FileBrowser)
	{
		std::string _text = browserelemt.FileName;
		_text.append(" Type: ");
		_text.append(ToString(browserelemt._filetype));
		ImGui::Text(_text.c_str());   // 關於窗口的定義在Window.cpp
	}
	ImGui::Separator();
	// Child 1: no border, enable horizontal scrollbar
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | 0;
		ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);
		for (int i = 0; i < vec_FileBrowser.size(); i++)
		{
			ImGui::PushID(i);
		
			int frame_padding = -1 + i;     // -1 = uses default padding
			ImGui::TreeNode(vec_FileBrowser[i].FileName.c_str());
			

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
			ImGui::PopID();
			//ImGui::SameLine();
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	// Child 2: rounded border
	{
		ImGuiWindowFlags window_flags = 0|0;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("Child2", ImVec2(0, 260), true, window_flags);
		static int line = 10;
		ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Columns(3);
		for (int i = 0; i < vec_FileBrowser.size(); i++)
		{
			ImGui::PushID(i);
			if (i % 5 != 0)
				ImGui::SameLine();
			int frame_padding = -1 + i;     // -1 = uses default padding
			ImGui::ImageButton((void*)0, ImVec2(64, 64), ImVec2(0, 0), ImVec2(64.0f / 1, 64 / 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
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
			ImGui::PopID();
			//ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
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