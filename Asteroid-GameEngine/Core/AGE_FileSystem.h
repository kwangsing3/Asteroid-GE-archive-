#ifndef AGE_FILESYSTEM
#define AGE_FILESYSTEM

#include <iostream>
#include <filesystem>


namespace AGE_LIB
{
	namespace FileSystem 
	{


		static void Move(std::filesystem::path  _from, std::filesystem::path _to) 
		{
#ifndef FILESYSTEMDONOTHING
			path entry(_from);
			path dest(_to.string() + "\\" + entry.filename().string());

			try
			{
				std::filesystem::copy_file(entry, dest, std::filesystem::copy_options::overwrite_existing);
				std::filesystem::remove(entry);
			}
			catch (const std::filesystem::filesystem_error & e)
			{
				AGE_PRINTCONSLE(e.what());
			}
#endif // FILESYSTEMDONOTHING
		}
		static void Move(std::string _fromPath, std::string _toPath)
		{
			path _from(_fromPath); path _to(_toPath);
			Move(_from, _to);
		}
		static void Move(std::wstring _fromPath, std::wstring _toPath)
		{
			path _from(_fromPath); path _to(_toPath);
			Move(_from, _to);
		}
		static void Copy(path _input)
		{
#ifndef FILESYSTEMDONOTHING
			path _in(_input);
			if (_in.empty())
			{
				AGE_PRINTCONSLE("Invaild file");
				return;
			}
			try
			{
				unsigned int i = 1;
				while (i++)
				{
					path _test(_input.parent_path().string()+"/"+ _input.stem().string() +"_"+ std::to_string(i).append(_input.extension().string()) );
					

					if (!std::filesystem::exists(_test))
					{

						std::filesystem::copy(_input, _test);
						break;
					}
						
					
					AGE_ASSERT(!(i - 1 > INTMAX_MAX));
				}

				
			}
			catch (const std::filesystem::filesystem_error & e)
			{
				AGE_PRINTCONSLE(e.what());
			}

#endif // FILESYSTEMDONOTHING		
		}

		static void Copy(std::string _input)
		{
			path _new(_input);
			Copy(_new);
		}
		
		static void Rename(std::filesystem::path  _from, std::string _name)
		{
#ifndef FILESYSTEMDONOTHING
			std::filesystem::rename(_from,_name);
#endif // FILESYSTEMDONOTHING	
		}
		static void Remove(std::filesystem::path  _input)
		{
#ifndef FILESYSTEMDONOTHING
			std::filesystem::remove(_input);
#endif // FILESYSTEMDONOTHING	
		}



	}



}



#endif // !AGE_FILESYSTEM
