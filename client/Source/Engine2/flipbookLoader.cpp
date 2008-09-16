#include "flipbookLoader.h"
#include "imageLibrary.h"

namespace pammo
{

bool loadFlipbook(char const* path, Image** images, uint32_t maxImages, uint32_t* numImages)
{
    *numImages = 0;
    DIR* dir;
    dir = opendir(path);
    if(!dir)
    {
        dprintf("Failed to open flipbook directory: %s", path);
        assert(0);
        return false;
    }

    typedef std::set< std::string > StringSet;
    StringSet filenames;
    struct dirent* item;

    while((item = readdir(dir)) != NULL)
    {
        int len = strlen(item->d_name);
        if(!strcmp(&(item->d_name[len-3]), "png"))
        {
            filenames.insert(item->d_name);
        }
    }

    closedir(dir);

    int i=0;
    for(StringSet::iterator j = filenames.begin(); j!=filenames.end() && i<maxImages; ++i, ++j)
    {
        char filename[256];
        snprintf(filename, 255, "%s/%s", path, (*j).c_str());
        images[i] = gImageLibrary->reference(filename);
        if(!images[i])
        {
            dprintf("Failed to load image %s", filename);
            assert(0);
            return false;
        }

        ++(*numImages);
    }

    dprintf("Flipbook loaded %u images.", i);
    return true;
}


} // namespace pammo