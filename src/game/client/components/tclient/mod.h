#ifndef GAME_CLIENT_COMPONENTS_TCLIENT_MOD_H
#define GAME_CLIENT_COMPONENTS_TCLIENT_MOD_H

#include <game/client/component.h>

#include <engine/shared/console.h>
#include <engine/shared/http.h>

class CMod : public CComponent
{
private:
	class CIden;

public:
	CIden IdenFromStr(const char *pStr, bool IsName);
	int TimeFromStr(const char *pStr, char Unit);

	void Kill(const CIden &Iden, bool Silent);
	void Kick(const CIden &Iden, const char *pReason);
	void Ban(const CIden &Iden, const char *pTime, const char *pReason);
	void Mute(const CIden &Iden, const char *pTime, const char *pReason);

	int m_ModWeaponActiveId = -1;
	float m_ModWeaponActiveTimeLeft;
	void ModWeapon(int Id);
	void OnFire(bool Pressed);

	int Sizeof() const override { return sizeof(*this); }
	void OnConsoleInit() override;
	void OnRender() override;
	void OnStateChange(int OldState, int NewState) override;
};

#endif
