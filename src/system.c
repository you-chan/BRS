#include <time.h>
#include <SDL/SDL.h>
#include "common.h"
#include "func.h"

static Pos DecideGoal(Pos pos, int command, int dir);
static int DecideDir(int ct);
static int DecideDistance(Pos p1, Pos p2);
static Pos MoveChara(Pos pos, Pos startpos, Pos goalpos, int dir);
static Pos MoveShot(int ct, Pos pos, int dir);
static int MoveDir(int startdir, int goaldir);
static void CheckWinner();

int restplayer; /* 残り人数 */

/*****************************************************************
関数名 : InitTitle
機能	: タイトルでの初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitTitle()
{
	gState = GAME_TITLE;
}

/*****************************************************************
関数名 : InitMain
機能	: メインでの初期化処理
引数	: なし
出力	: なし
*****************************************************************/
void InitMain()
{
    /* 乱数初期化 */
    srand(time(NULL));
	int i, j;

	mState = MAIN_COMMAND;
	cState = COMMAND_DIR;
	CT_NUM = restplayer = CT_MAXNUM;
	for(i=0; i<CT_NUM; i++){ //座標, 角度の決定
		gChara[i].pos.x = (i + 1) * WIDTH  / (CT_NUM + 1);
		gChara[i].pos.y = rand() % (HEIGHT-S_SIZE);
		gChara[i].dir = rand() % 360;
		gChara[i].shotflg = 0;
		gChara[i].state = LIVING;
		for(j=0; j<MAX_COMMAND; j++)
			gChara[i].command[j] = -1;
	}

	count = 0;
	nowcommand = 0;
	gCommand.dir = 0;
	gCommand.shot = C_SCOPE; //照準から選択できるように
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
	/* 初回 */
	if(count == 0){
		for(i=0; i<CT_NUM; i++){
			if(gChara[i].state == LIVING){
				switch(gChara[i].command[nowcommand]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: //8方位移動
					gChara[i].startpos = gChara[i].pos;
					gChara[i].goalpos =  DecideGoal(gChara[i].pos, gChara[i].command[nowcommand], gChara[i].dir);
					break;
				case C_SCOPE: //照準
					gChara[i].startdir = gChara[i].dir % 360;
					gChara[i].goaldir = DecideDir(i) % 360;
					break;
				case C_SHOT: //発射
					gChara[i].shotflg = 1;
					gChara[i].shotpos.x = gChara[i].pos.x + S_SIZE / 2;
					gChara[i].shotpos.y = gChara[i].pos.y + S_SIZE / 2;
					break;
				default:
					break;
				}
			}
		}
	}
	else if(count <= MAX_COUNT){
		for(i=0; i<CT_NUM; i++){
				switch(gChara[i].command[nowcommand]){
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
					if(gChara[i].state == LIVING)
						gChara[i].pos = MoveChara(gChara[i].pos, gChara[i].startpos, gChara[i].goalpos, (gChara[i].dir + gChara[i].command[nowcommand] * 45) % 360);
					break;
				case C_SCOPE: //照準
					if(gChara[i].state == LIVING && count <= MAX_COUNT / 2)
						gChara[i].dir = MoveDir(gChara[i].startdir, gChara[i].goaldir);
					break;
				case C_SHOT: //発射
					if(gChara[i].shotflg == 1)
						gChara[i].shotpos = MoveShot(i, gChara[i].shotpos, gChara[i].dir);
					break;
				default:
					break;
				}
		}
	}
	else{ //カウント終了後のリセット
		for(i=0; i<CT_NUM; i++)
			gChara[i].shotflg = 0;
		count = -1;
		nowcommand++;
		if(nowcommand == MAX_COMMAND)
			nowcommand = 0;
	}
	count++;
}

/*****************************************************************
関数名 : DecideGoal
機能	: 船(仮)が移動する際の目的地の座標を求める
引数	: pos : 移動前の座標  command : 8方位の移動方向  dir:船(仮)の角度
出力	: 目的地の座標
*****************************************************************/
Pos DecideGoal(Pos pos, int command, int dir)
{
	Pos goalpos = {0, 0};
	goalpos.x = pos.x + 50 * sin((dir + command * 45) * M_PI /180);
	goalpos.y = pos.y - 50 * cos((dir + command * 45) * M_PI /180);
	return goalpos;
}

/*****************************************************************
関数名 : DecideDir
機能	: 変更後の角度の決定
引数	: ct : 角度を変更する船(仮)の番号
出力	: 変更後の角度
*****************************************************************/
int DecideDir(int ct)
{
	/* 2点間の距離が最も近い船(仮)を決め、それとの角度を求める */
	int i, d[CT_NUM], minid = ct, goaldir = 0;
	for(i=0; i<CT_NUM; i++){
			d[i] = DecideDistance(gChara[ct].pos, gChara[i].pos);
			d[minid] = DecideDistance(gChara[ct].pos, gChara[minid].pos);
		if(((d[minid] > d[i] && d[i] != 0) || d[minid] == 0) && gChara[i].state == LIVING){
			minid = i;
		}
	}

	int dx = gChara[minid].pos.x - gChara[ct].pos.x; //距離の差
	int dy = gChara[minid].pos.y - gChara[ct].pos.y;
	goaldir = (atan2(dy, dx) * 180 / M_PI + 450);

	return goaldir;
}

/*****************************************************************
関数名 : DecideDistance
機能	: 2点間の距離の測定
引数	: p1, p2 : 各船(仮)の座標
出力	: 距離の2乗
*****************************************************************/
int DecideDistance(Pos p1, Pos p2)
{
	int d = 0;
	d = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	return d;
}

/*****************************************************************
関数名 : MoveChara
機能	: 船(仮)の移動
引数	: pos : 移動前の座標
出力	: なし
*****************************************************************/
Pos MoveChara(Pos pos, Pos startpos, Pos goalpos, int dir)
{
	Pos newpos;
	newpos.x = startpos.x + count * (goalpos.x - startpos.x) / MAX_COUNT;
	newpos.y = startpos.y + count * (goalpos.y - startpos.y) / MAX_COUNT;
	/* 壁と衝突時 */
	if((newpos.x < 0) || (newpos.x + S_SIZE > WIDTH))
		newpos.x = pos.x;
	if((newpos.y < 0) || (newpos.y + S_SIZE > HEIGHT))
		newpos.y = pos.y;
	return newpos;
}

/*****************************************************************
関数名 : MoveShot
機能	: 先輩こいつ玉とか移動しはじめましたよ
引数	: ct : 発射した船(仮)の番号  pos : 玉の座標  dir : 玉の角度
出力	: 移動後の座標
*****************************************************************/
Pos MoveShot(int ct, Pos pos, int dir)
{
	int i;
	Pos newpos;
	newpos.x = pos.x + 20 * 50 / MAX_COUNT * sin(dir * M_PI /180);
	newpos.y = pos.y - 20 * 50 / MAX_COUNT * cos(dir * M_PI /180);
	/* 壁と衝突時 */
	if((newpos.x < 0) || (newpos.x > WIDTH) || (newpos.y < 0) || (newpos.y > HEIGHT)){
		gChara[ct].shotflg = 0;
		return pos;
	}
	/* 玉と船(仮)と衝突時 */
	for(i=0; i<CT_NUM; i++){
		if(i != ct && gChara[i].state == LIVING && gChara[ct].shotflg &&
				newpos.x >= gChara[i].pos.x && newpos.x <= gChara[i].pos.x + S_SIZE &&
				newpos.y >= gChara[i].pos.y && newpos.y <= gChara[i].pos.y + S_SIZE){
			gChara[ct].shotflg = 0;
			gChara[i].state = DEAD;
			restplayer--;
			if(restplayer == 1){
				CheckWinner();
				mState = MAIN_RESULT;
			}

			return pos;
		}
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
	return startdir + (count * (goaldir-startdir) / (MAX_COUNT/2)); //(MAXCOUNT / n)でn倍速く回転
}

void CheckWinner()
{
	int i;
	for(i=0; i<CT_NUM; i++)
		if(gChara[i].state == LIVING)
			winner = i;
}

/* end of system.c */
