#ifndef __ENEMYLOADER_H__
#define __ENEMYLOADER_H__

#include "pammo.h"
#include "enemyManager.h"

namespace pammo
{

class EnemyLoader
{
public:
    EnemyLoader();
    virtual ~EnemyLoader();

    bool load(char const* filename, EnemyTemplate* enemyTemplate);

    void dump();

protected:
private:
    bool parseName(char* s);
    
    bool parseProperties(char* s);
    
    bool parseWeapon(char* s);
    bool parseWeaponNone(char* s);
    
    // Common turret type weapon parsing.
    bool parseWeaponTurret(char* s, EnemyWeaponTemplate* weaponTemplate, TurretWeaponData* data);
    
    // Turret based weapons.
    bool parseWeaponFlamethrower(char* s);
    bool parseWeaponTrebuchet(char* s);
    bool parseWeaponMachineGun(char* s);
    bool parseWeaponSelfDestruct(char* s);
    bool parseWeaponHeatSeaker(char* s);

    void dumpWeapon(EnemyWeaponTemplate* w);
    void dumpWeaponTurret(TurretWeaponData* d);
    void dumpWeaponFlamethrower(FlamethrowerWeaponData* d);
    void dumpWeaponTrebuchet(TrebuchetWeaponData* d);
    void dumpWeaponMachineGun(MachineGunWeaponData* d);
    void dumpWeaponSelfDestruct(SelfDestructWeaponData* d);
    void dumpWeaponHeatSeaker(HeatSeakerWeaponData* d);

    bool parseBehavior(char* s);
    bool parseBehaviorApproachAndFire(char* s);
    bool parseBehaviorSurround(char* s);
    bool parseBehaviorDriveBy(char* s);
    bool parseBehaviorCamp(char* s);
    bool parseBehaviorKamikaze(char* s);

    bool parseParticle(char* s);
    bool parseParticleNone(char* s);
    bool parseParticleSmoke(char* s);
    bool parseParticleJetFlame(char* s);

    void dumpParticle(EnemyParticleTemplate* p);
    void dumpParticleSmoke(SmokeParticleData* d);
    void dumpParticleJetFlame(JetFlameParticleData* d);

    char const* getWeaponName(WeaponType type);
    char const* getBehaviorName(BehaviorType type);
    char const* getParticleName(EnemyParticleType type);

    EnemyTemplate* mTemplate;
};
/**
"Enemy","Name",,,,,,,,,,,,
,"flameTank",,,,,,,,,,,,
"Properties","Image Type","Image Location","Mass","Radius","Health",,,,,,,,
,"Single","vehicles/flameTank/00.png",50,20,100,,,,,,,,
"Behavior","Type","Speed","Target Distance","","","","","","","","","",""
,"Approach and Fire",50,150,,,,,,,,,,
"Weapon","Type","Position X","Position Y","Accuracy","Damage","Rotation Min","Rotation Max","Distance Max","Spread Angle","","","",""
,"Flamethrower",0,0,80,10,0,360,150,15,,,,
"Weapon","Type","","","","","","","","","","","",""
,"None",,,,,,,,,,,,
"Weapon","Type","","","","","","","","","","","",""
,"None",,,,,,,,,,,,
"Weapon","Type","","","","","","","","","","","",""
,"None",,,,,,,,,,,,
**/

} // namespace pammo

#endif // __ENEMYLOADER_H__