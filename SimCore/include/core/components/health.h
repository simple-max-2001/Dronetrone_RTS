#pragma once

#include <cstdint>

#include "component.h"

/*
 Armor type of entity. There are considered three types:
   - Light armor (i.e. scout units, UAVs)
   - Medium armor (i.e. combat units)
   - Heavy armor (i.e. HQ, buildings, tanks)
 */
enum class ArmorType : uint8_t
{
	LIGHT,	// Light armor (i.e. scout units, UAVs)
	MEDIUM,	// Medium armor (i.e. combat units)
	HEAVY,	// Heavy armor (i.e. HQ, buildings, tanks)
};

/// This structure contain information about damage to entity
struct DamageData
{
	double DamageToLightArmor  = .0;	// Dealt damage if armor is light
	double DamageToMediumArmor = .0;	// Dealt damage if armor is medium
	double DamageToHeavyArmor  = .0;	// Dealt damage if armor is heavy
};

class HealthComponent : public Component
{
public:
	HealthComponent(double maxHealth = 100, ArmorType armorType = ArmorType::LIGHT);

	virtual void tick(double dt) override;

	/* Take damage
	Args:
	    damage: DamageData - taken damage
	Return:
	  is_alive: bool - If entity is still alive after damage, it is true
	 */
	bool takeDamage(DamageData damage);

	ArmorType getArmorType() const;

	double getHealth() const;

	double getMaxHealth() const;

	double getPercentage() const;

	bool isFull() const;

	bool isAlive() const;

private:
	double currentHealth_{};

	double maxHealth_{};

	bool isAlive_{};

	ArmorType armorType_{};


};
