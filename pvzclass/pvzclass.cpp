#include "pvzclass.h"
#include <iostream>
#include "events.h"
#include <ctime>
#include <sys/timeb.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
using namespace std;
//声明变量
int waittime;
////////////////////
int papertype;
int peazombietype;
////////////////////
HWND PVZhWnd;
DWORD pid;
PVZ* pvz;
PVZ::Mouse* mouse;
PVZ::CardSlot* seedcard;
PVZ::Memory* memory;
HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);//主窗口句柄
//声明变量

DWORD WINAPI peazombirthread(LPVOID)
{
	int row;
	srand(time(NULL));
	struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);  // milli time
	row = rand() % 5;
	if (peazombietype == 5)
	{
		Zombie* zombie = Creater::CreateZombie(ZombieType::PeashooterZombie, row, 12);
		zombie->Temp = 66988;
		while (zombie->NotDying == true)
		{
			if (pvz->GamePaused != true)
			{
				//zombie->GarlicBited = true;
				Projectile* pro = Creater::CreateProjectile(ProjectileType::ZombiePea, zombie->Row, zombie->X);
				pro->Motion = MotionType::Left;
				zombie->X = zombie->X - 20;
				Sleep(300);
			}
		}
	}
	return 0;
}

DWORD WINAPI propeazombiethread(LPVOID)
{
	Event* e;
	srand(time(NULL));
	struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);  // milli time
	peazombietype = rand() % 10 + 1;
	cout << peazombietype << endl;
	if (peazombietype == 5)
	{
		memory->WriteMemory<int>(0x65109A, 1);
		CreateThread(NULL, 0, &peazombirthread, NULL, 0, NULL);
		Sleep(30);
		memory->WriteMemory<int>(0x65109A, 0);
	}
	return 0;
}

BOOL WINAPI OnExit(DWORD)
{
	if (MessageBoxA(NULL, "您要一起关闭游戏吗？", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		system("taskkill /im plantsvszombies.exe");
	}
	else
	{
		MessageBoxA(NULL, "启动器即将关闭，游戏\n可能会出现bug！", "提示", MB_OK | MB_ICONERROR);
	}
	return 0;
}

DWORD WINAPI flyplantthread(LPVOID)
{
	Plant* plants[100];
	int len = pvz->GetAllPlants(plants);
	for (int i = 0; i < len; i++)
	{
		while (pvz->BaseAddress)
		{
			plants[i]->X = plants[i]->X + 1;
			Sleep(50);
		}
	}
	return 0;
}

void OnZombieSpawn(Event* e)
{
	if (((EventZombieSpawn*)e)->zombie->Type == ZombieType::PeashooterZombie && ((EventZombieSpawn*)e)->zombie->Temp != 66988 && pvz->GameState == PVZGameState::Playing)
	{
		CreateThread(NULL, 0, &propeazombiethread, NULL, 0, NULL);
	}
}

void OnDeath(Event* e)//僵尸死亡
{
	int newzombietype = 0;
	newzombietype = rand() % 32 - 0;
	if (((EventZombieDead*)e)->zombie->Type == ZombieType::ScreenDoorZombie)
	{
		Creater::CreateZombie((ZombieType::ZombieType)newzombietype, ((EventZombieDead*)e)->zombie->Row, ((EventZombieDead*)e)->zombie->X / 80);
		Sleep(50);
	}
	if (((EventZombieDead*)e)->zombie->Type == ZombieType::WallnutZombie)
	{
		Zombie* jackzombie = Creater::CreateZombie(ZombieType::JackintheboxZombie, ((EventZombieDead*)e)->zombie->Row, ((EventZombieDead*)e)->zombie->X / 80);
		jackzombie->State = ZombieState::JACKBOX_POP;
		jackzombie->AttributeCountdown = 10;
		jackzombie->Visible = false;
		jackzombie->Hit(100000);
		((EventZombieDead*)e)->zombie->Hit(1000000);
		((EventZombieDead*)e)->zombie->NotDying = false;
	}
}

void OnZombieHypnotized(Event* e)
{
	Zombie* zombie = ((EventZombieHypnotized*)e)->zombie;
}

void FakeCheckLevel(Event* e)
{
	mouse = pvz->GetMouse();
	seedcard = pvz->GetCardSlot();//实例化卡牌
	Zombie* zombie = ((EventZombieSpawn*)e)->zombie;
	memory = new PVZ::Memory();

	if (pvz->AdventureLevel == 5)
	{
		Creater::CreateCaption("Use your dancing zombie get win！", 33, CaptionStyle::Lowerpart, 2000);
		Sleep(2000);
		Creater::CreateCaption("And you can use your zombie to gnaw other zombie!have fun!", 58, CaptionStyle::Lowerpart, 2000);
		Sleep(2000);
		Zombie* zom = Creater::CreateZombie(ZombieType::DancingZombie, 1, 9);
		Zombie* zom2 = Creater::CreateZombie(ZombieType::DancingZombie, 1, 9);
		zom->Hypnotized = true;
		zom2->Hypnotized = true;
		while (pvz->AdventureLevel == 5)//Don't type code here!
		{
			if (zom->NotDying != false)
			{
				zom->SetBodyHp(10000, 10000);
				zom->X = mouse->X;
				zom->Y = mouse->Y;
				zom->Row = mouse->Y / 80;
				zom2->SetBodyHp(10000, 10000);
				zom2->X = mouse->X;
				zom2->Y = mouse->Y;
				zom2->Row = mouse->Y / 80;

			}
			seedcard->X = 1000;
			seedcard->Visible = false;
		}
	}
}

void OnPaperZombieSpawn2(Event* e)
{
	Zombie* zombie = ((EventZombieSpawn*)e)->zombie;
	if (zombie->Type == ZombieType::NewspaperZombie)
	{
		if (papertype == 4)
		{
			while (zombie->NotDying == true)
			{
				zombie->GarlicBited = true;
				Sleep(1000);
			}
		}
	}
}

void OnPaperZombieSpawn(Event* e)
{
	Zombie* zombie = ((EventZombieSpawn*)e)->zombie;
	if (zombie->Type == ZombieType::NewspaperZombie)
	{
		if (papertype == 2)
		{
			zombie->X = zombie->X + 0.1;
			zombie->Type = ZombieType::DancingZombie;
		}
		else if (papertype == 3)
		{
			zombie->Size = 1.2;
			zombie->Type = ZombieType::Gigagargantuar;
			zombie->Hit(200, DamageType::Side);
		}
	}
}

void OnBungeeZombieSpawn(Event* e)
{
	Zombie* zombie = ((EventZombieSpawn*)e)->zombie;
	if (zombie->Type == ZombieType::BungeeZombie)
	{
		memory->WriteMemory(0x006511B7, 80);
		while (zombie->NotExist != true)
		{
			if (pvz->GamePaused != true && pvz->GameState == 3)
			{
				Projectile* pro = Creater::CreateProjectile(ProjectileType::ZombiePea, zombie->Row, zombie->X - 72);
				pro->Motion = MotionType::Left;
				Sleep(500);
			}
		}
	}
}

void OnPlantDamage(Event* e)
{
	Plant* plant = ((EventPlantDamage*)e)->plant;
	Zombie* zombie = ((EventPlantDamage*)e)->zombie;
	int newzombietype = 0;
	srand(time(NULL));
	struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);  // milli time
	newzombietype = rand() % 32 - 0;
	if (zombie->Type == ZombieType::ScreenDoorZombie)
	{
		if (zombie->NotDying == true)
		{
			if (plant->Hp <= 30)
			{
				Creater::CreateZombie((ZombieType::ZombieType)newzombietype, plant->Row, plant->Column);
				plant->Hp = -100000;
				Sleep(50);
			}
		}
	}
	if (zombie->Type == ZombieType::WallnutZombie)
	{
		if (zombie->NotDying == true)
		{
			Zombie* jackzombie = Creater::CreateZombie(ZombieType::JackintheboxZombie, zombie->Row, zombie->X / 80);
			jackzombie->State = ZombieState::JACKBOX_POP;
			jackzombie->AttributeCountdown = 10;
			jackzombie->Visible = false;
			jackzombie->Hit(100000);
			plant->Hp = -100000;
			zombie->Hit(1000000);
			zombie->NotDying = false;
		}
	}
	if (zombie->Type == ZombieType::FlagZombie)
	{
		plant->Squash = true;
	}
	if (plant->Type == PlantType::Wallnut)
	{
		if (plant->Hp <= 30)
		{
			plant->Type = PlantType::Squash;
		}
	}
	if (plant->Type == PlantType::Hypnoshroon)
	{
		Creater::CreateExplosion(plant->X, plant->Y, 70, true, true, false);
		Creater::CreateEffect(76, plant->X + 35, plant->Y + 35);
		plant->Hp = -1000000;
	}
	if (zombie->Temp == 66988)
	{
		plant->Squash = true;
		zombie->GarlicBited = true;
	}
}

void ProjectileFire(Event* e)
{
	Projectile* pro = ((EventProjectileFire*)e)->projectile;
	if (pro->Type == ProjectileType::SnowPea)
	{
		int ShootType = 0;
		srand((unsigned int)time(NULL));
		ShootType = rand() % 12 + 1;
		Creater::CreateProjectile((ProjectileType::ProjectileType)ShootType, pro->Row, pro->X);
		pro->X = -100000;
		pro->NotExist = true;
	}
}

void IZChecker(Event* e)
{
	seedcard = pvz->GetCardSlot();
	memory = new PVZ::Memory();
	switch (pvz->LevelId)
	{
	case PVZLevel::I_Zombie:
		//僵尸
		memory->WriteMemory(0x0042A044, 26);//普通 豌豆
		memory->WriteMemory(0x0042A064, 27);//铁桶 樱桃
		memory->WriteMemory(0x0042A090, 29);//橄榄 机枪
		//阳光
		memory->WriteMemory(0x00467B60, 75);//普通
		memory->WriteMemory(0x00467B72, 50);//铁桶
		//字幕
		break;
	case PVZLevel::I_ZombieToo:
		//僵尸
		memory->WriteMemory(0x0042A044, 12);//普通 冰车
		memory->WriteMemory(0x0042A0A6, 22);//臭虫 投篮
		memory->WriteMemory(0x0042A064, 13);//铁桶 小队
		//阳光
		memory->WriteMemory(0x00467B60, 150);//普通
		memory->WriteMemory(0x00467B3D, 125);//臭虫
		memory->WriteMemory(0x00467B72, 75);//铁桶
		//字幕
		break;
	case PVZLevel::Can_You_DigIt:
		CreateThread(NULL, 0, &flyplantthread, NULL, 0, NULL);
		break;
	default:
		memory->WriteMemory(0x0042A044, 0);//普通
		memory->WriteMemory(0x0042A064, 4);//铁桶
		memory->WriteMemory(0x0042A090, 7);//橄榄
		memory->WriteMemory(0x0042A0A6, 6);//臭虫
		//阳光
		memory->WriteMemory(0x00467B60, 50);//普通
		memory->WriteMemory(0x00467B3D, 100);//臭虫
		memory->WriteMemory(0x00467B72, 125);//铁桶
		memory->WriteMemory(0x00467B7E, 175);//橄榄
		break;
	}
}

void OnPlantPlant(Event* e)
{
	Plant* plant = ((EventPlantPlant*)e)->plant;
	if (plant->Type == PlantType::Imitater)
	{
		plant->AttributeCountdown = 10;
	}
	//正式代码
	if (plant->Type == PlantType::Marigold)//金盏花修改，热带向日葵
	{
		Creater::CreateCoin(CoinType::NormalSun, (plant->Row + 1) * 75, (plant->Column + 1) * 78, CoinMotionType::Product);
		plant->Hp = 10;
	}
	if (plant->Type == PlantType::Blover)//三叶草修改
	{
		Zombie* zombies[500];
		int len = pvz->GetAllZombies(zombies);
		for (int i = 0; i < len; i++)
		{
			zombies[i]->X = zombies[i]->X + 82;
			zombies[i]->Hit(50, DamageType::Side_Ice);
		}
	}
	if (plant->Type == PlantType::Puffshroon)
	{
		Zombie* zombies[500];
		int len = pvz->GetAllZombies(zombies);
		int anyth;
		for (int i = 0; i < 6; i++)
		{
			Projectile* peabomb = Creater::CreateProjectile(ProjectileType::Pea, plant->Row, plant->X);
			if (len != 0)
			{
				for (int j = 0; j < len; j++)
				{
					peabomb->Motion = MotionType::Track;
					srand((unsigned int)time(NULL));
					anyth = rand() % len;
					peabomb->TracktargetId = zombies[j]->Id;
				}
			}
			else
			{
				peabomb->Motion = MotionType::Direct;
			}
			Sleep(50);
		}
		plant->Hp = -10000;
	}
}

void OnPlantDead(Event* e)
{
	Plant* plant = ((EventPlantDead*)e)->plant;
	Zombie* zombie = ((EventPlantDead*)e)->zombie;
	if (plant->Type == PlantType::Explodenut)
	{
		if (plant->Sleeping == false)
		{
			Creater::CreateExplosion(plant->X, plant->Y, 100, true, true, false);
			Creater::CreateEffect(4, plant->X + 35, plant->Y + 35);
			plant->Sleeping = true;
		}
	}
	if (plant->Type == PlantType::Marigold)
	{
		if (plant->Sleeping == false)
		{
			for (int i = 0; i < 3; i++)
			{
				Creater::CreateCoin(CoinType::NormalSun, (plant->Row + 1) * 75, (plant->Column + 1) * 78, CoinMotionType::Product);
			}
			plant->Sleeping = true;
		}
	}
}

void OnZombieDamage(Event* e)
{
	Projectile* damagepro = ((EventZombieDamage*)e)->projectile;
	Zombie* damagezombie = ((EventZombieDamage*)e)->zombie;
	if (damagepro->Type == ProjectileType::Cabbage)
	{
		Zombie* zombie = Creater::CreateZombie(ZombieType::Zombie, damagepro->Row, damagepro->X / 80 - 0.5);
		zombie->Hypnotized = true;
		damagepro->X = -100000;
		damagepro->NotExist = true;
	}
}

void OnLevelStart(Event* e)
{
	Creater::CreatePlant(PlantType::Chomper, 0, 0);
}

DWORD WINAPI PaperZombieSpawnThread(LPVOID)
{
	EventHandler e(pvz);
	e.RegistryListeners("ZombieSpawn", OnPaperZombieSpawn);
	while (pvz->BaseAddress)
	{
		srand((unsigned)time(NULL));
		papertype = rand() % 4 + 1;
		e.Run();
		Sleep(50);
	}
	return 0;
}

DWORD WINAPI PaperZombieSpawnThread2(LPVOID)
{
	EventHandler e(pvz);
	e.RegistryListeners("ZombieSpawn", OnPaperZombieSpawn2);
	while (pvz->BaseAddress)
	{
		srand((unsigned)time(NULL));
		papertype = rand() % 5 + 1;
		e.Run();
		Sleep(50);
	}
	return 0;
}

DWORD WINAPI BungeeZombieSpawnThread(LPVOID)
{
	EventHandler e(pvz);
	e.RegistryListeners("ZombieSpawn", OnBungeeZombieSpawn);
	while (pvz->BaseAddress)
	{
		srand((unsigned)time(NULL));
		e.Run();
		Sleep(10);
	}
	return 0;
}

Zombie* firstboss;

DWORD WINAPI level10thread(LPVOID)
{
	int hp, maxhp;
	int bossfirst = 0;
	int bosssecond = 0;
	int bossthird = 0;
	int boss4th = 0;
	int boss5th = 0;
	int boss6th = 0;
	Plant* plants[100];
	Zombie* boss = firstboss;
	Creater::CreateCaption("Hello!                                                         ", 63, CaptionStyle::Bottom, 300);
	boss->SetBodyHp(6000, 6000);
	while (boss->NotExist == false)
	{
		boss->GetBodyHp(&hp, &maxhp);
		boss->X = 400;
		boss->Size = 1.5;
		boss->Row = 2;
		if (hp < 5000 && boss6th == 0)
		{
			boss->SetAnimation("anim_smash", APA_LOOP);
			boss->GetAnimation()->Speed = 2;
			for (int i = 0; i < 10; i++)
			{
				Projectile* bosspea = Creater::CreateProjectile(ProjectileType::ZombiePea, boss->Row, boss->X);
				bosspea->Motion = MotionType::LeftSlide;
				bosspea->RotationSpeed = 10;
				Sleep(50);
			}
			boss6th = 1;
		}
		if (hp <= 4500 && bossfirst == 0)
		{
			Creater::CreateCaption("Brothers go into battle!                                                         ", 81, CaptionStyle::Bottom, 300);
			for (int i = 0; i < 5; i++)
			{
				Creater::CreateZombie(ZombieType::Gargantuar, i, 6);
			}
			bossfirst = 1;
		}
		if (hp <= 4300 && boss4th == 0)
		{
			Creater::CreateZombie(ZombieType::PogoZombie, 1, 13);
			Creater::CreateZombie(ZombieType::PogoZombie, 3, 13);
			boss->SetAnimation("anim_smash", APA_ONCE_STOP);
			Plant* ea = Creater::CreatePlant(PlantType::FlowerPot, boss->Row, boss->X / 80);
			ea->Visible = false;
			boss4th = 1;
		}
		if (hp <= 3000 && bosssecond == 0)
		{
			Creater::CreateCaption("Brothers revive~                                                         ", 73, CaptionStyle::Bottom, 300);
			for (int i = 0; i < 5; i++)
			{
				Creater::CreateZombie(ZombieType::Gigagargantuar, i, 6);
			}
			bosssecond = 1;
		}
		if (hp <= 1000 && bossthird == 0)
		{
			Creater::CreateCaption("HAHAHA!Are you think you can win this battle?                   ", 64, CaptionStyle::Bottom, 500);
			for (int i = 0; i < pvz->GetAllPlants(plants); i++)
			{
				Creater::CreateEffect(76, plants[i]->X + 35, plants[i]->Y + 35);
				plants[i]->Hp = -10000;
			}
			bossthird = 1;
		}
		if (hp <= 500 && boss5th == 0)
		{
			Creater::CreateCaption("I will be back!                   ", 34, CaptionStyle::Bottom, 500);
			boss5th = 1;
		}
	}
	int hp1, maxhp1;
	int bossfirst2 = 0;
	int bosssecond2 = 0;
	int bossthird2 = 0;
	int boss4th2 = 0;
	int boss5th2 = 0;
	int boss6th2 = 0;
	Zombie* boss2 = Creater::CreateZombie(ZombieType::Gigagargantuar, 2, 8);
	boss2->SetBodyHp(8000, 8000);
	Creater::CreateCaption("Hey,We meet again,XD!                   ", 40, CaptionStyle::Bottom, 500);
	while (boss2->NotExist == false)
	{
		boss->GetBodyHp(&hp1, &maxhp1);
		boss2->X = 400;
		boss2->Size = 2;
		boss2->Row = 2;
		if (hp1 < 7500 && bossfirst2 == 0)
		{
			boss2->SetAnimation("anim_smash", APA_LOOP);
			boss2->GetAnimation()->Speed = 2;
			for (int i = 0; i < 50; i++)
			{
				Projectile* bosspea = Creater::CreateProjectile(ProjectileType::ZombiePea, boss2->Row, boss2->X);
				bosspea->Motion = MotionType::LeftSlide;
				bosspea->RotationSpeed = 10;
				Sleep(50);
			}
			bossfirst2 = 1;
		}
		if (hp1 < 6500 && bosssecond2 == 0)
		{
			Creater::CreateCaption("Grave Dance!                   ", 12, CaptionStyle::Bottom, 12);
			for (int i = 0; i < 5; i++)
			{
				Creater::CreateGrave(i, 9);
				Creater::CreateGrave(i, 8);
			}
			bosssecond2 = 1;
		}
		if (hp1 < 5500 && boss6th2 == 0)
		{
			Creater::CreateCaption("Do you like ladders?                   ", 39, CaptionStyle::Bottom, 20);
			boss2->SetAnimation("anim_smash", APA_LOOP);
			boss2->GetAnimation()->Speed = 10;
			for (int i = 0; i < pvz->GetAllPlants(plants); i++)
			{
				Creater::CreateLadder(plants[i]->Row, plants[i]->Column);
			}
			boss6th2 = 1;
		} 
		if (hp1 < 4500 && bossthird2 == 0)
		{
			boss2->SetAnimation("anim_smash", APA_LOOP);
			boss2->GetAnimation()->Speed = 10;
			Creater::CreateCaption("lolol,you can't kill me!                   ", 43, CaptionStyle::Bottom, 12);
			bossthird2 = 1;
		}
		if (hp1 <= 3000 && boss4th2 == 0)
		{
			boss2->SetAnimation("anim_smash", APA_LOOP);
			boss2->GetAnimation()->Speed = 10;
			for (int i = 0; i < pvz->GetAllPlants(plants); i++)
			{
				plants[i]->Squash = true;
			}
			boss4th2 = 1;
			boss2->Light(5000);
		}
		if (hp1 <= 1500 && boss5th2 == 0)
		{
			while (hp <= 1500)
			{
				boss2->Light(100);
				boss2->SetAnimation("anim_smash", APA_LOOP);
				boss2->GetAnimation()->Speed = 10;
			}
			boss5th2 = 1;
		}
	}
	return 0;
}

DWORD WINAPI levelthread(LPVOID)
{
	while (pvz->BaseAddress)
	{
		Plant* plants[100];
		Zombie* zombies[100];
		if (pvz->AdventureLevel == 51)
		{
			ZombieType::ZombieType ztypes[] = { ZombieType::FootballZombie,ZombieType::DiggerZombie,ZombieType::NewspaperZombie,ZombieType::JackintheboxZombie,ZombieType::DancingZombie };
			Creater::CreateZombieInLevel(ztypes, sizeof(ztypes));
			if (pvz->GetAllPlants(plants) == 0 && pvz->GameState == PVZGameState::Preparing)
			{
				for (int i = 0; i < 5; i++)
				{
					Creater::CreatePlant(PlantType::UmbrellaLeaf, i, 0);
				}
				MessageBoxA(PVZhWnd, "拓展关卡第一关：6-1\n规则：不要让僵尸吃掉你的叶子保护伞！\n提示：他很脆弱。", "关卡提示：", MB_OK);
				MessageBoxA(PVZhWnd, "本关出怪：\n橄榄僵尸\n矿工僵尸\n报纸僵尸\n小丑僵尸\n舞王僵尸", "关卡信息：", MB_OK);
			}
		}
		if (pvz->AdventureLevel == 10)
		{
			if (pvz->GetAllPlants(plants) == 0 && pvz->GameState == PVZGameState::Playing)
			{
				int diyici = 0;
				while (diyici == 0)
				{
					pvz->GetWave(5)->Set(1, ZombieType::Gargantuar);
					while (firstboss == NULL)
					{
						for (int i = 0; i < pvz->GetAllZombies(zombies); i++)
						{
							if (zombies[i]->Type == ZombieType::Gargantuar)
							{
								firstboss = zombies[i];
							}
						}
					}
					diyici = 1;
				}
				CreateThread(NULL, 0, &level10thread, NULL, 0, NULL);
			}
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI gardenthread(LPVOID)
{
	int myplant;
	PVZ::MousePointer* mop;
	while (pvz->BaseAddress)
	{
		if (pvz->LevelId == PVZLevel::Sunny_Day)
		{
			mop = pvz->GetMousePointer();
			srand(time(NULL));
			struct timeb timeSeed;
			ftime(&timeSeed);
			srand(timeSeed.time * 1000 + timeSeed.millitm);  // milli time
			myplant = rand() % 48 + 1;
			if ((CardType::CardType)myplant != CardType::Imitater)
			{
				mop->ContentCard = (CardType::CardType)myplant;
			}
			PVZ::Coin* coins[199];
			int len = pvz->GetAllCoins(coins);
			for (int i = 0; i < len; i++)
			{
				if (coins[i]->Type == CoinType::LargeSun)
				{
					coins[i]->Type = CoinType::NormalSun;
				}
			}
			PVZ::CardSlot* slot = pvz->GetCardSlot();
			for (int j = 0; j < 10; j++)
			{
				if ((CardType::CardType)myplant != CardType::Imitater)
				{
					slot->GetCard(j)->ContentCard = (CardType::CardType)myplant;
				}
			}
		}
		Sleep(1);
	}
	return 0;
}

int main()
{
	GetWindowThreadProcessId(PVZhWnd, &pid);
	SetConsoleCtrlHandler(OnExit, TRUE);
	WinExec("PlantsVsZombies.exe", SW_NORMAL);
	//pid = ProcessOpener::OpenByFilePath(ProcessOpener::Directory, TEXT("PlantsVsZombies.exe"));
	if (!pid)
	{
		pid = ProcessOpener::Open();
		if (!pid)
		{
			ProcessOpener::OpenByProcessName(TEXT("PlantsVsZombies.exe"));
		}
	}

	if (pid)
	{
		PVZhWnd = FindWindowA(NULL, PVZText);
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
		cout << "PVZ Glitch" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
		cout << "此版本为基于pvzclass的开源项目！\n开源地址：https://github.com/FxmlesXD/PVZ-Glitch" << endl;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_BLUE);
		cout << "当前版本:" << Version << endl;
		cout << "---------------DEBUG---------------" << endl;
		cout << "Console hWnd= " << hWnd << endl;
		cout << "PVZ hWnd= " << PVZhWnd << endl;
		cout << endl;
	}

	while (pid)
	{
		PVZhWnd = FindWindowA(NULL, PVZText);
		pid = ProcessOpener::OpenByProcessName(TEXT("PlantsVsZombies.exe"));
		
		pvz = new PVZ(pid);
		seedcard = pvz->GetCardSlot();
		memory = new PVZ::Memory();
		mouse = pvz->GetMouse();
		
		EnableBackgroundRunning(true);
		ShowHiddenLevel(true);

		//memory->WriteMemory(0x0040B08F, 250);//第一关初始阳光
		memory->WriteMemory(0x0069F9A4, 75);//爆炸坚果价格
		memory->WriteMemory(0x0069F740, 250);//僵尸投手价格（卷心菜投手。）
		//memory->WriteMemory(0x004309F0, 50);//普通阳光价值
		//memory->WriteMemory(0x004309FD, 856615700);//小阳光价值
		memory->WriteMemory(0x004268FA, 1183527536);//坟墓模式
		memory->WriteMemory(0x0045FAFC, 1665568223);//金盏花第一个（扭头向日葵）
		memory->WriteMemory(0x0045FB0B, 5);//金盏花第二个（扭头向日葵）
		memory->WriteMemory(0x0052378F, 10);//樱桃炸弹僵尸 （坚果僵尸头颅对应动画）
		memory->WriteMemory<byte>(0x00483F1A, 235);//戴夫不选卡
		memory->WriteMemory(0x6511BC, 113);
		
		//These codes make spawn zombies faster than original PVZ.
		///////////////////////////////////////////////////////////////
		byte __EB_68[2]{ 0xEB, 0x68 };
		PVZ::Memory::WriteArray<byte>(0x40D91A, STRING(__EB_68));
		byte __E9_D1_00_90[6]{ 0xE9, 0xD1, 0x00, 0x00, 0x00, 0x90 };
		PVZ::Memory::WriteArray<byte>(0x40988A, STRING(__E9_D1_00_90));
		byte __EB_15[6]{ 0xEB, 0x15 };
		PVZ::Memory::WriteArray<byte>(0x40AB5A, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x40D487, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x419B91, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x41C058, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x41D149, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x41F7D7, STRING(__EB_15));
		PVZ::Memory::WriteArray<byte>(0x41F831, STRING(__EB_15));
		////////////////////////////////////////////////////////////////

		if (pvz->BaseAddress)
		{
			if (pvz->LevelId == PVZLevel::Sunny_Day)
			{
				MessageBoxA(PVZhWnd, "自制关卡：随机种植！\n规则：手上的植物随机切换\n卡槽栏的植物也会随机切换\n注：本关的大阳光捡起时会变成中等阳光\n", "关卡提示：", MB_OK);
			}
			CreateThread(NULL, 0, &PaperZombieSpawnThread, NULL, 0, NULL);
			CreateThread(NULL, 0, &PaperZombieSpawnThread2, NULL, 0, NULL);
			CreateThread(NULL, 0, &BungeeZombieSpawnThread, NULL, 0, NULL);
			CreateThread(NULL, 0, &levelthread, NULL, 0, NULL);
			CreateThread(NULL, 0, &gardenthread, NULL, 0, NULL);
			EventHandler e(pvz);
			e.RegistryListeners("ZombieHypnotized", OnZombieHypnotized);
			e.RegistryListeners("PlantDamage", OnPlantDamage);
			e.RegistryListeners("ZombieDead", OnDeath);
			e.RegistryListeners("ZombieSpawn", FakeCheckLevel);
			e.RegistryListeners("ZombieSpawn", OnZombieSpawn);
			e.RegistryListeners("PlantDead", OnPlantDead);
			e.RegistryListeners("PlantPlant", OnPlantPlant);
			e.RegistryListeners("LevelStart", IZChecker);
			e.RegistryListeners("ProjectileFire", ProjectileFire);
			e.RegistryListeners("ZombieDamage", OnZombieDamage);
			//e.RegistryListeners("LevelOpen", OnLevelStart);
			while (pvz->BaseAddress)
			{
				e.Run();
				Sleep(10);
			}
		}
		delete pvz;
	}
	return 0;
}
