#pragma once
#include "entity.hpp"


struct EnemyStruct {
    int level = -1;
    float health = 0;
    float baseDamage = 0;
    float damageModifire = 0;
    float agression = 0;
    BaseEntity* target = nullptr;
};

class EnemyBase : public BaseEntity
{
public:
    EnemyBase();
    ~EnemyBase() = default;

    void init(int _level, float _health, float _baseDamage, float _damageModifire, float _agression);

    EnemyStruct getEnemyData();
    void setTarget(BaseEntity* _target);
private:
    EnemyStruct _enemyData;
};  

