#include <AGE_FileBrowser.hpp>


#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif // !STB_IMAGE_WRITE_IMPLEMENTATION


namespace AGE_LIB_PRIVATE
{
	namespace FileSystem
	{
		std::string screencapture()
		{
			int g_gl_width = 1920;
			int g_gl_height = 1080;
			unsigned char* buffer = (unsigned char*)malloc(g_gl_width * g_gl_height * 3);
			glReadPixels(0, 0, g_gl_width, g_gl_height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			char name[1024];
			long int t = time(NULL);
			sprintf(name, "screenshot_%ld.png", t);
			unsigned char* last_row = buffer + (g_gl_width * 3 * (g_gl_height - 1));
			if (!stbi_write_png(name, g_gl_width, g_gl_height, 3, last_row, -3 * g_gl_width)) {
				fprintf(stderr, "ERROR: could not write screenshot file %s\n", name);
			}
			free(buffer);
			return std::string(name);
		}
	}
}



