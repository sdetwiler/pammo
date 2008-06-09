#ifndef __ANIMATION_ENTITY__
#define __ANIMATION_ENTITY__

#include "image.h"
#include "entity.h"

namespace pammo
{
class AnimationEntity : 
    public Entity
{
public:
    AnimationEntity();
    virtual ~AnimationEntity();

    int init(char const* path);

    virtual void draw();

    Vector2 const& getSize() const;

protected:

private:
    Image** mImages;
    int mCurrImageIndex;
    int mNumImages;
};

} // namespace pammo


#endif // __ANIMATION_ENTITY__