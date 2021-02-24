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
	//创建僵尸
	PVZ::Zombie* CreateZombie(ZombieType::ZombieType type, int row, byte column);
	//创建植物
	PVZ::Plant* CreatePlant(PlantType::PlantType type, int row, byte column, BOOLEAN imitative = false);
	//创建子弹
	PVZ::Projectile* CreateProjectile(ProjectileType::ProjectileType type, byte row, int x);

	void AsmInit();

	//你需要先调用一次AsmInit后才能使用这个函数
	PVZ::Projectile* CreateProjectile(ProjectileType::ProjectileType type, int x, int y, float angle, float speed);
	//创建物品
	PVZ::Coin* CreateCoin(CoinType::CoinType type,int x,int y,CoinMotionType::CoinMotionType motion);
	//重建草坪
	void ResetLawnmover(PVZ* pvz);

	//只是一个底部实现
	PVZ::Griditem* CreateGriditem();
	//创建墓碑
	void CreateGrave(int row, int column);
	//创建弹坑
	PVZ::Crater* CreateCrater(int row, int column,int duration);
	//创建梯子
	PVZ::Griditem* CreateLadder(int row, byte column);
	//创建花瓶
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
	//创建钉耙
	void CreateRake(byte row, byte column);
	//创建字幕
	void CreateCaption(const char* str, int length, CaptionStyle::CaptionStyle style, int duration = 500);
	//创建图片字幕（只支持英文）
	void CreateImageCaption(const char* str, int length);
	//创建种植特效
	void CreatePlantEffect(PlantEffectType::PlantEffectType type, int x, int y);
	//创建爆炸
	void CreateExplosion(int x, int y, int radius, BOOLEAN cinder = true, byte bound = 5, BOOLEAN fromjackzombie = false);
	//创建特效
	void CreateEffect(int effectid, float x, float y);

	//创建音效，部分音效是持续的，需要用StopSound手动停止音效持续播放
	void CreateSound(int soundid);

	//冻死所有僵尸，当前关卡必须至少存在过一个植物，此函数才有效
	void FrozeAll();
	//停止音效
	void StopSound(int soundid);
	//创建我是僵尸构造
	void CreateIZombieFormation(PVZLevel::PVZLevel izlevel);
	//创建花瓶构造
	void CreateVaseFormation(PVZLevel::PVZLevel vblevel);

	//创建入口，你需要先调用一次AsmInit后才能使用这个函数
	void __CreatePortal(PVZ* pvz);

	//创建入口，你需要先调用一次AsmInit后才能使用这个函数
	void CreatePortal(PVZ* pvz,int yellow1Row, int yellow1Column, int yellow2Row, int yellow2Column, int blue1Row, int blue1Column, int blue2Row, int blue2Column);
	//清除僵尸预览
	void __ClearZombiePreview();

	//在关卡中创建僵尸，wave = 0 意味着默认和不修改
	void CreateZombieInLevel(ZombieType::ZombieType* ztypes, int length, int wave = 0);

}