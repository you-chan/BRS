#include <time.h>
#include <SDL/SDL.h>
#include "common.h"
#include "func.h"

static Pos DecideGoal(Pos pos, int command, int dir, int speed);
static int DecideDir(int ct, Pos pos);
static int DecideDistance(Pos p1, Pos p2);
static void Fire(int ct);
static void BossFire(int type);
static void MakeShot(int ct, int dir);
static void BossMakeShot(int type, int dir);
static Pos MoveChara(int i, Pos pos, Pos startpos, Pos goalpos, int dir);
static int MoveDir(int startdir, int goaldir);
static void MoveShot();

int restplayer; /* 残り人数 */

/*****************************************************************
関数名 : InitSystem
機能	: 起動時の初期化処理、1回のみ行う処理を実行
引数	: なし
出力	: なし
*****************************************************************/
void InitSystem()
{
    /* 乱数初期化 */
    srand(time(NULL));

	gState = GAME_TITLE;

	gGun[GUN_1SHOT].atk    = 100;
	gGun[GUN_1SHOT].color  = 0xFF0000FF;
	gGun[GUN_3SHOT].atk    = 70;
	gGun[GUN_3SHOT].color  = 0xFF00FFFF;
	gGun[GUN_BUBBLE].atk   = 1;
	gGun[GUN_BUBBLE].color = 0x0000FF80;
	gGun[GUN_MILK].atk     = 1;
	gGun[GUN_MILK].color   = 0xFFFFFFFF;
	gGun[GUN_5SHOT].atk    = 100;
	gGun[GUN_5SHOT].color  = 0xC0C0C0FF;

	gArmor[ARMOR_LIGHT].hp		= 300;
	gArmor[ARMOR_LIGHT].speed	= 500;
	gArmor[ARMOR_MIDDLE].hp		= 450;
	gArmor[ARMOR_MIDDLE].speed	= 150;
	gArmor[ARMOR_HEAVY].hp		= 800;
	gArmor[ARMOR_HEAVY].speed	= 50;

	gBoss[SENKOUSHA].hp		= 5000;
	gBoss[SENKOUSHA].w		= 200;
	gBoss[SENKOUSHA].h		= 400;
	gBoss[SENKOUSHA].speed	= 150;
	gBoss[SENKOUSHA].gun[0]	= GUN_MILK;
	gBoss[SENKOUSHA].gun[1]	= -1;//GUN_5SHOT;
	gBoss[SENKOUSHA].gun[2]	= -1;

	gBoss[SENKOUSHA].hp		= 5000;
	gBoss[SENKOUSHA].w		= 200;
	gBoss[SENKOUSHA].h		= 400;
	gBoss[SENKOUSHA].speed	= 150;
	gBoss[SENKOUSHA].gun[0]	= GUN_5SHOT;
	gBoss[SENKOUSHA].gun[1]	= -1;//GUN_5SHOT;
	gBoss[SENKOUSHA].gun[2]	= -1;
}

/*****************************************************************
関数名 : InitTitle
機能	: タイトルでの初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitTitle()
{
	tState = ADVENTURE;
}

/*****************************************************************
関数名 : InitEdit
機能	: 編集での初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitEdit()
{
	eState = EDIT_GUN;
	gChara[0].gun = GUN_1SHOT;
	gChara[0].armor = ARMOR_MIDDLE;
}

/*****************************************************************
関数名 : InitMain
機能	: メインでの初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitMain()
{
	int i, j;

	if(tState == VS_MODE) //人数設定、通信時要変更
		CT_NUM = MAX_CT;
	else CT_NUM = 2;

	restplayer = CT_NUM;
	mState = MAIN_COMMAND;	//コマンド選択画面へ
	cState = COMMAND_DIR;	//方向選択から選択できるように
	count = 0;
	nowcommand = 0;
	lastcount = MAX_COUNT;
	gspeed = 1;
	win = 0;
	gCommand.dir = 0;		 //上方向から選択できるように
	gCommand.gun = C_SCOPE; //照準から選択できるように

	 /* 座標, 角度の決定 */
	for(i=0; i<CT_NUM; i++){
		gChara[i].pos.x = S_SIZE;//S_SIZE + i * (F_WIDTH - 3 * S_SIZE) / CT_NUM;
		gChara[i].pos.y = rand() % (HEIGHT-S_SIZE);
		gChara[i].dir = 0;//rand() % 360;
		gChara[i].state = LIVING;
		gChara[i].maxhp = gArmor[gChara[i].armor].hp;
		gChara[i].hp = gChara[i].maxhp;
		gChara[i].commandnum = 0;
		for(j=0; j<MAX_COMMAND; j++)
			gChara[i].command[j] = -1;
	}

	/* 玉の初期化 */
	for(i=0; i<MAX_SHOT; i++){
		gShot[i].id = -1;
		gShot[i].pos.x = gShot[i].pos.y = gShot[i].dir = 0;
		gShot[i].state = DEAD;
	}

	/* 敵の武器決定(ランダム) */
	for(i=1; i<CT_NUM; i++){
		gChara[i].gun	= rand() % MAX_PLAYERGUN;
		gChara[i].armor = rand() % MAX_ARMOR;
		gChara[i].maxhp = gArmor[gChara[i].armor].hp;
		gChara[i].hp = gChara[i].maxhp;
	}
}

/*****************************************************************
関数名 : InitAdventure
機能	: Adventureでの初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitAdventure()
{
	int i;
	gEnemy.no = 0;//とりあえず0番
	gEnemy.hp = gEnemy.maxhp = gBoss[gEnemy.no].hp;
	gEnemy.w = gBoss[gEnemy.no].w;
	gEnemy.h = gBoss[gEnemy.no].h;
	gEnemy.pos.x = F_WIDTH - gEnemy.w;
	gEnemy.pos.y = (HEIGHT-gEnemy.h)/2;
	gEnemy.dir = 0;
	gEnemy.state = LIVING;
	gEnemy.speed = gBoss[gEnemy.no].speed;
	for(i=0; i<MAX_BOSSGUN; i++)
		gEnemy.gun[i] = gBoss[gEnemy.no].gun[i];
	for(i=0; i<MAX_BOSSCOMMAND; i++){
		if(i % 2 == 0)
			//gEnemy.command[i] = C_SCOPE;
			gEnemy.command[i] = C_FIRE;
		else
			gEnemy.command[i] = C_FIRE;
	}
}

/*****************************************************************
関数名 : UseCommand
機能	: コマンドの適用
引数	: なし
出力	: なし
*****************************************************************/
void UseCommand()
{
	/* カウントを設け、カウントが増えるに伴い各座標も移動する */
	int i, j;
	Pos bpos;

	MoveShot();

	/* 初回 */
	if(count == 0){
		/* Player */
		for(i=0; i<CT_NUM; i++){
			if(gChara[i].state == LIVING)
				switch(gChara[i].command[nowcommand % gChara[i].commandnum]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: //8方位移動
					gChara[i].startpos = gChara[i].pos;
					gChara[i].goalpos =  DecideGoal(gChara[i].pos, gChara[i].command[nowcommand % gChara[i].commandnum],
													gChara[i].dir, gArmor[gChara[i].armor].speed);
					break;
				case C_SCOPE: //照準
					gChara[i].startdir = (gChara[i].dir % 360);
					gChara[i].goaldir = DecideDir(i, gChara[i].pos);
					break;
				case C_FIRE: //発射
					Fire(i);
					break;
				default:
					break;
				}
		}
		/* Boss */
		if(tState == ADVENTURE && gEnemy.state == LIVING)
			switch(gEnemy.command[nowcommand % MAX_BOSSCOMMAND]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: //8方位移動
					gEnemy.startpos = gEnemy.pos;
					gEnemy.goalpos =  DecideGoal(gEnemy.pos, gEnemy.command[nowcommand % MAX_BOSSCOMMAND], gEnemy.dir, gEnemy.speed);
					break;
				case C_SCOPE: //照準
					gEnemy.startdir = (gEnemy.dir % 360);
					bpos.x = gEnemy.pos.x + gEnemy.w / 2;
					bpos.y = gEnemy.pos.y + gEnemy.h / 2;
					gEnemy.goaldir = DecideDir(BOSS, bpos);
					break;
				case C_FIRE: //発射
					for(i=0; i<MAX_BOSSGUN; i++)
							BossFire(i);
					break;
				default:
					break;
				}
	}
	/* カウント中 */
	else if(count <= lastcount){
		/* Player */
		for(i=0; i<CT_NUM; i++){
			if(gChara[i].state == LIVING)
				switch(gChara[i].command[nowcommand % gChara[i].commandnum]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
						gChara[i].pos = MoveChara(i, gChara[i].pos, gChara[i].startpos, gChara[i].goalpos,
								(gChara[i].dir + gChara[i].command[nowcommand % gChara[i].commandnum] * 45 % 360));
					break;
				case C_SCOPE: //照準
					if(count <= lastcount / 2)
						gChara[i].dir = MoveDir(gChara[i].startdir, gChara[i].goaldir);
					break;
				case C_FIRE: //発射
					for(j=0; j < MAX_COUNT/lastcount; j++)
						if(gChara[i].gun == GUN_BUBBLE || gChara[i].gun == GUN_MILK)
							Fire(i);
					break;
				default:
					break;
				}
		}
		/* BOSS */
		if(tState == ADVENTURE && gEnemy.state == LIVING)
				switch(gEnemy.command[nowcommand % MAX_BOSSCOMMAND]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
						gEnemy.pos = MoveChara(BOSS, gEnemy.pos, gEnemy.startpos, gEnemy.goalpos,
								(gEnemy.dir + gEnemy.command[nowcommand % MAX_BOSSCOMMAND] * 45 % 360));
					break;
				case C_SCOPE: //照準
					if(count <= lastcount / 2)
						gEnemy.dir = MoveDir(gEnemy.startdir, gEnemy.goaldir);
					break;
				case C_FIRE: //発射
					for(i=0; i<MAX_BOSSGUN; i++)
						for(j=0; j < MAX_COUNT/lastcount; j++)
							if(gEnemy.gun[i] == GUN_BUBBLE || gEnemy.gun[i] == GUN_MILK)
								BossFire(i);
					break;
				default:
					break;
				}
	}
	/* カウント終了後のリセット */
	if(count == lastcount){
		count = -1;
		lastcount = MAX_COUNT / gspeed;
		nowcommand++;
		if(nowcommand == 60)
			nowcommand = 0;
	}

	count++;
}

/*****************************************************************
関数名 : DecideGoal
機能	: キャラクターが移動する際の目的地の座標を求める
引数	: pos : 移動前の座標  command : 移動コマンド(8方位)  dir:角度  speed : 速さ
出力	: 目的地の座標
*****************************************************************/
Pos DecideGoal(Pos pos, int command, int dir, int speed)
{
	Pos goalpos = {0, 0};
	goalpos.x = pos.x + speed * sin((dir + command * 45) * M_PI /180);
	goalpos.y = pos.y - speed * cos((dir + command * 45) * M_PI /180);
	return goalpos;
}

/*****************************************************************
関数名 : DecideDir
機能	: 変更後の角度の決定
引数	: ct : 角度を変更するキャラクターの番号  pos : 角度を変更するキャラクターの座標
出力	: 変更後の角度
*****************************************************************/
int DecideDir(int ct, Pos pos)
{
	/* 2点間の距離が最も近いキャラクターを決め、それとの角度を求める */
	int i, d[CT_NUM], dx, dy, minid = ct, goaldir = 0;
	if(tState == ADVENTURE && ct != BOSS){ //ボス狙い
		dx = gEnemy.pos.x + (gEnemy.w / 2) - pos.x;
		dy = gEnemy.pos.y + (gEnemy.h / 2) - pos.y;
	}
	else{
		if(ct == BOSS){ //BOSS -> Player
			minid = 0;
			for(i=0; i<CT_NUM; i++){
				d[i] = DecideDistance(pos, gChara[i].pos);
				d[minid] = DecideDistance(pos, gChara[minid].pos);
				if(d[minid] >= d[i] && gChara[i].state == LIVING)
					minid = i;
			}
		}
		else{
			for(i=0; i<CT_NUM; i++){
				d[i] = DecideDistance(pos, gChara[i].pos);
				d[minid] = DecideDistance(pos, gChara[minid].pos);
				if(((d[minid] >= d[i] && i != ct) || minid == ct) && gChara[i].state == LIVING)
					minid = i;
			}
		}
		dx = gChara[minid].pos.x - pos.x; //距離の差
		dy = gChara[minid].pos.y - pos.y;
	}
	goaldir = (atan2(dy, dx) * 180 / M_PI + 450);
	return goaldir % 360;
}

/*****************************************************************
関数名 : DecideDistance
機能	: 2点間の距離の測定
引数	: p1, p2 : 各キャラクターの座標
出力	: 距離の2乗
*****************************************************************/
int DecideDistance(Pos p1, Pos p2)
{
	int d = 0;
	d = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	return d;
}

/*****************************************************************
関数名 : Fire
機能	: 玉の発射処理
引数	: ct : 発射したキャラクターの番号　gun : 武器の種類
出力	: なし
*****************************************************************/
void Fire(int ct)
{
	int i,j;
	i=j=0;
	switch(gChara[ct].gun){
	case GUN_1SHOT:
		MakeShot(ct, gChara[ct].dir);
		break;
	case GUN_3SHOT:
		MakeShot(ct, gChara[ct].dir);
		MakeShot(ct, (gChara[ct].dir +       10 ) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 - 10) % 360);
		break;
	case GUN_BUBBLE:
		MakeShot(ct, gChara[ct].dir + 10 * sin((double)count / (lastcount/2) * M_PI)); //2n + 1 - n
		break;
	case GUN_MILK:
		j=1;
		for(i=0; i<j; i++)
			MakeShot(ct, count * 9 + i * 360/j);
		break;
	case GUN_5SHOT:
		MakeShot(ct, gChara[ct].dir);
		MakeShot(ct, (gChara[ct].dir +       10) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 - 10) % 360);
		MakeShot(ct, (gChara[ct].dir +       20) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 - 20) % 360);
		break;
	default:
		break;
	}
}

/*****************************************************************
関数名 : BossFire
機能	: 玉の発射処理
引数	: type : ボスの武器配列の添字(0〜MAX_BOSSGUN)　gun : 武器の種類
出力	: なし
*****************************************************************/
void BossFire(int type)
{
	int i,j;
	switch(gEnemy.gun[type]){
	case GUN_1SHOT:
		BossMakeShot(type, gEnemy.dir);
		break;
	case GUN_3SHOT:
		BossMakeShot(type, gEnemy.dir);
		BossMakeShot(type, (gEnemy.dir +       10 ) % 360);
		BossMakeShot(type, (gEnemy.dir + 360 - 10) % 360);
		break;
	case GUN_BUBBLE:
		BossMakeShot(type, gEnemy.dir + 10 * sin((double)count / (lastcount/2) * M_PI)); //2n + 1 - n
		break;
		case GUN_MILK:
		//BossMakeShot(type, gEnemy.dir + 10 * sin((double)count / (lastcount/2) * M_PI));
		j=18;
		///*
		for(i=0; i<j; i++)
			BossMakeShot(type, count * MAX_COUNT / lastcount * 6 + i * 360/j);
		//*/
		break;
	case GUN_5SHOT:
		BossMakeShot(type, gEnemy.dir);
		BossMakeShot(type, (gEnemy.dir +        5) % 360);
		BossMakeShot(type, (gEnemy.dir + 360 -  5) % 360);
		BossMakeShot(type, (gEnemy.dir +       10) % 360);
		BossMakeShot(type, (gEnemy.dir + 360 - 10) % 360);
		break;
		default:
		break;
	}
}

/*****************************************************************
関数名 : MakeShot
機能	: 玉の作成
引数	: ct : 発射したキャラクターの番号　dir : 角度  color : 玉の色
出力	: なし
*****************************************************************/
void MakeShot(int ct, int dir)
{
	int i;
	for(i=0; i<MAX_SHOT; i++)
		if(gShot[i].state == DEAD){
			gShot[i].id = ct;
			gShot[i].state = LIVING;
			gShot[i].dir = dir;
			gShot[i].pos.x = gChara[ct].pos.x + S_SIZE/2;
			gShot[i].pos.y = gChara[ct].pos.y + S_SIZE/2;
			gShot[i].color = gGun[gChara[ct].gun].color;
			break;
		}
}

/*****************************************************************
関数名 : BossMakeShot
機能	: ボス用の玉の作成
引数	: type : ボスの武器配列の添字(0〜MAX_BOSSGUN)　dir : 角度
出力	: なし
*****************************************************************/
void BossMakeShot(int type, int dir)
{
	int i;
	for(i=0; i<MAX_SHOT; i++)
		if(gShot[i].state == DEAD){
			gShot[i].id = type - MAX_BOSSGUN; //type == id + MAXでダメージ計算
			gShot[i].state = LIVING;
			gShot[i].dir = dir;
			gShot[i].pos.x = gEnemy.pos.x + gEnemy.w / 2;
			gShot[i].pos.y = gEnemy.pos.y + gEnemy.h / 2;
			gShot[i].color = gGun[gEnemy.gun[type]].color;
			break;
		}
}

/*****************************************************************
関数名 : MoveChara
機能	: キャラクターの移動
引数	: pos : 移動前の座標
出力	: なし
*****************************************************************/
Pos MoveChara(int ct, Pos pos, Pos startpos, Pos goalpos, int dir)
{
	Pos newpos;
	newpos.x = startpos.x + count * (goalpos.x - startpos.x) / lastcount;
	newpos.y = startpos.y + count * (goalpos.y - startpos.y) / lastcount;
	/*if(k != gShot[j].id && gChara[k].state == LIVING && gShot[j].state == LIVING &&
							newpos.x >= gChara[k].pos.x && newpos.x <= gChara[k].pos.x + S_SIZE &&
							newpos.y >= gChara[k].pos.y && newpos.y <= gChara[k].pos.y + S_SIZE)*/
	/* 壁と衝突時 */
	//for(i=0; i<CT_NUM; i++){
		if((newpos.x < 0) || (newpos.x + S_SIZE > F_WIDTH))
			newpos.x = pos.x;
		if((newpos.y < 0) || (newpos.y + S_SIZE > HEIGHT))
			newpos.y = pos.y;
	//}
	return newpos;
}

/*****************************************************************
関数名 : MoveDir
機能	: 角度の変更
引数	: startdir : 変更を開始する前の角度  goaldir : 目的の角度
出力	: 変更後の角度
*****************************************************************/
int MoveDir(int startdir, int goaldir)
{
	return (startdir + (count * (goaldir-startdir) / (lastcount/2))) % 360; //(lastcount / n)でn倍速く回転
}

/*****************************************************************
関数名 : MoveShot
機能	: 先輩こいつ玉とか移動しはじめましたよ
引数	: なし
出力	: なし
*****************************************************************/
void MoveShot()
{
	int i, j, k;
	Pos newpos;
	for(i=0; i<MAX_COUNT/lastcount; i++)
		for(j=0; j<MAX_SHOT; j++)
			if(gShot[j].state == LIVING){
				gShot[j].pos.x = gShot[j].pos.x + 10 * sin(gShot[j].dir * M_PI /180);
				gShot[j].pos.y = gShot[j].pos.y - 10 * cos(gShot[j].dir * M_PI /180);
				newpos.x = gShot[j].pos.x;
				newpos.y = gShot[j].pos.y;

				/* 壁と衝突時 */
				if((newpos.x <= 0) || (newpos.x >= F_WIDTH) || (newpos.y <= 0) || (newpos.y >= HEIGHT)){
					gShot[j].state = DEAD;
				}
				/* 玉とキャラとの衝突時 */
				if(tState != ADVENTURE)
					for(k=0; k<CT_NUM; k++){
						if(k != gShot[j].id && gChara[k].state == LIVING && gShot[j].state == LIVING &&
								newpos.x >= gChara[k].pos.x && newpos.x <= gChara[k].pos.x + S_SIZE &&
								newpos.y >= gChara[k].pos.y && newpos.y <= gChara[k].pos.y + S_SIZE){ //キャラの当たり判定
							gShot[j].state = DEAD;
							gChara[k].hp -= gGun[gChara[gShot[j].id].gun].atk;
							if( gChara[k].hp <= 0 ){
								gChara[k].state = DEAD;
								restplayer--;
							}
							if(restplayer == 1){
								if(gChara[0].state == LIVING)
									win = 1;
								else win = 0;
								mState = MAIN_RESULT;
							}
						}
					}
				else //ADVENTURE
					/* Player -> Boss */
					if(gShot[j].id >= 0){
						if(gEnemy.state == LIVING && gShot[j].state == LIVING &&
								newpos.x >= gEnemy.pos.x && newpos.x <= gEnemy.pos.x + gEnemy.w &&
								newpos.y >= gEnemy.pos.y && newpos.y <= gEnemy.pos.y + gEnemy.h){ //当たり判定
							gShot[j].state = DEAD;
							gEnemy.hp -= gGun[gChara[gShot[j].id].gun].atk;
							if( gEnemy.hp <= 0 ){
								gEnemy.state = DEAD;
								win = 1;
								mState = MAIN_RESULT;
							}
						}
					}
					/* Boss -> Player */
					else
						for(k=0; k<CT_NUM; k++){
							if(gChara[k].state == LIVING && gShot[j].state == LIVING &&
									newpos.x >= gChara[k].pos.x && newpos.x <= gChara[k].pos.x + S_SIZE &&
									newpos.y >= gChara[k].pos.y && newpos.y <= gChara[k].pos.y + S_SIZE){ //キャラの当たり判定
								gShot[j].state = DEAD;
								gChara[k].hp -= gGun[gEnemy.gun[gShot[j].id + MAX_BOSSGUN]].atk;
								if( gChara[k].hp <= 0 ){
									gChara[k].state = DEAD;
									restplayer--;
								}
								if(restplayer == 0){
									win = 0;
									mState = MAIN_RESULT;
								}
							}
						}
			}
}

/* end of system.c */
