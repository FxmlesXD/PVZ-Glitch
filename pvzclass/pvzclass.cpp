#include "pvzclass.h"
#include <iostream>
#include "events.h"
#include <ctime>
#include <sys/timeb.h>
#include <conio.h>
#include <Windows.h>
using namespace std;
//声明变量
int waittime;
int papertype;
int a0x768;
Zombie* jdshjfgshg;
Plant* hsafshahhhhh;
//////////////////////////////////
int diyicijinru = 0;
bool sixunhuanneedexit = false;
PVZLevel::PVZLevel level;
//////////////////////////////////
HWND PVZhWnd;
DWORD pid;
PVZ* pvz;
PVZ::Mouse* mouse;
PVZ::CardSlot* seedcard;
PVZ::Memory* memory;
//声明变量

BOOL WINAPI OnExit(DWORD dwCtrlType) {
	if (MessageBoxA(NULL, "您要一起关闭游戏吗？", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		system("taskkill /im plantsvszombies.exe");
	}
	else
	{
		MessageBoxA(NULL, "启动器即将关闭，游戏\n可能会出现许多\n奇奇怪怪的bug哦！", "提示", MB_OK | MB_ICONERROR);
	}
	return 0;
}

void OnDeath(Event* e)//僵尸死亡
{
	Zombie* zombie = ((EventZombieDead*)e)->zombie;
	int newzombietype = 0;
	newzombietype = rand() % 32 - 0;
	if (zombie->Type == ZombieType::ScreenDoorZombie)
	{
		Creater::CreateZombie((ZombieType::ZombieType)newzombietype, zombie->Row, zombie->X / 80);
		Sleep(50);
	}
	if (zombie->Type == ZombieType::WallnutZombie)
	{
		Zombie* jackzombie = Creater::CreateZombie(ZombieType::JackintheboxZombie, zombie->Row, zombie->X / 80);
		jackzombie->State = ZombieState::JACKBOX_POP;
		jackzombie->AttributeCountdown = 10;
		jackzombie->Visible = false;
		jackzombie->Hit(100000);
		zombie->Hit(1000000);
		zombie->NotDying = false;
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
			if (pvz->GamePaused != true)
			{
				for (int i = 0; i <= 4; i++)
				{
					if (i != 4)
					{
						Projectile* pro = Creater::CreateProjectile(ProjectileType::ZombiePea, zombie->Row, zombie->X - 72);
						pro->Motion = MotionType::Left;
						Sleep(100);
					}
					else
					{
						Sleep(500);
						continue;
					}

				}
			}
		}
	}
}

void OnPlantDamage(Event* e)
{
	Plant* plant = ((EventPlantDamage*)e)->plant;
	Zombie* zombie = ((EventPlantDamage*)e)->zombie;
	int newzombietype = 0;
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

void PowerPlantOK(Event* e)
{
	Plant* newplant = new Plant(pid);
	Plant* plant = ((EventPlantRemove*)e)->plant;
	if (plant->Type == PlantType::Imitater)
	{
		Plant* plants[100];
		int pls = pvz->GetAllPlants(plants);
		for (int j = 0; j < pls; j++)
		{
			if (plants[j]->Row == plant->Row)
			{
				if (plants[j]->Column == plant->Column)
				{
					newplant = plants[j];
					Creater::CreateEffect(96, plant->X, plant->Y);
				}
			}
		}
		//开始编写代码
		if (newplant->Type == PlantType::Garlic)//大蒜
		{
			Zombie* zombies[500];
			int len = pvz->GetAllZombies(zombies);
			for (int i = 0; i < len; i++)
			{
				zombies[i]->GarlicBited = true;
			}
			newplant->Hp = -100000;
		}
		else if (newplant->Type == PlantType::Melonpult)
		{
			int anyrow;
			int anyx;
			newplant->Hp = -10000;
			for (int i = 1; i <= 6; i++)
			{
				struct timeb timeSeed;
				ftime(&timeSeed);
				srand(timeSeed.time * 1000 + timeSeed.millitm);
				anyrow = rand() % 5;
				anyx = rand() % 800;
				Projectile* pro = Creater::CreateProjectile(ProjectileType::Melon, anyrow, anyx);
				Sleep(300);
				pro->Motion = MotionType::Direct;
			}
		}
		else if (newplant->Type == PlantType::Sunflower)
		{
			newplant->Hp = -10000;
			for (int i = 1; i <= 10; i++)
			{
				Creater::CreateCoin(CoinType::NormalSun, (newplant->Row + 1) * 75, (newplant->Column + 1) * 78, CoinMotionType::Product);
			}
		}
		else if (newplant->Type == PlantType::CherryBomb)
		{
			newplant->Hp = -100000;
			for (byte i = 0; i < 9; i++)
			{
				Creater::CreatePlant(PlantType::CherryBomb, newplant->Row, i);
			}
		}
		else
		{
			newplant->Hp = -100000;
			Creater::CreateCaption("invalid power card", 34, CaptionStyle::Bottom);
		}
	}
	delete newplant;
}

DWORD WINAPI PaperZombieSpawnThread(LPVOID pWaitTime)
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

DWORD WINAPI PaperZombieSpawnThread2(LPVOID pWaitTime)
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

DWORD WINAPI BungeeZombieSpawnThread(LPVOID pWaitTime)
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

DWORD WINAPI mainthread(LPVOID pWaitTime)
{
	EventHandler e(pvz);
	e.RegistryListeners("PlantRemove", PowerPlantOK);
	while (pvz->BaseAddress)
	{
		e.Run();
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI lhelperthread(LPVOID pWaitTime)
{
	Zombie* zombies[100];
	Sleep(1600);
	//可自定义选卡界面僵尸属性
	int a = PVZ::Memory::ReadMemory<int>(0x6A9EC0);//读取基址
	a = PVZ::Memory::ReadMemory<int>(a + 0x7fc);//偏移7FC
	if (a == 2)//判断是否是选卡状态
	{
		int len = pvz->GetAllZombies(zombies);
		for (int i = 0; i < len; i++)
		{
			//zombies[i]->State = ZombieState::DYING;//消失
			//zombies[i]->State = ZombieState::DYING_FROM_LAWNMOWER; //除草机
			zombies[i]->State = ZombieState::DYING_FROM_INSTANT_KILL; //变黑
		}
	}
	//可自定义选卡界面僵尸属性
	return 0;
}

DWORD WINAPI levelthread(LPVOID pWaitTime)
{
	while (sixunhuanneedexit == false)
	{
		Plant* plants[100];
		if (pvz->BaseAddress != a0x768 && pvz->BaseAddress != NULL && diyicijinru != 0)
		{
		FLAG:
			if (pvz->GetAllPlants(plants) == 0)
			{
				CreateThread(NULL, 0, &lhelperthread, NULL, 0, NULL);//可自定义选卡僵尸状态

				//在此键入代码
				
				//在此键入代码
				a0x768 = pvz->BaseAddress;
				Sleep(1);
			}
		}
		else if (pvz->BaseAddress != NULL && diyicijinru == 0)
		{
			diyicijinru++;
			goto FLAG;
		}
		else if (pvz->LevelId == level && pvz->BaseAddress != NULL && diyicijinru != 0 && pvz->BaseAddress == a0x768)
		{
			level = (PVZLevel::PVZLevel)245436778;
			goto FLAG;
		}
		Sleep(1);
	}
	return 0;
}

int main()
{
	PVZhWnd = FindWindowA(NULL, PVZText);
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
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
		cout << "PVZ Glitch" << endl;
		cout << "官方群号：370019571" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
		cout << "官方只在这个群发布，在其他群下载均为盗版！" << endl;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_BLUE);
		cout << "当前版本:" << Version << endl;
		cout << endl;
	}

	while (pid)
	{
		pid = ProcessOpener::OpenByProcessName(TEXT("PlantsVsZombies.exe"));

		CreateThread(NULL, 0, &PaperZombieSpawnThread, NULL, 0, NULL);
		CreateThread(NULL, 0, &PaperZombieSpawnThread2, NULL, 0, NULL);
		CreateThread(NULL, 0, &BungeeZombieSpawnThread, NULL, 0, NULL);
		CreateThread(NULL, 0, &mainthread, NULL, 0, NULL);
		
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
		
		if (pvz->BaseAddress)
		{
			a0x768 = pvz->BaseAddress;
			level = pvz->LevelId;
			HANDLE hThread233 = CreateThread(NULL, 0, &levelthread, NULL, 0, NULL);
			EventHandler e(pvz);
			e.RegistryListeners("ZombieHypnotized", OnZombieHypnotized);
			e.RegistryListeners("PlantDamage", OnPlantDamage);
			e.RegistryListeners("ZombieDead", OnDeath);
			e.RegistryListeners("ZombieSpawn", FakeCheckLevel);
			e.RegistryListeners("PlantDead", OnPlantDead);
			e.RegistryListeners("PlantPlant", OnPlantPlant);
			e.RegistryListeners("LevelStart", IZChecker);
			e.RegistryListeners("ProjectileFire", ProjectileFire);
			e.RegistryListeners("ZombieDamage", OnZombieDamage);
			while (pvz->BaseAddress)//如果在关卡内，就循环
			{
				if (pvz->LevelId == PVZLevel::Portal_Combat)
				{
					pvz->Win();
					MessageBoxA(PVZhWnd, "哈哈，你赢了", "其实这关有bug", MB_OK);
				}
				e.Run();//开启日志
				Sleep(10);//降低CPU
			}
		}

		sixunhuanneedexit = true;
		Sleep(5);
		sixunhuanneedexit = false;
		delete pvz;
	}
	return 0;
}