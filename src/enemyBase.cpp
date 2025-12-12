#include "../include/entity/enemyBase.hpp"

EnemyBase::EnemyBase()
{
	log("EnemyBase constructor");
}

void EnemyBase::init(int _level, float _health, float _baseDamage, float _damageModifire, float _agression)
{
	_enemyData = {_level, _health, _baseDamage, _damageModifire, _agression, nullptr};
}

EnemyStruct EnemyBase::getEnemyData()
{
	return _enemyData;
}

void EnemyBase::setTarget(BaseEntity* _target)
{
	_enemyData.target = _target;
}
