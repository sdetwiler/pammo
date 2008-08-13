#include "enemyLoader.h"

namespace pammo
{
EnemyLoader::EnemyLoader()
{}

EnemyLoader::~EnemyLoader()
{}

bool EnemyLoader::parseName(char* s)
{
    int column = 1;
    s=strtok(s, ",\"");
    while(s)
    {
        switch(column)
        {
        case 1: // Name
            strcpy(mTemplate->mName, s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}

bool EnemyLoader::parseProperties(char* s)
{
    int column = 1;
    s=strtok(s, ",\"");  // strtok skips leading delimitors.
    while(s)
    {
        switch(column)
        {
        case 1: // Image Type
            if(!strcmp(s, "Single"))
                mTemplate->mImageType = Single;
            else if(!strcmp(s, "Flipbook"))
                mTemplate->mImageType = Flipbook;
            else
            {
                dprintf("Unknown appearance image type: %s", s);
                return false;
            }
            break;

        case 2: // Image Location
            strcpy(mTemplate->mImagePath, s);
            break;

        case 3: // Mass
            mTemplate->mMass = (float)atof(s);
            break;
            
        case 4: // Radius
            mTemplate->mRadius = (float)atof(s);
            break;

        case 5: // Health
            mTemplate->mHealth = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}


bool EnemyLoader::parseWeapon(char* s)
{
    int column = 1;
    s=strtok(s, ",\"");
    while(s)
    {
        switch(column)
        {
        case 1: // Weapon Type
            if(!strcmp(s, "None"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mType = None;
            else if(!strcmp(s, "Flamethrower"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mType = Flamethrower;
            else if(!strcmp(s, "MachineGun"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mType = MachineGun;
            else if(!strcmp(s, "Trebuchet"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mType = Trebuchet;
            else
            {
                dprintf("Unknown weapon type: %s", s);
                return false;
            }
            break;

        case 2: // Specific weapon data
            switch(mTemplate->mWeapons[mTemplate->mWeaponCount].mType)
            {
            case None:
                return parseWeaponNone(s);
            case Flamethrower:
                return parseWeaponFlamethrower(s);
            case MachineGun:
                return parseWeaponMachineGun(s);
            case Trebuchet:
                return parseWeaponTrebuchet(s);
            default:
                dprintf("Can't parse specific weapon data.");
                return false;
            }
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}

bool EnemyLoader::parseWeaponFlamethrower(char* s)
{
    FlamethrowerWeaponData* data = (FlamethrowerWeaponData*)mTemplate->mWeapons[mTemplate->mWeaponCount].mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Position X
            data->mPositionX = atol(s);
            break;
        case 3: // Position Y
            data->mPositionY = atol(s);
            break;
        case 4: // Accuracy
            data->mAccuracy = atol(s);
            break;
        case 5: // Damage
            data->mDamage = atol(s);
            break;
        case 6: // Rotation min
            data->mRotationMin = atol(s);
            break;
        case 7: // Rotation max
            data->mRotationMax = atol(s);
            break;
        case 8: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 9: // Spread angle
            data->mSpreadAngle= atol(s);
            ++mTemplate->mWeaponCount;
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseWeaponTrebuchet(char* s)
{
    TrebuchetWeaponData* data = (TrebuchetWeaponData*)mTemplate->mWeapons[mTemplate->mWeaponCount].mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Position X
            data->mPositionX = atol(s);
            break;
        case 3: // Position Y
            data->mPositionY = atol(s);
            break;
        case 4: // Accuracy
            data->mAccuracy = atol(s);
            break;
        case 5: // Damage
            data->mDamage = atol(s);
            break;
        case 6: // Rotation min
            data->mRotationMin = atol(s);
            break;
        case 7: // Rotation max
            data->mRotationMax = atol(s);
            break;
        case 8: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 9: // Firing rate
            data->mFireRate = atol(s);
            ++mTemplate->mWeaponCount;
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseWeaponMachineGun(char* s)
{
    MachineGunWeaponData* data = (MachineGunWeaponData*)mTemplate->mWeapons[mTemplate->mWeaponCount].mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Position X
            data->mPositionX = atol(s);
            break;
        case 3: // Position Y
            data->mPositionY = atol(s);
            break;
        case 4: // Accuracy
            data->mAccuracy = atol(s);
            break;
        case 5: // Damage
            data->mDamage = atol(s);
            break;
        case 6: // Rotation min
            data->mRotationMin = atol(s);
            break;
        case 7: // Rotation max
            data->mRotationMax = atol(s);
            break;
        case 8: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 9: // Firing rate
            data->mFireRate = atol(s);
            ++mTemplate->mWeaponCount;
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseWeaponNone(char* s)
{
    return true;
}

bool EnemyLoader::parseBehavior(char* s)
{
    int column = 1;
    s=strtok(s, ",\"");
    while(s)
    {
        switch(column)
        {
        case 1: // Behavior Type
            if(!strcmp(s, "Approach and Fire"))
                mTemplate->mBehavior.mType = ApproachAndFire;
            else if(!strcmp(s, "Surround"))
                mTemplate->mBehavior.mType = Surround;
            else if(!strcmp(s, "Drive By"))
                mTemplate->mBehavior.mType = DriveBy;
            else if(!strcmp(s, "Camp"))
                mTemplate->mBehavior.mType = Camp;
            else if(!strcmp(s, "Kamikaze"))
                mTemplate->mBehavior.mType = Kamikaze;
            else
            {
                dprintf("Unknown behavior type: %s", s);
                return false;
            }
            break;

        case 2:
            switch(mTemplate->mBehavior.mType)
            {
            case ApproachAndFire:
                return parseBehaviorApproachAndFire(s);
            case Surround:
                return parseBehaviorSurround(s);
            case DriveBy:
                return parseBehaviorDriveBy(s);
            case Camp:
                return parseBehaviorCamp(s);
            case Kamikaze:
                return parseBehaviorKamikaze(s);
            default:
                dprintf("Can't parse specific behavior data");
                return false;
            }
        }

        ++column;
        s=strtok(NULL, ",\"");
    }
    return false;
}


bool EnemyLoader::parseBehaviorApproachAndFire(char* s)
{
    ApproachAndFireBehaviorData* data = (ApproachAndFireBehaviorData*)mTemplate->mBehavior.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {

        case 2: // Speed
            data->mSpeed = (float)atof(s);
            break;

        case 3: // Target distance
            data->mDistance = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseBehaviorSurround(char* s)
{
    SurroundBehaviorData* data = (SurroundBehaviorData*)mTemplate->mBehavior.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {

        case 2: // Speed
            data->mSpeed = (float)atof(s);
            break;

        case 3: // Target radius
            data->mDistance = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseBehaviorDriveBy(char* s)
{
    DriveByBehaviorData* data = (DriveByBehaviorData*)mTemplate->mBehavior.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {

        case 2: // Speed
            data->mSpeed = (float)atof(s);
            break;

        case 3: // Target distance
            data->mDistance = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseBehaviorCamp(char* s)
{
    CampBehaviorData* data = (CampBehaviorData*)mTemplate->mBehavior.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {

        case 2: // Speed
            data->mSpeed = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseBehaviorKamikaze(char* s)
{
    KamikazeBehaviorData* data = (KamikazeBehaviorData*)mTemplate->mBehavior.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {

        case 2: // Speed
            data->mSpeed = (float)atof(s);
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

char const* sUnknown      = "Unknown";

char const* sNone         = "None";
char const* sFlamethrower = "Flamethrower";
char const* sTrebuchet    = "Trebuchet";
char const* sMachineGun   = "Machine Gun";
char const* EnemyLoader::getWeaponName(WeaponType type)
{
    switch(type)
    {
    case None:
        return sNone;
    case Flamethrower:
        return sFlamethrower;
    case Trebuchet:
        return sTrebuchet;
    case MachineGun:
        return sMachineGun;
    default:
        return sUnknown;
    }
}

char const* sApproachAndFire = "Approach and Fire";
char const* sSurround        = "Surround";
char const* sDriveBy         = "Drive By";
char const* sCamp            = "Camp";
char const* sKamikaze        = "Kamikaze";
char const* EnemyLoader::getBehaviorName(BehaviorType type)
{
    switch(type)
    {
    case ApproachAndFire:
        return sApproachAndFire;
    case Surround:
        return sSurround;
    case DriveBy:
        return sDriveBy;
    case Camp:
        return sCamp;
    case Kamikaze:
        return sKamikaze;
    default:
        return sUnknown;
    }
}

void EnemyLoader::dump()
{
    char buf[1024];

    snprintf(buf, sizeof(buf)-1,
"\
Enemy: %s\n\
  Appearance\n\
    ImageType: %s\n\
    ImagePath: %s\n\
    Mass:      %.2f\n\
    Radius:    %.2f\n\
  Behavior\n\
    Type:      %s"
,

    mTemplate->mName,
    mTemplate->mImageType==Single?"Single":"Flipbook",
    mTemplate->mImagePath,
    mTemplate->mMass,
    mTemplate->mRadius,
    getBehaviorName(mTemplate->mBehavior.mType)
    );

    dprintf("%s", buf);

    for(uint32_t i=0; i<mTemplate->mWeaponCount; ++i)
    {
        dumpWeapon(&mTemplate->mWeapons[i]);
    }
}

void EnemyLoader::dumpWeapon(EnemyWeapon* w)
{
    dprintf("  Weapon\n    Type:         %s", getWeaponName(w->mType));
    switch(w->mType)
    {
    case Flamethrower:
        dumpWeaponFlamethrower((FlamethrowerWeaponData*)w->mData);
        break;
    case Trebuchet:
        dumpWeaponTrebuchet((TrebuchetWeaponData*)w->mData);
        break;
    case MachineGun:
        dumpWeaponMachineGun((MachineGunWeaponData*)w->mData);
        break;
    }
}

void EnemyLoader::dumpWeaponFlamethrower(FlamethrowerWeaponData* d)
{
    dprintf("\
    Position X:   %d\n\
    Position Y:   %d\n\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Rotation Min: %u\n\
    Rotation Max: %u\n\
    Max Distance: %u\n\
    Spread Angle: %u",
        d->mPositionX,
        d->mPositionY,
        d->mAccuracy,
        d->mDamage,
        d->mRotationMin,
        d->mRotationMax,
        d->mMaxDistance,
        d->mSpreadAngle
    );
}

void EnemyLoader::dumpWeaponTrebuchet(TrebuchetWeaponData* d)
{
    dprintf("\
    Position X:   %d\n\
    Position Y:   %d\n\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Rotation Min: %u\n\
    Rotation Max: %u\n\
    Max Distance: %u\n\
    Fire Rate:    %u",
        d->mPositionX,
        d->mPositionY,
        d->mAccuracy,
        d->mDamage,
        d->mRotationMin,
        d->mRotationMax,
        d->mMaxDistance,
        d->mFireRate
    );
}

void EnemyLoader::dumpWeaponMachineGun(MachineGunWeaponData* d)
{
    dprintf("\
    Position X:   %d\n\
    Position Y:   %d\n\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Rotation Min: %u\n\
    Rotation Max: %u\n\
    Max Distance: %u\n\
    Fire Rate:    %u",
        d->mPositionX,
        d->mPositionY,
        d->mAccuracy,
        d->mDamage,
        d->mRotationMin,
        d->mRotationMax,
        d->mMaxDistance,
        d->mFireRate
    );
}
bool EnemyLoader::load(char const* filename, EnemyTemplate* enemyTemplate)
{
    mTemplate = enemyTemplate;
    // reset the template.
    memset(mTemplate, 0, sizeof(EnemyTemplate));

    // open file.
    FILE* f = fopen(filename, "rb");
    if(!f)
    {
        dprintf("Failed to open %s", filename);
        return false;
    }
    uint32_t fileLen;
    fseek(f, 0, SEEK_END);
    fileLen= ftell(f);
    fseek(f, 0, SEEK_SET);

    // read.
    char* buf = new char[fileLen+1];
    memset(buf, 0, fileLen+1);
    char* cur = buf;
    uint32_t numRead = 0;
    uint32_t remain = fileLen;
    while(remain)
    {
        numRead = (uint32_t)fread(cur, 1, remain, f);
        if(numRead == 0)
            return false;
        remain-=numRead;
        cur+=numRead;
    }
    fclose(f);


    int stage = 0;
    /**

    0 name
    1 properties
    2 behavior
    3 weapon (optional)
    4 weapon (optional)
    5 weapon (optional)
    6 weapon (optional)

    **/
    bool ret = true;
    bool header = true;
    cur = buf;
    char* s = buf;
    char* e;
    while(s-buf < fileLen)
    {
        e = strchr(s, '\n');
        if(!e)
            e = buf + fileLen;
        *e = NULL;

        if(header == false)
        {
            switch(stage)
            {
            case 0:
                ret = parseName(s);
                break;
            case 1:
                ret = parseProperties(s);
                break;
            case 2:
                ret = parseBehavior(s);
                break;
            case 3:
            case 4:
            case 5:
            case 6:
                ret = parseWeapon(s);
                break;
            default:
                ret = false;
                dprintf("Unknown data found");
            }

            ++stage;
        }
        if(ret == false)
        {
            dprintf("Error parsing at stage %d", stage-1);
            assert(0);
        }

        header = !header;
        s = e+1;
    }

    delete[] buf;
    
    dump();
    return true;
}


} // namespace pammo