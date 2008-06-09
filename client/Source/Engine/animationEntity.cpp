#include "animationEntity.h"
#include "imageLibrary.h"

namespace pammo
{

AnimationEntity::AnimationEntity()
{
    mNumImages = 0;
    mImages = 0;
}

AnimationEntity::~AnimationEntity()
{
}

Vector2 const& AnimationEntity::getSize() const
{
    return mImages[0]->mSize;
}

int AnimationEntity::init(char const* path)
{
    DIR* dir;
    dir = opendir(path);
    if(!dir)
    {
        dprintf("Failed to open %s", path);
        return -1;
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

    mImages = new Image*[filenames.size()];
    int i=0;
    for(StringSet::iterator j = filenames.begin(); j!=filenames.end(); ++i, ++j)
    {
      
        char filename[256];
        snprintf(filename, 255, "%s/%s", path, (*j).c_str());
        mImages[i] = gImageLibrary->reference(filename);
        if(!mImages[i])
        {
            return -1;
        }

        ++mNumImages;
    }

    mCurrImageIndex = 0;
//    mSize = mImages[0]->mSize;

    return 0;
}

void AnimationEntity::draw()
{
    mCurrImageIndex = (mCurrImageIndex+1)%mNumImages;

    drawImage(mImages[mCurrImageIndex], getTransform(), 1.0f);
}


} // namespace pammo