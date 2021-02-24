#pragma once
#include "PVZ.h"

namespace Creater
{

inline short makeshort(byte b1, byte b2)
{
	return (b2 << 8) + b1;
}
inline void xytorc(int* x, int* y)
{
	int temp = *y;
	*y = (*x - 40) / 80;
	SceneType::SceneType scene = PVZ::Memory::ReadMemory<SceneType::SceneType>(PVZBASEADDRESS + 0x554C);
	bool sixroute = (scene == SceneType::Pool) || (scene == SceneType::Fog);
	*x = sixroute ? (temp - 80) / 85 : (temp - 80) / 100;
}


#define PI 3.1415926f
#define CREATEZOMBIE INVOKE_BYTE_BYTE(0x42A0F0,0,0)
#define CREATEPLANT INVOKE_DWORD_BYTE_BYTE_BYTE(0x40D120,0,0,0,-1)
#define CREATEPROJECTILE INVOKE_DWORD_DWORD_BYTE_BYTE_BYTE(0x40D620,0,0,0,0,0)
#define CREATECOIN INVOKE_DWORD_DWORD_BYTE_BYTE(0x40CB10,0,0,0,0)
#define RESETLAWNMOVER INVOKE_DWORD(0x40BC70,0)
#define CREATEGRDITEM INVOKE(0x41E1C0)
#define CREATEGRAVE INVOKE(0x426620)
#define CREATELADDER INVOKE_BYTE(0x408F40,0)
#define CREATERAKE INVOKE_DWORD_BYTE_BYTE(0x40B9C0,0,0,0)
#define UNKNOWSTRINGFUNCTION INVOKE(0x404450)
#define CREATECAPTION INVOKE(0x459010)
#define CREATEIMAGECAPTION INVOKE(0x40CA10)
#define CREATEPLANTEFFECT INVOKE_DWORD(0x4666A0,0)
#define CREATEEXPLOTION INVOKE_DWORD_BYTE_DWORD_DWORD_DWORD_BYTE_BYTE_BYTE(0x41D8A0,0,0,0,0,0,0,0,0)
#define _CREATEEFFECT INVOKE_DWORD_DWORD_DWORD_DWORD(0x518A70,0,0,0x61A80,0)
#define CREATESOUND INVOKE_BYTE(0x515020,0)
#define FORZEALL INVOKE(0x466420)
#define STOPSOUND INVOKE(0x515290)
#define CREATEIZOMBIEFORMATION INVOKE_DWORD(0x42A890,0)
#define CREATEVASEFORMATION INVOKE(0x4286F0)
#define CREATEPORTAL INVOKE(0x426FC0)
#define CLEARZOMBIEPREVIEW INVOKE(0x40DF70)
#define CREATEZOMBIEINLEVEL INVOKE(0x4092E0)
	//������ʬ
	PVZ::Zombie* CreateZombie(ZombieType::ZombieType type, int row, byte column);
	//����ֲ��
	PVZ::Plant* CreatePlant(PlantType::PlantType type, int row, byte column, BOOLEAN imitative = false);
	//�����ӵ�
	PVZ::Projectile* CreateProjectile(ProjectileType::ProjectileType type, byte row, int x);

	void AsmInit();

	//����Ҫ�ȵ���һ��AsmInit�����ʹ���������
	PVZ::Projectile* CreateProjectile(ProjectileType::ProjectileType type, int x, int y, float angle, float speed);
	//������Ʒ
	PVZ::Coin* CreateCoin(CoinType::CoinType type,int x,int y,CoinMotionType::CoinMotionType motion);
	//�ؽ���ƺ
	void ResetLawnmover(PVZ* pvz);

	//ֻ��һ���ײ�ʵ��
	PVZ::Griditem* CreateGriditem();
	//����Ĺ��
	void CreateGrave(int row, int column);
	//��������
	PVZ::Crater* CreateCrater(int row, int column,int duration);
	//��������
	PVZ::Griditem* CreateLadder(int row, byte column);
	//������ƿ
	PVZ::Vase* CreateVase(int row, int column, VaseContent::VaseContent content, VaseSkin::VaseSkin skin = VaseSkin::VaseSkinUnknow, ZombieType::ZombieType zombie = ZombieType::Zombie, PlantType::PlantType plant = PlantType::Peashooter, int sun = 0);

	struct VaseCreateInfo
	{
		int row;
		int column;
		VaseContent::VaseContent content;
		VaseSkin::VaseSkin skin;
		ZombieType::ZombieType zombie;
		PlantType::PlantType plant;
		int sun;
	};

	PVZ::Vase* CreateVase(VaseCreateInfo vaseinfo);
	//��������
	void CreateRake(byte row, byte column);
	//������Ļ
	void CreateCaption(const char* str, int length, CaptionStyle::CaptionStyle style, int duration = 500);
	//����ͼƬ��Ļ��ֻ֧��Ӣ�ģ�
	void CreateImageCaption(const char* str, int length);
	//������ֲ��Ч
	void CreatePlantEffect(PlantEffectType::PlantEffectType type, int x, int y);
	//������ը
	void CreateExplosion(int x, int y, int radius, BOOLEAN cinder = true, byte bound = 5, BOOLEAN fromjackzombie = false);
	//������Ч
	void CreateEffect(int effectid, float x, float y);

	//������Ч��������Ч�ǳ����ģ���Ҫ��StopSound�ֶ�ֹͣ��Ч��������
	void CreateSound(int soundid);

	//�������н�ʬ����ǰ�ؿ��������ٴ��ڹ�һ��ֲ��˺�������Ч
	void FrozeAll();
	//ֹͣ��Ч
	void StopSound(int soundid);
	//�������ǽ�ʬ����
	void CreateIZombieFormation(PVZLevel::PVZLevel izlevel);
	//������ƿ����
	void CreateVaseFormation(PVZLevel::PVZLevel vblevel);

	//������ڣ�����Ҫ�ȵ���һ��AsmInit�����ʹ���������
	void __CreatePortal(PVZ* pvz);

	//������ڣ�����Ҫ�ȵ���һ��AsmInit�����ʹ���������
	void CreatePortal(PVZ* pvz,int yellow1Row, int yellow1Column, int yellow2Row, int yellow2Column, int blue1Row, int blue1Column, int blue2Row, int blue2Column);
	//�����ʬԤ��
	void __ClearZombiePreview();

	//�ڹؿ��д�����ʬ��wave = 0 ��ζ��Ĭ�ϺͲ��޸�
	void CreateZombieInLevel(ZombieType::ZombieType* ztypes, int length, int wave = 0);

}