#include "stdafx.h"

void(*PM_WeaponUseAmmo)(int ps, int wp, int amount);

void hk_PM_WeaponUseAmmo(int ps, int wp, int amount)
{
	if (!player_sustainAmmo->current.enabled)
		PM_WeaponUseAmmo(ps, wp, amount);
}

void PM_Weapon_Jam(int ps)
{
	// Do nothing. ESI is argument.
}

void PM_Weapon_FinishRechamber(playerState_s *ps)
{
	((void(__thiscall *)(playerState_s *))0x00765CC0)(ps);
}

int PM_WeaponAmmoAvailable(playerState_s *ps)
{
	return ((int(__cdecl *)(playerState_s *))0x005B6D90)(ps);
}

// /bgame/bg_weapons.cpp:278
void PM_StartWeaponAnim(playerState_s *ps, int anim, int leftAnim)
{
	if (ps->pm_type < 9)
	{
		if (leftAnim)
			ps->weapAnimLeft = anim | (~ps->weapAnimLeft & 0x400);
		else
			ps->weapAnim = anim | (~ps->weapAnim & 0x400);
	}
}

// /bgame/bg_weapons.cpp:290
void PM_ContinueWeaponAnim(playerState_s *ps, int anim, int leftAnim)
{
	if (((ps->weapAnim & ~0x400) != anim || ps->bRunLeftGun) &&
		((ps->weapAnimLeft & ~0x400) != anim || !ps->bRunLeftGun))
	{
		PM_StartWeaponAnim(ps, anim, leftAnim);
	}
}

// /bgame/bg_weapons.cpp:432
int *BG_GetWeaponState(playerState_s *ps, bool leftGun)
{
	if (leftGun)
		return &ps->weaponstateLeft;

	return &ps->weaponstate;
}

// /bgame/bg_weapons.cpp:440
int *BG_GetWeaponTime(playerState_s *ps, bool leftGun)
{
	if (leftGun)
		return &ps->weaponTimeLeft;

	return&ps->weaponTime;
}

// /bgame/bg_weapons.cpp:448
int *BG_GetWeaponDelay(playerState_s *ps, bool leftGun)
{
	if (leftGun)
		return &ps->weaponDelayLeft;
	
	return &ps->weaponDelay;
}

// /bgame/bg_weapons.cpp:456
unsigned int *BG_GetWeaponShotCount(playerState_s *ps, bool leftGun)
{
	if (leftGun)
		return &ps->weaponShotCountLeft;

	return &ps->weaponShotCount;
}

// /bgame/bg_weapons.cpp:1266
unsigned int PM_GetWeaponIndexForHand(playerState_s *ps)
{
	if (ps->bRunLeftGun)
		return BG_GetWeaponDef(ps->weapon)->dualWieldWeaponIndex;

	return ps->weapon;
}

// /bgame/bg_weapons.cpp:1518
bool BG_CanFastSwitch(WeaponDef *weapDef, int weaponState)
{
	if (weaponState == 1 || weaponState == 2 || weaponState == 3 || weaponState == 4 || weaponState == 5)
		return weapDef->offhandSlot != 4;

	return false;
}

// /bgame/bg_weapons.cpp:2541
bool ShotLimitReached(playerState_s *ps, WeaponDef *weapDef)
{
	ASSERT(ps);
	ASSERT(weapDef);

	unsigned int *weaponShotCount = BG_GetWeaponShotCount(ps, ps->bRunLeftGun);

	switch (weapDef->fireType)
	{
	case WEAPON_FIRETYPE_SINGLESHOT:
		if (*weaponShotCount < 1)
			return false;
		return true;

	case WEAPON_FIRETYPE_BURSTFIRE2:
		if (*weaponShotCount < 2)
			return false;
		return true;

	case WEAPON_FIRETYPE_BURSTFIRE3:
		if (*weaponShotCount < 3)
			return false;
		return true;

	case WEAPON_FIRETYPE_BURSTFIRE4:
		if (*weaponShotCount < 4)
			return false;
		return true;

	case WEAPON_FIRETYPE_STACKED:
		if (*weaponShotCount < ps->stackFireCount)
			return false;
		return true;
	}

	return false;
}

// /bgame/bg_weapons.cpp:2580
bool BurstFirePending(playerState_s *ps)
{
	static DWORD dwCall = 0x00766800;

	__asm
	{
		mov esi, ps
		call dwCall
	}
}

// /bgame/bg_weapons.cpp:2614
bool WeaponUsesBurstCooldown(unsigned int weaponIdx)
{
	if (weaponIdx)
	{
		switch (BG_GetWeaponDef(weaponIdx)->fireType)
		{
		case WEAPON_FIRETYPE_BURSTFIRE2:
		case WEAPON_FIRETYPE_BURSTFIRE3:
		case WEAPON_FIRETYPE_BURSTFIRE4:
			return true;
		}
	}

	return false;
}

// /bgame/bg_weapons.cpp:2699
int PM_Weapon_WeaponTimeAdjust(pmove_t *pm, pml_t *pml)
{
	playerState_s *ps = pm->ps;
	ASSERT(ps);

	int msec = 0;
	int *weaponState = BG_GetWeaponState(ps, ps->bRunLeftGun);
	int *weaponTime = BG_GetWeaponTime(ps, ps->bRunLeftGun);
	int *weaponDelay = BG_GetWeaponDelay(ps, ps->bRunLeftGun);
	unsigned int *weaponShotCount = BG_GetWeaponShotCount(ps, ps->bRunLeftGun);
	unsigned int weaponIndex = PM_GetWeaponIndexForHand(ps);

	WeaponVariantDef *weapVariantDef = BG_GetWeaponVariantDef(weaponIndex);
	WeaponDef *weapDef = BG_GetWeaponDef(weaponIndex);

	if (ps->weaponRestrictKickTime > 0)
		ps->weaponRestrictKickTime = max(ps->weaponRestrictKickTime - pml->msec, 0);

	// Apply perk modifications to the animation time
	if (IS_WEAPONSTATE_RELOAD(*weaponState) && BG_HasPerk(ps->perks, PERK_FASTRELOAD))
	{
		// Fast weapon reload perk
		if (perk_weapReloadMultiplier->current.value != 0.0f)
			msec = (int)((float)pml->msec / perk_weapReloadMultiplier->current.value);
		else
			msec = max(*weaponDelay, *weaponTime);
	}
	else if (IS_WEAPONSTATE_FIRE(*weaponState) && !IS_WEAPONSTATE_MELEE(*weaponState) && BG_HasPerk(ps->perks, PERK_RATEOFFIRE))
	{
		// Doubletap (rate of fire) perk
		if (perk_weapRateMultiplier->current.value != 0.0f)
			msec = (int)((float)pml->msec / perk_weapRateMultiplier->current.value);
		else
			msec = max(*weaponDelay, *weaponTime);
	}
	else if (BG_CanFastSwitch(weapDef, *weaponState) && BG_HasPerk(ps->perks, PERK_FASTSWITCH))
	{
		// Fast weapon switch perk
		ASSERT(perk_weapSwitchMultiplier->current.value > 0.0f);

		msec = (int)((float)pml->msec / perk_weapSwitchMultiplier->current.value);
	}
	else
	{
		// Default modifier time
		msec = pml->msec;
	}

	if (*weaponTime)
	{
		*weaponTime -= msec;

		if (weapDef->fireType == WEAPON_FIRETYPE_STACKED && BurstFirePending(ps))
			*weaponTime = 0;

		if (*weaponTime <= 0)
		{
			if ((*weaponState == 6 || *weaponState == 32) && WeaponUsesBurstCooldown(weaponIndex) && !BurstFirePending(ps))
			{
				float scalar = player_burstFireCooldown->current.value;

				if (scalar != 0.0f)
					*weaponTime = (int)(scalar * 1000.0f);
				else
					*weaponTime = 1;

				// Bugfix? BO2 uses ps->bRunLeftGun
				PM_ContinueWeaponAnim(ps, 0, ps->bRunLeftGun);
				*weaponState = WEAPON_READY;
				return 0;
			}

			bool pausedAfterFiring = !(ps->weapFlags & 0x400) && ShotLimitReached(ps, weapDef);
			bool holdingGrenadeBtn = (weapDef->weapType == WEAPTYPE_GRENADE || weapDef->weapType == WEAPTYPE_MINE) && weapDef->holdButtonToThrow;
			bool holdingFireBtn = false;

			if (ps->bRunLeftGun)
				holdingFireBtn = pm->cmd.buttons.testBit(0x18) != 0;
			else
				holdingFireBtn = pm->cmd.buttons.testBit(0) != 0;

			if (*weaponState >= 21 && *weaponState <= 26 || !pausedAfterFiring && !holdingGrenadeBtn)
				goto LABEL_105;
			if (!holdingFireBtn)
				goto LABEL_97;
			if (ps->weapon == pm->cmd.weapon || !PM_WeaponAmmoAvailable(ps) && !weapDef->unlimitedAmmo)
			{
			LABEL_105:
				if (holdingFireBtn && !(ps->weapFlags & 0x400))
				{
					*weaponTime = 0;
					goto LABEL_100;
				}
			LABEL_97:
				if (!BurstFirePending(ps))
					*weaponShotCount = 0;

				*weaponTime = 0;
				goto LABEL_100;
			}

			*weaponTime = 1;

			if (IS_WEAPONSTATE_RELOAD(*weaponState))
			{
				*weaponTime = 0;
				*weaponShotCount = 0;
			}
			else if (*weaponState == 8)
			{
				PM_Weapon_FinishRechamber(ps);
			}
			else if (IS_WEAPONSTATE_FIRE(*weaponState))
			{
				// Bugfix? BO2 uses ps->bRunLeftGun
				PM_ContinueWeaponAnim(ps, 0, ps->bRunLeftGun);
				*weaponState = WEAPON_READY;
			}
		}
	}

LABEL_100:
	if (!*weaponDelay)
		return 0;

	*weaponDelay -= msec;

	if (*weaponDelay > 0)
		return 0;

	*weaponDelay = 0;
	return 1;
}