#include <time.h>
#include <SDL/SDL.h>
#include "common.h"
#include "func.h"

static int DecideDir(int ct, Pos pos);
static int DecideDistance(Pos p1, Pos p2);
static void Fire(int ct);
static void BossFire(int gun, Pos pos, int dir);
static void MakeShot(int ct, int dir);
static void BossMakeShot(int gun, Pos pos, int dir);
static void MakeMob(int ct, GunInfo gun, Pos pos, int dir);
static Pos MoveChara(int ct, Pos pos, int dir, int speed);
static Pos MoveBoss(Pos pos, int command);
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

	gGun[GUN_1SHOT].type	= SHOT;
	gGun[GUN_1SHOT].atk		= 100;
	gGun[GUN_1SHOT].speed	= 10;
	gGun[GUN_1SHOT].size	= 10;
	gGun[GUN_1SHOT].color	= 0xFF0000FF;
	gGun[GUN_3SHOT].type	= SHOT;
	gGun[GUN_3SHOT].atk		= 70;
	gGun[GUN_3SHOT].speed	= 10;
	gGun[GUN_3SHOT].size	= 10;
	gGun[GUN_3SHOT].color	= 0xFF00FFFF;
	gGun[GUN_5SHOT].type	= SHOT;
	gGun[GUN_5SHOT].atk		= 50;
	gGun[GUN_5SHOT].speed	= 10;
	gGun[GUN_5SHOT].size	= 10;
	gGun[GUN_5SHOT].color	= 0xC0C0C0FF;
	gGun[GUN_1SHOT].type	= SHOT;
	gGun[GUN_1SHOT].atk		= 100;
	gGun[GUN_1SHOT].speed	= 10;
	gGun[GUN_1SHOT].size	= 10;
	gGun[GUN_1SHOT].color	= 0xFF0000FF;
	gGun[GUN_NUCLEAR].type	= SHOT;
	gGun[GUN_NUCLEAR].atk	= 1000;
	gGun[GUN_NUCLEAR].speed	= 1;
	gGun[GUN_NUCLEAR].size	= 20;
	gGun[GUN_NUCLEAR].color	= 0x000000FF;
	gGun[GUN_1LASER].type	= LASER;
	gGun[GUN_1LASER].atk	= 5;
	gGun[GUN_1LASER].speed	= 1;
	gGun[GUN_1LASER].size	= 10;
	gGun[GUN_1LASER].color  = 0x00FFFFFF;
	gGun[GUN_3LASER].type	= LASER;
	gGun[GUN_3LASER].atk	= 3;
	gGun[GUN_3LASER].speed	= 1;
	gGun[GUN_3LASER].size	= 30;
	gGun[GUN_3LASER].color  = 0xFFFFFFFF;
	gGun[GUN_BUBBLE].type	= SHOT;
	gGun[GUN_BUBBLE].atk	= 3;
	gGun[GUN_BUBBLE].speed	= 10;
	gGun[GUN_BUBBLE].size	= 10;
	gGun[GUN_BUBBLE].color	= 0x0000FF80;
	gGun[GUN_MILK].type		= SHOT;
	gGun[GUN_MILK].atk		= 3;
	gGun[GUN_MILK].speed	= 10;
	gGun[GUN_MILK].size		= 10;
	gGun[GUN_MILK].color	= 0xFFFFFFFF;

	gGun[GUN_HOMO0].type	= MOB;
	gGun[GUN_HOMO0].atk		= HOMO0;
	gGun[GUN_HOMO0].speed	= 2;
	gGun[GUN_HOMO0].size	= 10;
	gGun[GUN_HOMO0].color	= 0x000000FF;

	gArmor[ARMOR_LIGHT].hp		= 300;
	gArmor[ARMOR_LIGHT].speed	= 10;
	gArmor[ARMOR_MIDDLE].hp		= 450;
	gArmor[ARMOR_MIDDLE].speed	= 5;
	gArmor[ARMOR_HEAVY].hp		= 800;
	gArmor[ARMOR_HEAVY].speed	= 3;

	mData[HOMO0].hp			= 500;
	mData[HOMO0].w			= 100;
	mData[HOMO0].h			= 100;
	mData[HOMO0].speed		= 50;
	mData[HOMO0].gun		= GUN_1SHOT;
	mData[HOMO0].command	= 898989;
	mData[HOMO0].anipat		= 1;


	mData[HOMO1].hp			= 500;
	mData[HOMO1].w			= 75;
	mData[HOMO1].h			= 100;
	mData[HOMO1].speed		= 50;
	mData[HOMO1].gun		= GUN_1SHOT;
	mData[HOMO1].command	= 898989;
	mData[HOMO1].anipat		= 1;

	bData[SENKOUSHA].hp				= 2000;
	bData[SENKOUSHA].w				= 200;
	bData[SENKOUSHA].h				= 400;
	bData[SENKOUSHA].speed			= 50;
	bData[SENKOUSHA].gun[0]			= GUN_1LASER;
	bData[SENKOUSHA].gun[1]			= -1;
	bData[SENKOUSHA].gun[2]			= -1;
	bData[SENKOUSHA].shotpos[0].x	= 72;		//右目
	bData[SENKOUSHA].shotpos[0].y	= 20;
	bData[SENKOUSHA].shotpos[1].x	= 97;		//左目
	bData[SENKOUSHA].shotpos[1].y	= 15;
	bData[SENKOUSHA].shotpos[2].x	= 76;		//腰
	bData[SENKOUSHA].shotpos[2].y	= 188;
	bData[SENKOUSHA].shotcommand[0]	= 12121212; //12001200 //武器0
	bData[SENKOUSHA].shotcommand[1]	=  1200120;
	bData[SENKOUSHA].shotcommand[2]	= 02020202;
	bData[SENKOUSHA].movecommand	= 10201020;
	bData[SENKOUSHA].anipat			= 1;
	bData[SENKOUSHA].next			= DELETE;

	bData[GAHARA].hp				= 6000;
	bData[GAHARA].w					= 300;
	bData[GAHARA].h					= 300;
	bData[GAHARA].speed				= 200;
	bData[GAHARA].gun[0]			= GUN_HOMO0;
	bData[GAHARA].gun[1]			= GUN_HOMO0;
	bData[GAHARA].gun[2]			= GUN_HOMO0;
	bData[GAHARA].shotpos[0].x		= 60;
	bData[GAHARA].shotpos[0].y		= 75;
	bData[GAHARA].shotpos[1].x		= 240;
	bData[GAHARA].shotpos[1].y		= 75;
	bData[GAHARA].shotpos[2].x		= 158;
	bData[GAHARA].shotpos[2].y		= 136;
	bData[GAHARA].shotcommand[0]	= 20000000; //12001200 //武器0
	bData[GAHARA].shotcommand[1]	= 20000000;
	bData[GAHARA].shotcommand[2]	= 20000000;
	bData[GAHARA].movecommand		= 12121212;
	bData[GAHARA].anipat			= 8;
	bData[GAHARA].next				= DELETE;

	bData[SUDACHI].hp				= 6000;
	bData[SUDACHI].w				= 300;
	bData[SUDACHI].h				= 300;
	bData[SUDACHI].speed			= 200;
	bData[SUDACHI].gun[0]			= GUN_3LASER;
	bData[SUDACHI].gun[1]			= GUN_3LASER;
	bData[SUDACHI].gun[2]			= GUN_MILK;
	bData[SUDACHI].shotpos[0].x		= 148;
	bData[SUDACHI].shotpos[0].y		= 250;
	bData[SUDACHI].shotpos[1].x		= 148;
	bData[SUDACHI].shotpos[1].y		= 250;
	bData[SUDACHI].shotpos[2].x		= 148;
	bData[SUDACHI].shotpos[2].y		= 250;
	bData[SUDACHI].shotcommand[0]	= 00120012; //12001200 //武器0
	bData[SUDACHI].shotcommand[1]	= 12001200;
	bData[SUDACHI].shotcommand[2]	= 22222222;
	bData[SUDACHI].movecommand		= 12121212;
	bData[SUDACHI].anipat			= 2;
	bData[SUDACHI].next				= DELETE;

	bData[SHIBBOLETH].hp				= 6000;
	bData[SHIBBOLETH].w					= 250;
	bData[SHIBBOLETH].h					= 300;
	bData[SHIBBOLETH].speed				= 200;
	bData[SHIBBOLETH].gun[0]			= GUN_HOMO0;
	bData[SHIBBOLETH].gun[1]			= -1;
	bData[SHIBBOLETH].gun[2]			= -1;
	bData[SHIBBOLETH].shotpos[0].x		= 47;
	bData[SHIBBOLETH].shotpos[0].y		= 55;
	bData[SHIBBOLETH].shotpos[1].x		= 47;
	bData[SHIBBOLETH].shotpos[1].y		= 55;
	bData[SHIBBOLETH].shotpos[2].x		= 47;
	bData[SHIBBOLETH].shotpos[2].y		= 55;
	bData[SHIBBOLETH].shotcommand[0]	= 0; //12001200 //武器0
	bData[SHIBBOLETH].shotcommand[1]	= 12121212;
	bData[SHIBBOLETH].shotcommand[2]	= 0;
	bData[SHIBBOLETH].movecommand		= 10201020;
	bData[SHIBBOLETH].anipat			= 1;
	bData[SHIBBOLETH].next				= DELETE;
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
	gBoss.no = GAHARA;//とりあえず
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
	int i, j, n;
	int posflg;
	Pos cpos;

	if(tState == VS_MODE) //人数設定、通信時要変更
		CT_NUM = MAX_CT;
	else CT_NUM = 2;

	restplayer = CT_NUM;
	mState = MAIN_COMMAND;	//コマンド選択画面へ
	cState = COMMAND_DIR;	//方向選択から選択できるように
	count = 0;
	nowcommand = 0;
	win = 0;
	gCommand.dir = 0;		 //上方向から選択できるように
	gCommand.gun = C_SCOPE; //照準から選択できるように

	/* 敵の武器決定(ランダム) */
	for(i=1; i<CT_NUM; i++){
		gChara[i].gun	= rand() % MAX_PLAYERGUN;
		gChara[i].armor = rand() % MAX_ARMOR;
	}

	 /* 座標, 角度の決定 */
	for(i=0; i<CT_NUM; i++){
		posflg = 1;
		n = 0;
		/* 座標決め、重なっていたらやり直し */
		do{
			gChara[i].pos.x = cpos.x = rand() % (F_WIDTH-S_SIZE);
			gChara[i].pos.y = cpos.y = rand() % (HEIGHT-S_SIZE);
			for(j=0; j<i+1; j++){
				if((i != j && cpos.x <= gChara[j].pos.x + S_SIZE && cpos.x + S_SIZE >= gChara[j].pos.x &&
							  cpos.y <= gChara[j].pos.y + S_SIZE && cpos.y + S_SIZE >= gChara[j].pos.y) ||
					(tState == ADVENTURE && cpos.x <= gBoss.pos.x + gBoss.w && cpos.x + S_SIZE >= gBoss.pos.x &&
											cpos.y <= gBoss.pos.y + gBoss.h && cpos.y + S_SIZE >= gBoss.pos.y)){
					n++;
					if(n >= CT_NUM * 10){
						printf("no put character\n");
						posflg = 0;
						gState = GAME_END;
					}
				}
				else{
					posflg = 0;
				}
			}
		} while(posflg);
		gChara[i].dir	= DecideDir(i, gChara[i].pos);
		gChara[i].state = LIVING;
		gChara[i].maxhp = gArmor[gChara[i].armor].hp;
		gChara[i].hp	= gChara[i].maxhp;
		gChara[i].atk	= gGun[gChara[i].gun].atk;
		gChara[i].speed = gArmor[gChara[i].armor].speed;
		gChara[i].commandnum = 0;
		for(j=0; j<MAX_COMMAND; j++){
			gChara[i].command[j] = -1;
		}
		for(j=0; j<MAX_USEMOB; j++){
			gMob[j].state = DEAD;
			gMob[j].pos.x = gMob[j].pos.y = 0;
		}
	}

	/* 玉の初期化 */
	for(i=0; i<MAX_SHOT; i++){
		gShot[i].id		= 0;
		gShot[i].pos.x	= gShot[i].pos.y = gShot[i].startpos.x = gShot[i].startpos.y = 0;
		gShot[i].speed	= 0;
		gShot[i].type	= SHOT;
		gShot[i].dir	= 0;
		gShot[i].size	= 0;
		gShot[i].color	= 0x00000000;
		gShot[i].state	= DEAD;
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
	int i, j;
	gBoss.hp	= gBoss.maxhp = bData[gBoss.no].hp;
	gBoss.w		= bData[gBoss.no].w;
	gBoss.h		= bData[gBoss.no].h;
	gBoss.pos.x	= F_WIDTH - gBoss.w - 100;
	gBoss.pos.y	= (HEIGHT-gBoss.h)/2;
	gBoss.state	= LIVING;
	gBoss.speed	 = bData[gBoss.no].speed;
	gBoss.anipat = 0;
	gBoss.anipatnum = bData[gBoss.no].anipat;
	gBoss.next 		= bData[gBoss.no].next;
	for(i=0; i<MAX_BOSSCOMMAND; i++){
		gBoss.movecommand[i] = (int)(bData[gBoss.no].movecommand/pow(10, MAX_BOSSCOMMAND-i-1)) % 10;
	}
	for(i=0; i<MAX_BOSSGUN; i++){
		gBoss.dir[i] = 0;
		gBoss.gun[i] = bData[gBoss.no].gun[i];
		gBoss.atk[i] = gGun[gBoss.gun[i]].atk;
		gBoss.shotpos[i] = bData[gBoss.no].shotpos[i];
		for(j=0; j<MAX_BOSSCOMMAND; j++){
			gBoss.shotcommand[i][j] = (int)(bData[gBoss.no].shotcommand[i]/pow(10, MAX_BOSSCOMMAND-j-1)) % 10;
		}
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
	int i;
	Pos bpos;

	MoveShot();

	/* 初回 */
	if(count == 0){
		for(i=0; i<MAX_SHOT; i++){
			if(gShot[i].type == LASER)
				gShot[i].state = DEAD;
		}
		/* Player */
		for(i=0; i<CT_NUM; i++){
			if(gChara[i].state == LIVING)
				switch(gChara[i].command[nowcommand % gChara[i].commandnum]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: //8方位移動
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
		if(tState == ADVENTURE && gBoss.state == LIVING){
			for(i=0; i<MAX_BOSSGUN; i++){
				bpos.x = gBoss.pos.x + gBoss.shotpos[i].x;
				bpos.y = gBoss.pos.y + gBoss.shotpos[i].y;
				switch(gBoss.shotcommand[i][nowcommand % MAX_BOSSCOMMAND]){
				case BOSS_SCOPE: //照準
					bpos.x = gBoss.pos.x + gBoss.shotpos[i].x; //移動後から狙いを定めるならpos->goalposに?
					bpos.y = gBoss.pos.y + gBoss.shotpos[i].y;
					gBoss.startdir[i] = (gBoss.dir[i] % 360);
					gBoss.goaldir[i] = DecideDir(BOSS, bpos);
					break;
				case BOSS_FIRE: //発射
					if(gGun[gBoss.gun[i]].type == MOB){
						MakeMob(BOSS, gGun[gBoss.gun[i]], bpos, gBoss.dir[i]);
					}
					BossFire(i, bpos, gBoss.dir[i]);
					break;
				default:
					break;
				}
			}
		}
		/* Mob */
		for(i=0; i<MAX_USEMOB; i++){
			if(gMob[i].state == LIVING)
				switch(gMob[i].command[nowcommand % gMob[i].commandnum]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: //8方位移動
					break;
				case C_SCOPE: //照準
					gMob[i].startdir = (gMob[i].dir % 360);
					gMob[i].goaldir = DecideDir(i, gMob[i].pos);
					break;
				case C_FIRE: //発射
//					if(gMob[i].id == BOSS)
//						BossFire()
					Fire(i);
					break;
				default:
					break;
				}

		}
	}
	/* カウント中 */
	else if(count <= MAX_COUNT){
		/* Player */
		for(i=0; i<CT_NUM; i++){
			if(gChara[i].state == LIVING){
				switch(gChara[i].command[nowcommand % gChara[i].commandnum]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
					gChara[i].pos = MoveChara(i, gChara[i].pos, (gChara[i].dir + gChara[i].command[nowcommand % gChara[i].commandnum] * 45) % 360, gChara[i].speed);
					break;
				case C_SCOPE: //照準
					if(count <= MAX_COUNT / 2) //2倍の速さで振り向く
						gChara[i].dir = MoveDir(gChara[i].startdir, gChara[i].goaldir);
					break;
				case C_FIRE: //発射
					if(gChara[i].gun == GUN_BUBBLE || gChara[i].gun == GUN_MILK)
						Fire(i);
					break;
				default:
					break;
				}
			}
		}
		/* BOSS */
		if(tState == ADVENTURE && gBoss.state == LIVING){
			gBoss.pos = MoveBoss(gBoss.pos, gBoss.movecommand[nowcommand % MAX_BOSSCOMMAND]);
			for(i=0; i<MAX_BOSSGUN; i++){
				bpos.x = gBoss.pos.x + gBoss.shotpos[i].x;
				bpos.y = gBoss.pos.y + gBoss.shotpos[i].y;
				switch(gBoss.shotcommand[i][nowcommand % MAX_BOSSCOMMAND]){
				case BOSS_SCOPE: //照準
					if(count <= MAX_COUNT / 2)
						for(i=0; i<MAX_BOSSGUN; i++){
							gBoss.dir[i] = MoveDir(gBoss.startdir[i], gBoss.goaldir[i]);
						}
					break;
				case BOSS_FIRE: //発射
					if(gBoss.gun[i] == GUN_BUBBLE || gBoss.gun[i] == GUN_MILK)
						BossFire(i, bpos, gBoss.dir[i]);
					break;
				default:
					break;
				}
			}
		}
	}
	/* カウント終了後のリセット */
	if(count == MAX_COUNT){
		count = -1;
		nowcommand++;
		if(nowcommand == 60)
			nowcommand = 0;
	}

	count++;
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
		dx = gBoss.pos.x + (gBoss.w / 2) - pos.x;
		dy = gBoss.pos.y + (gBoss.h / 2) - pos.y;
	}
	else{
		if(ct == BOSS){ //BOSS -> Player
			minid = 0;
			for(i=0; i<CT_NUM; i++){
				if(gChara[minid].state == DEAD)
					minid = (minid + 1) % CT_NUM;
				d[i] = DecideDistance(pos, gChara[i].pos);
				d[minid] = DecideDistance(pos, gChara[minid].pos);
				if(d[minid] >= d[i] && gChara[i].state == LIVING)
					minid = i;
			}
		}
		else{ //VS
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
	case GUN_1LASER:
	case GUN_NUCLEAR:
		MakeShot(ct, gChara[ct].dir);
		break;
	case GUN_3SHOT:
	case GUN_3LASER:
		MakeShot(ct, gChara[ct].dir);
		MakeShot(ct, (gChara[ct].dir +       20 ) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 - 20) % 360);
		break;
	case GUN_BUBBLE:
		MakeShot(ct, gChara[ct].dir + 10 * sin((double)count / (MAX_COUNT/2) * M_PI)); //2n + 1 - n
		break;
	case GUN_MILK:
		j=1;
		for(i=0; i<j; i++)
			MakeShot(ct, count * 9 + i * 360/j);
		break;
	case GUN_5SHOT:
		MakeShot(ct, gChara[ct].dir);
		MakeShot(ct, (gChara[ct].dir +        5) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 -  5) % 360);
		MakeShot(ct, (gChara[ct].dir +       10) % 360);
		MakeShot(ct, (gChara[ct].dir + 360 - 10) % 360);
		break;
	default:
		break;
	}
}

/*****************************************************************
関数名 : BossFire
機能	: 敵の玉の発射処理
引数	: gun : ボスの武器配列の添字(0〜MAX_BOSSGUN)
出力	: なし
*****************************************************************/
void BossFire(int gun, Pos pos, int dir)
{
	int i,j;
	switch(gBoss.gun[gun]){
	case GUN_1SHOT:
	case GUN_1LASER:
	case GUN_NUCLEAR:
		BossMakeShot(gun, pos, dir);
		break;
	case GUN_3SHOT:
	case GUN_3LASER:
		BossMakeShot(gun, pos, dir);
		BossMakeShot(gun, pos, (dir +       20) % 360);
		BossMakeShot(gun, pos, (dir + 360 - 20) % 360);
		break;
	case GUN_BUBBLE:
		BossMakeShot(gun, pos, dir + 10 * sin((double)count / (MAX_COUNT/2) * M_PI)); //2n + 1 - n
		break;
		case GUN_MILK:
		//BossMakeShot(gun, pos, dir + 10 * sin((double)count / (MAX_COUNT/2) * M_PI));
		j=2;
		///*
		for(i=0; i<j; i++)
			BossMakeShot(gun, pos, count * 360 / MAX_COUNT + 360 * i / j);
		//*/
		break;
	case GUN_5SHOT:
		BossMakeShot(gun, pos, dir);
		BossMakeShot(gun, pos, (dir +       15) % 360);
		BossMakeShot(gun, pos, (dir + 360 - 15) % 360);
		BossMakeShot(gun, pos, (dir +       30) % 360);
		BossMakeShot(gun, pos, (dir + 360 - 30) % 360);
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
			gShot[i].id		= ct;
			gShot[i].state	= LIVING;
			gShot[i].dir	= dir;
			gShot[i].pos.x	= gShot[i].startpos.x = gChara[ct].pos.x + S_SIZE/2;
			gShot[i].pos.y	= gShot[i].startpos.y = gChara[ct].pos.y + S_SIZE/2;
			gShot[i].type	= gGun[gChara[ct].gun].type;
			gShot[i].speed	= gGun[gChara[ct].gun].speed;
			gShot[i].size	= gGun[gChara[ct].gun].size;
			gShot[i].color	= gGun[gChara[ct].gun].color;
			break;
		}
}

/*****************************************************************
関数名 : BossMakeShot
機能	: 敵用の玉の作成
引数	: gun : ボスの武器配列の添字(0〜MAX_BOSSGUN)　dir : 角度
出力	: なし
*****************************************************************/
void BossMakeShot(int gun, Pos pos, int dir)
{
	int i;
	for(i=0; i<MAX_SHOT; i++)
		if(gShot[i].state == DEAD){
			gShot[i].id		= gun - MAX_BOSSGUN; //type == id + MAXでダメージ計算
			gShot[i].state	= LIVING;
			gShot[i].dir	= dir;
			gShot[i].pos	= gShot[i].startpos = pos;
			gShot[i].type	= gGun[gBoss.gun[gun]].type;
			gShot[i].speed	= gGun[gBoss.gun[gun]].speed;
			gShot[i].size	= gGun[gBoss.gun[gun]].size;
			gShot[i].color	= gGun[gBoss.gun[gun]].color;
			break;
		}
}

/*****************************************************************
関数名 : BossMob
機能	: サポートキャラ(Mob)の作成
引数	: gun : ボスの武器配列の添字(0〜MAX_BOSSGUN)　dir : 角度
出力	: なし
*****************************************************************/
void MakeMob(int ct, GunInfo gun, Pos pos, int dir){
	int i, j, num = 0;
	for(i=0; i<MAX_USEMOB; i++){
		if(gMob[i].state == LIVING && gMob[i].id == ct){
			num++;
			printf("%d\n", num);
		}
		else if(gMob[i].state == DEAD && num < gun.speed){
			gMob[i].state		= LIVING;
			gMob[i].no			= gun.atk;
			gMob[i].id			= ct;
			gMob[i].w			= mData[gMob[i].no].w;
			gMob[i].h			= mData[gMob[i].no].h;
			gMob[i].pos.x		= pos.x - gMob[i].w / 2;
			gMob[i].pos.y		= pos.y - gMob[i].h / 2;
			gMob[i].dir			= gMob[i].startdir = gMob[i].goaldir = dir;
			gMob[i].hp			= gMob[i].maxhp = mData[gMob[i].no].hp;
			gMob[i].atk			= gGun[mData[gMob[i].no].gun].atk;
			gMob[i].speed		= gGun[mData[gMob[i].no].gun].speed;
			gMob[i].anipat		= 0;
			gMob[i].anipatnum	= mData[gMob[i].no].anipat;
			for(j=0; j<MAX_COMMAND; j++){
				gMob[i].command[j] = (int)(mData[gMob[i].no].command/pow(10, (MAX_COMMAND-j-1))) % 10;
			}
				gMob[i].commandnum = MAX_COMMAND;
			break;
		}
	}
}

/*****************************************************************
関数名 : MoveChara
機能	: キャラクターの移動
引数	: ct : キャラの番号 pos : 移動前の座標  dir : 移動方向  speed : 速さ
出力	: なし
*****************************************************************/
Pos MoveChara(int ct, Pos pos, int dir, int speed)
{
	int i;
	Pos newpos = pos;
	newpos.x = pos.x + speed * sin(dir * M_PI /180);
	newpos.y = pos.y - speed * cos(dir * M_PI /180);

	/* 壁と衝突時 */
	if((newpos.x < 0) || (newpos.x + S_SIZE > F_WIDTH))
		newpos.x = pos.x;
	if((newpos.y < 0) || (newpos.y + S_SIZE > HEIGHT))
		newpos.y = pos.y;

	/* キャラクターと衝突時 */
	for(i=0; i<CT_NUM; i++){
		if((i != ct && newpos.x <= gChara[i].pos.x + S_SIZE && newpos.x + S_SIZE >= gChara[i].pos.x &&
			newpos.y <= gChara[i].pos.y + S_SIZE && newpos.y + S_SIZE >= gChara[i].pos.y) ||
			(tState == ADVENTURE && newpos.x <= gBoss.pos.x + gBoss.w && newpos.x + S_SIZE >= gBoss.pos.x &&
									newpos.y <= gBoss.pos.y + gBoss.h && newpos.y + S_SIZE >= gBoss.pos.y)){
			newpos = pos;
		}
	}

	return newpos;
}

/*****************************************************************
関数名 : MoveBoss
機能	: ボスの移動
引数	: pos : 移動前の座標  w,h : ボスの幅・高さ  command : コマンド
出力	: なし
*****************************************************************/
Pos MoveBoss(Pos pos, int command){
	int i;
	Pos newpos = pos;
	switch(command){
	case MOVE_UP: //上昇
		newpos.y = pos.y - 1;
		if(newpos.y < 0)
			newpos.y = pos.y;
		/* キャラクターと衝突時 */
		for(i=0; i<CT_NUM; i++){
			if(newpos.y <= gChara[i].pos.y + S_SIZE && newpos.x <= gChara[i].pos.x + S_SIZE && newpos.x + gBoss.pos.x >= gChara[i].pos.x){
				if(gChara[i].pos.y >= 1)
					gChara[i].pos.y--;
				else newpos.y = pos.y;
			}
		}
		break;
	case MOVE_DOWN: // 下降
		newpos.y = pos.y + 1;
		if(newpos.y + gBoss.h > HEIGHT)
			newpos.y = pos.y;
		for(i=0; i<CT_NUM; i++){
			if(newpos.y + gBoss.h >= gChara[i].pos.y && newpos.x <= gChara[i].pos.x + S_SIZE && newpos.x + gBoss.pos.x >= gChara[i].pos.x){
				if(gChara[i].pos.y  <= HEIGHT -1)
					gChara[i].pos.y++;
				else newpos.y = pos.y;
			}
		}
		break;
	default:
		break;
	}


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
	return (startdir + (count * (goaldir-startdir) / (MAX_COUNT/2))) % 360; //(MAX_COUNT / n)でn倍速く回転
}

/*****************************************************************
関数名 : MoveShot
機能	: 先輩こいつ玉とか移動しはじめましたよ
引数	: なし
出力	: なし
*****************************************************************/
void MoveShot()
{
	int i, j;
	int lflg; //1ならループ
	Pos newpos;
	for(i=0; i<MAX_SHOT; i++){
		if(gShot[i].type == LASER){
			lflg = 1;
			gShot[i].pos = gShot[i].startpos;
		}
		else
			lflg = 0;
		if(gShot[i].state == LIVING){
			do{
				newpos.x = gShot[i].pos.x = gShot[i].pos.x + (double)gShot[i].speed * sin(gShot[i].dir * M_PI /180);
				newpos.y = gShot[i].pos.y = gShot[i].pos.y - (double)gShot[i].speed * cos(gShot[i].dir * M_PI /180);

				/* 壁と衝突時 */
				if((newpos.x <= 0) || (newpos.x >= F_WIDTH) || (newpos.y <= 0) || (newpos.y >= HEIGHT)){
					if(lflg == 1)
						lflg = 0;
					else
						gShot[i].state = DEAD;
				}
				/* 玉とキャラとの衝突時 */
				if(tState != ADVENTURE){
					/* Player -> Player */
					for(j=0; j<CT_NUM; j++){
						if(j != gShot[i].id && gChara[j].state == LIVING &&
								newpos.x >= gChara[j].pos.x && newpos.x <= gChara[j].pos.x + S_SIZE &&
								newpos.y >= gChara[j].pos.y && newpos.y <= gChara[j].pos.y + S_SIZE){ //キャラの当たり判定
							if(lflg == 1)
								lflg = 0;
							else{
								gShot[i].state = DEAD;
							}
							gChara[j].hp -= gChara[gShot[i].id].atk;
							if( gChara[j].hp <= 0 ){
								gChara[j].state = DEAD;
								restplayer--;
							}
							if(restplayer == 1){
								if(gChara[0].state == LIVING) //0Pが生きてたら勝ち
									win = 1;
								else win = 0;
								mState = MAIN_RESULT;
							}
						}
					}
				}
				else{ //ADVENTURE
					/* Player -> Boss */
					if(gShot[i].id >= 0){
						if(gBoss.state == LIVING &&
								newpos.x >= gBoss.pos.x && newpos.x <= gBoss.pos.x + gBoss.w &&
								newpos.y >= gBoss.pos.y && newpos.y <= gBoss.pos.y + gBoss.h){ //当たり判定
							if(lflg == 1)
								lflg = 0;
							else{
								gShot[i].state = DEAD;
							}
							gBoss.hp -= gChara[gShot[i].id].atk;
							if(gBoss.hp <= 0 ){
								if(gBoss.next == DELETE){
									gBoss.state = DEAD;
									win = 1;
									mState = MAIN_RESULT;
								}
								else{
									gBoss.no = gBoss.next;
									InitAdventure();
								}
							}
						}
					}
					/* Boss -> Player */
					else{
						for(j=0; j<CT_NUM; j++){
							if(gChara[j].state == LIVING &&
									newpos.x >= gChara[j].pos.x && newpos.x <= gChara[j].pos.x + S_SIZE &&
									newpos.y >= gChara[j].pos.y && newpos.y <= gChara[j].pos.y + S_SIZE){ //キャラの当たり判定
								if(lflg == 1)
									lflg = 0;
								else{
									gShot[i].state = DEAD;
								}
								gChara[j].hp -= gBoss.atk[gShot[i].id + MAX_BOSSGUN];
								if( gChara[j].hp <= 0 ){
									gChara[j].state = DEAD;
									restplayer--;
								}
								if(restplayer == 0){
									win = 0;
									mState = MAIN_RESULT; //負け
								}
							}
						}
					}
				}
			} while(lflg);
		}
	}
}

/* end of system.c */
