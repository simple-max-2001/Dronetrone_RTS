#include "core/components/health.h"


HealthComponent::HealthComponent(double maxHealth, ArmorType armorType) :
	maxHealth_(maxHealth), armorType_(armorType)
{
	currentHealth_ = maxHealth;
	isAlive_ = true;
}

void HealthComponent::tick(double dt)
{
}

bool HealthComponent::takeDamage(DamageData damage)
{
	// If entity is already dead, do nothing
	if (!isAlive()) return false;

	double damage_ = .0;

	// Choose damage depending on armor type
	switch (armorType_)
	{
	case ArmorType::LIGHT:
		damage_ = damage.DamageToLightArmor;
		break;

	case ArmorType::MEDIUM:
		damage_ = damage.DamageToMediumArmor;
		break;

	case ArmorType::HEAVY:
		damage_ = damage.DamageToHeavyArmor;
		break;

	default:
		break;
	}

	// If there are no damage, do nothing
	if (damage_ <= 0) return true;

	currentHealth_ -= damage_;

	if (currentHealth_ <= 0)
	{
		currentHealth_ = 0;
		isAlive_ = false;
	}

	return isAlive();
}

ArmorType HealthComponent::getArmorType() const
{
	return armorType_;
}

double HealthComponent::getHealth() const
{
	return currentHealth_;
}

double HealthComponent::getMaxHealth() const
{
	return maxHealth_;
}

double HealthComponent::getPercentage() const
{
	if (!isAlive())
	{
		return .0;
	}
	else if (isFull())
	{
		return 1.;
	}
	else
	{
		return currentHealth_ / maxHealth_;
	}	
}

bool HealthComponent::isFull() const
{
	return currentHealth_ >= maxHealth_;
}

bool HealthComponent::isAlive() const
{
	return isAlive_;
}
