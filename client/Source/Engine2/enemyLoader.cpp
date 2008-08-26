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
                mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType = None;
            else if(!strcmp(s, "Flamethrower"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType = Flamethrower;
            else if(!strcmp(s, "MachineGun"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType = MachineGun;
            else if(!strcmp(s, "Trebuchet"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType = Trebuchet;
            else if(!strcmp(s, "SelfDestruct"))
                mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType = SelfDestruct;
            else
            {
                dprintf("Unknown weapon type: %s", s);
                return false;
            }
            break;

        case 2: // Specific weapon data
            switch(mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mType)
            {
            case None:
                return parseWeaponNone(s);
            case Flamethrower:
                return parseWeaponFlamethrower(s);
            case MachineGun:
                return parseWeaponMachineGun(s);
            case Trebuchet:
                return parseWeaponTrebuchet(s);
            case SelfDestruct:
                return parseWeaponSelfDestruct(s);
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
    FlamethrowerWeaponData* data = (FlamethrowerWeaponData*)mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2:
            if(!parseWeaponTurret(s, &mTemplate->mWeapons[mTemplate->mWeaponCount], &data->mTurret))
                return false;
            column = 8;
            break;
        case 9: // Accuracy
            data->mAccuracy = atol(s);
            break;
        case 10: // Damage
            data->mDamage = atol(s);
            break;
        case 11: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 12: // Spread angle
            data->mSpreadAngle= atol(s);
            break;
        case 13: // Firing rate
            data->mFireRate = (float)atof(s);
            data->mFiring = false;
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
    TrebuchetWeaponData* data = (TrebuchetWeaponData*)mTemplate->mWeapons[mTemplate->mWeaponCount].mWeapon.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Image path
            strcpy(mTemplate->mWeapons[mTemplate->mWeaponCount].mImagePath, s);
            break;
        case 3:
            data->mAccuracy = atol(s);
            break;
        case 4: // Damage
            data->mDamage = atol(s);
            break;
        case 5: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 6: // Firing rate
            data->mFireRate = (float)atof(s);
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
    EnemyWeaponTemplate* weaponTemplate = &mTemplate->mWeapons[mTemplate->mWeaponCount];
    MachineGunWeaponData* data = (MachineGunWeaponData*)weaponTemplate->mWeapon.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2:
            if(!parseWeaponTurret(s, &mTemplate->mWeapons[mTemplate->mWeaponCount], &data->mTurret))
                return false;
            column = 8;
            break;
        case 9: // Accuracy
            data->mAccuracy = atol(s);
            break;
        case 10: // Damage
            data->mDamage = atol(s);
            break;
        case 11: // Max distance
            data->mMaxDistance = atol(s);
            break;
        case 12: // Firing rate
            data->mFireRate = atol(s);
            ++mTemplate->mWeaponCount;
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}


bool EnemyLoader::parseWeaponTurret(char* s, EnemyWeaponTemplate* weaponTemplate, TurretWeaponData* data)
{
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Image path
            strcpy(weaponTemplate->mImagePath, s);
            break;
        case 3: // Position X
            data->mPosition.x = (float)atol(s);
            break;
        case 4: // Position y
            data->mPosition.y = (float)atol(s);
            break;
        case 5: // Fire Position X
            data->mFirePosition.x = (float)atol(s);
            break;
        case 6: // Fire Position y
            data->mFirePosition.y = (float)atol(s);
            break;
        case 7: // Rotation min
            data->mRotationMin = (float)atof(s) * 0.0174532925f;
            break;
        case 8: // Rotation max
            data->mRotationMax = (float)atof(s) * 0.0174532925f;
            return true;
        }

        ++column;
        s=strtok(NULL, ",\"");
    }

    return false;
}

bool EnemyLoader::parseWeaponSelfDestruct(char* s)
{
    EnemyWeaponTemplate* weaponTemplate = &mTemplate->mWeapons[mTemplate->mWeaponCount];
    SelfDestructWeaponData* data = (SelfDestructWeaponData*)weaponTemplate->mWeapon.mData;
    int column = 2;
    while(s)
    {
        switch(column)
        {
        case 2: // Image path
            strcpy(weaponTemplate->mImagePath, s);
            break;
        case 3: // Damage
            data->mDamage = atol(s);
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
char const* sSelfDestruct = "Self Destruct";
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
    case SelfDestruct:
        return sSelfDestruct;
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

void EnemyLoader::dumpWeapon(EnemyWeaponTemplate* w)
{
    dprintf("  Weapon\n    Type:         %s", getWeaponName(w->mWeapon.mType));
    
    dprintf("\
    Image File:   %s",
    w->mImagePath);

    switch(w->mWeapon.mType)
    {
    case Flamethrower:
        dumpWeaponFlamethrower((FlamethrowerWeaponData*)w->mWeapon.mData);
        break;
    case Trebuchet:
        dumpWeaponTrebuchet((TrebuchetWeaponData*)w->mWeapon.mData);
        break;
    case MachineGun:
        dumpWeaponMachineGun((MachineGunWeaponData*)w->mWeapon.mData);
        break;
    case SelfDestruct:
        dumpWeaponSelfDestruct((SelfDestructWeaponData*)w->mWeapon.mData);
        break;
    }
}

void EnemyLoader::dumpWeaponTurret(TurretWeaponData* d)
{
    dprintf("\
    Position X:   %.2f\n\
    Position Y:   %.2f\n\
    Fire Pos X:   %.2f\n\
    Fire Pos Y:   %.2f\n\
    Rotation Min: %.2f\n\
    Rotation Max: %.2f",
        d->mPosition.x,
        d->mPosition.y,
        d->mFirePosition.x,
        d->mFirePosition.y,
        d->mRotationMin * 57.2957795f,
        d->mRotationMax * 57.2957795f
    );
}

void EnemyLoader::dumpWeaponFlamethrower(FlamethrowerWeaponData* d)
{
    dumpWeaponTurret(&d->mTurret);
    dprintf("\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Max Distance: %u\n\
    Spread Angle: %u",
        d->mAccuracy,
        d->mDamage,
        d->mMaxDistance,
        d->mSpreadAngle
    );
}

void EnemyLoader::dumpWeaponTrebuchet(TrebuchetWeaponData* d)
{
    dumpWeaponTurret(&d->mTurret);
    dprintf("\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Max Distance: %u\n\
    Fire Rate:    %u",
        d->mAccuracy,
        d->mDamage,
        d->mMaxDistance,
        d->mFireRate
    );
}

void EnemyLoader::dumpWeaponMachineGun(MachineGunWeaponData* d)
{
    dumpWeaponTurret(&d->mTurret);
    dprintf("\
    Accuracy:     %u\n\
    Damage:       %u\n\
    Max Distance: %u\n\
    Fire Rate:    %u",
        d->mAccuracy,
        d->mDamage,
        d->mMaxDistance,
        d->mFireRate
    );
}

void EnemyLoader::dumpWeaponSelfDestruct(SelfDestructWeaponData* d)
{
    dprintf("\
    Damage:       %u\n",
        d->mDamage
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