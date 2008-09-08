#ifndef __SCOREMETER_H__
#define __SCOREMETER_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Image;

class ScoreMeter : public View
{
public:
    ScoreMeter();
    virtual ~ScoreMeter();

    virtual uint32_t getDrawPriority() const;
    virtual void draw();

    virtual void setScore(uint32_t score);

private:
    Image* mImages[10];
    uint32_t mScore;
};

} // namespace pammo

#endif // __SCOREMETER_H__