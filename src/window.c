/*****************************************************************
ファイル名	: window.c
機能			: ユーザーインターフェース処理
*****************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "common.h"
#include "func.h"

static char BRSImgFile[]  = "img/BRS.png";
static char TitleImgFile[]  = "img/title.png";
static char CountImgFile[]  = "img/count.png";
static char ResultImgFile[2][16] = {"img/win.png", "img/lose.png"};
static char NumberImgFile[] = {"img/number.png"};
static char ShipImgFile[4][16]  =
	{"img/ship01.png", "img/ship02.png", "img/ship03.png", "img/ship04.png"};
static char BossImgFile[MAX_BOSS][16] = {"img/boss00.png", "img/boss01.png", "img/boss02.png", "img/boss03.png", "img/boss04.png"};
static char MobImgFile[MAX_MOB][16] =	{"img/mob00.png", "img/mob01.png", "img/mob02.png", "img/mob03.png"};
static char GunImgFile[] = "img/gun.png";
static char ArmorImgFile[] = "img/armor.png";
static char ArrowImgFile[] = "img/arrow.png";
static char BombImgFile[] = "img/bomb.png";
static char ChooseImgFile[] = "img/choose.png";
static char CommandImgFile[]    = "img/command01.png";
static char MiniCommandImgFile[]    = "img/command02.png";
static char InfoImgFile[] = "img/info.png";

static SDL_Surface *gMainWindow;
static SDL_Surface *BRSWindow;
static SDL_Surface *TitleWindow;
static SDL_Surface *CountWindow;
static SDL_Surface *ResultWindow[2];
static SDL_Surface *NumWindow;
static SDL_Surface *ShipWindow[MAX_CT];
static SDL_Surface *BossWindow[MAX_BOSS];
static SDL_Surface *MobWindow[MAX_MOB];
static SDL_Surface *GunWindow;
static SDL_Surface *ArmorWindow;
static SDL_Surface *ArrowWindow;
static SDL_Surface *BombWindow;
static SDL_Surface *ChooseWindow;
static SDL_Surface *CommandWindow;
static SDL_Surface *MiniCommandWindow;
static SDL_Surface *InfoWindow;
static SDL_Joystick *joystick; //ジョイスティックを特定・利用するための構造体

/* 画像転送用 */
typedef struct {
    SDL_Rect src;
    SDL_Rect dst;
} Rect;

static void DrawCommand();
static void DrawShip();
static void DrawCount();
static void DrawShot();
static void DrawMainCommand();
static void DrawBomb();
static void DrawResult();
static void WindowEvent(SDLKey key);

#define MAX_SET 4
int commandset[MAX_SET] = {189189, 899899, 898989, 849795};

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindow()
{
    int i;
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}

	/* 画像の読み込み */
	BRSWindow = IMG_Load(BRSImgFile);
    if(BRSWindow == NULL){
        printf("failed to open BRS image.");
        return -1;
    }

    TitleWindow = IMG_Load(TitleImgFile);
    if(TitleWindow == NULL){
        printf("failed to open title image.");
        return -1;
    }

    CountWindow = IMG_Load(CountImgFile);
    if(CountWindow == NULL){
        printf("failed to open count image.");
        return -1;
    }

    for(i=0; i<2; i++){
    	ResultWindow[i] = IMG_Load(ResultImgFile[i]);
    	if(ResultWindow[i] == NULL){
    		printf("failed to open result image.\n");
    		return -1;
    	}
    }

    NumWindow = IMG_Load(NumberImgFile);
    	if(NumWindow == NULL){
        		printf("failed to open num image.");
        		return -1;
        	}

    for(i=0; i < MAX_CT; i++){
		ShipWindow[i] = IMG_Load(ShipImgFile[i % 4]);
		if(ShipWindow[i] == NULL){
			printf("failed to open ship image.");
			return -1;
		}
    }

    for(i=0; i<MAX_BOSS; i++){
    	BossWindow[i] = IMG_Load(BossImgFile[i]);
    	if(BossWindow[i] == NULL){
    		printf("failed to open boss image.");
    		return -1;
    	}
    }

    for(i=0; i<MAX_MOB; i++){
    	MobWindow[i] = IMG_Load(MobImgFile[i]);
    	if(MobWindow[i] == NULL){
    		printf("failed to open mob image.");
    		return -1;
    	}
    }

    GunWindow = IMG_Load(GunImgFile);
    	if(GunWindow == NULL){
    		printf("failed to open gun image.");
    		return -1;
    	}

    ArmorWindow = IMG_Load(ArmorImgFile);
    	if(ArmorWindow == NULL){
    		printf("failed to open armor image.");
    	    return -1;
    	}

    BombWindow = IMG_Load(BombImgFile);
        if(BombWindow == NULL){
        	printf("failed to open bomb image.");
        	return -1;
        }

    ArrowWindow = IMG_Load(ArrowImgFile);
        if(ArrowWindow == NULL){
        	printf("failed to open arrow image.");
        	return -1;
        }

    ChooseWindow = IMG_Load(ChooseImgFile);
   		if(ChooseWindow == NULL){
    		printf("failed to open choose image.");
    		return -1;
    	}

	CommandWindow = IMG_Load(CommandImgFile);
    if(CommandWindow == NULL){
        printf("failed to open command image.");
        return -1;
    }

	MiniCommandWindow = IMG_Load(MiniCommandImgFile);
    if(CommandWindow == NULL){
        printf("failed to open minicommand image.");
        return -1;
    }

    InfoWindow = IMG_Load(InfoImgFile);
    	if(InfoWindow == NULL){
        		printf("failed to open info image.");
        		return -1;
        }

	/* メインウインドウの作成 */
	if((gMainWindow = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
	/* ウインドウのタイトルをセット */
	SDL_WM_SetCaption("BattRoutinShip",NULL);

	// 接続されているジョイスティックの名前を表示
	for(i=0;i<SDL_NumJoysticks();i++){
		if(strcmp(SDL_JoystickName(0), "ST LIS3LV02DL Accelerometer") != 0)
			printf("Connecting %s\n ", SDL_JoystickName(i));
	}

	if(SDL_NumJoysticks() > 0 && strcmp(SDL_JoystickName(0),
										"ST LIS3LV02DL Accelerometer") != 0)
		joystick=SDL_JoystickOpen(0);// ジョイスティックを開く
	SDL_JoystickEventState(SDL_ENABLE);// ジョイスティック関連のイベントを取得可能にする

	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0xffffff);
	SDL_Flip(gMainWindow);
	return 0;
}

/*****************************************************************
関数名 : DrawTitle
機能	: タイトル画面の描画
引数	: なし
出力	: なし
*****************************************************************/
void DrawTitle()
{
	int i;
	Rect rect = {{0, 0, WIDTH, HEIGHT}, {0, 0}};
	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0xffffff);
	SDL_BlitSurface(BRSWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = rect.src.y = 0;
	rect.src.w = 360;
	rect.src.h = 60;
	rect.dst.x = (WIDTH - 360)/2;
	for(i=0; i<MAX_TITLE; i++){
		rect.src.y = i * 60;
		rect.dst.y = HEIGHT*((i*2)+1)/8+120;
		SDL_BlitSurface(TitleWindow, &(rect.src), gMainWindow, &(rect.dst));
	}

	/*選択中でないタブを灰色に*/
	boxColor(gMainWindow, (WIDTH-360)/2, HEIGHT*(((tState+1)*2+1)%6)/8+120, (WIDTH-360)/2+360, HEIGHT*(((tState+1)*2+1)%6)/8+180, 0x00000060);
	boxColor(gMainWindow, (WIDTH-360)/2, HEIGHT*(((tState*2)+5)%6)/8+120, (WIDTH-360)/2+360, HEIGHT*(((tState*2)+5)%6)/8+180, 0x00000060);
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : DrawBossEdit
機能	: ボス編集画面の描画
引数	: なし
出力	: なし
*****************************************************************/
void DrawBossEdit()
{
	Rect rect = {{0, 0, 0, 0}, {0, 0}};

	/* 背景を灰色にする */
	SDL_FillRect(gMainWindow,NULL,0x808080);
	boxColor(gMainWindow, F_WIDTH, 0, WIDTH, HEIGHT, 0x000000FF);

	rect.src.x = 100 * gBoss.no;
	rect.src.y = 0;
	rect.src.w = 100;
	rect.src.h = 100;
	rect.dst.x = (F_WIDTH - 100)/2;
	rect.dst.y = (HEIGHT - 100) / 2;

	SDL_BlitSurface(NumWindow, &(rect.src), gMainWindow, &(rect.dst));
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : DrawEdit
機能	: 装備編集画面の描画
引数	: なし
出力	: なし
*****************************************************************/
void DrawEdit()
{
	Rect rect = {{0, 0, 0, 0}, {0, 0}};

	/* 背景を灰色にする */
	SDL_FillRect(gMainWindow,NULL,0x808080);
	boxColor(gMainWindow, F_WIDTH, 0, WIDTH, HEIGHT, 0x000000FF);

	rect.src.x = 0;
	rect.src.y = 0;
	rect.src.w = 256;
	rect.src.h = 768;
	rect.dst.x = F_WIDTH;
	rect.dst.y = 0;
	SDL_BlitSurface(InfoWindow, &(rect.src), gMainWindow, &(rect.dst));

	rect.src.x = 0;
	rect.src.y = 0;
	rect.src.w = 100;
	rect.src.h = 320;
	rect.dst.x = F_WIDTH / 8;
	rect.dst.y = 5 * HEIGHT /8 - 110;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.dst.x = 5 * F_WIDTH /8;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));

	/*選択している装備に枠を付ける*/
	boxColor(gMainWindow, F_WIDTH*(eState*2+1)/8-5, 5*HEIGHT/8-5, F_WIDTH*(eState*2+1)/8+C_SIZE+5, 5*HEIGHT/8+C_SIZE+5, 0xFF0000FF);

	rect.src.x = gChara[0].gun*100;
	rect.src.y = 0;
	rect.src.w = rect.src.h = 100;
	rect.dst.x = F_WIDTH /8;
	rect.dst.y = 5 * HEIGHT /8;
	SDL_BlitSurface(GunWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = gChara[0].armor*100;
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(ArmorWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = 100;
	rect.dst.x = 5 * F_WIDTH /8;
	SDL_BlitSurface(ChooseWindow, &(rect.src), gMainWindow, &(rect.dst));

	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : DrawMain
機能	: メイン画面の描画
引数	: なし
出力	: なし
*****************************************************************/
void DrawMain()
{
	SDL_FillRect(gMainWindow,NULL,0x101010);
	if(mState == MAIN_RESULT) //結果画面
		DrawResult();
	else
		DrawShip();

	if(mState == MAIN_COMMAND) //コマンド入力
		DrawCommand();
	else if(mState == MAIN_COUNT){
		DrawMainCommand();
		DrawCount();
	}
	else if(mState == MAIN_MOVE){ //コマンド適用
		DrawShot();
		DrawMainCommand();
		DrawBomb();
	}
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : Drawcommand
機能	: コマンド入力中の画面を表示する
引数	: なし
出力	: なし
*****************************************************************/
void DrawCommand()
{
	int i;
	Rect rect = {{0, 0, 0, 0}, {0, 0}};

	/* 選択中のコマンドの描画 */
	boxColor(gMainWindow, 0, 0, F_WIDTH, HEIGHT, 0x00000040);

	rect.src.x = 256;
	rect.src.y = 0;
	rect.src.w = 256;
	rect.src.h = HEIGHT;
	rect.dst.x = F_WIDTH;
	rect.dst.y = 0;
	SDL_BlitSurface(InfoWindow, &(rect.src), gMainWindow, &(rect.dst));

	rect.src.x = rect.src.y = 0;
	rect.src.w = 100;
	rect.src.h = 320;
	rect.dst.x = 2.5 * F_WIDTH /8;
	rect.dst.y = 5 * HEIGHT / 8 - 110;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.dst.x = F_WIDTH/8;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));

	/*選択しているコマンドに枠を付ける*/
	boxColor(gMainWindow, F_WIDTH*(cState*1.5+1)/8-5, 5*HEIGHT/8-5, F_WIDTH*(cState*1.5+1)/8+C_SIZE+5, 5*HEIGHT/8+C_SIZE+5, 0xFF0000FF);

	rect.src.x = gCommand.dir * C_SIZE;
	rect.src.y = 0;
	rect.src.w = rect.src.h = C_SIZE;
	rect.dst.x = 1 * F_WIDTH /8;
	rect.dst.y = 5 * HEIGHT /8;
	SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = gCommand.gun * C_SIZE;
	rect.dst.x = 2.5 * F_WIDTH /8;
	SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = 0;
	rect.dst.x = 4 * F_WIDTH /8;
	SDL_BlitSurface(ChooseWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = 100;
	rect.dst.x = 5.5 * F_WIDTH /8;
	SDL_BlitSurface(ChooseWindow, &(rect.src), gMainWindow, &(rect.dst));

	/* 選択したコマンドの描画 */
	for(i=0; i<MAX_COMMAND; i++){
		if(gChara[0].command[gChara[0].ctype][i] != -1){
			rect.src.x = gChara[0].command[gChara[0].ctype][i] * C_SIZE;
			rect.src.y = 0;
			rect.src.w = rect.src.h = C_SIZE;
			if(gChara[0].cnum[gChara[0].ctype] <= 4)
				rect.dst.x = F_WIDTH / 4 + C_SIZE * i;
			else
				rect.dst.x = F_WIDTH / 4 + C_SIZE * i * 4 / gChara[0].cnum[gChara[0].ctype];//MAX_COMMAND;
			rect.dst.y = HEIGHT / 8;
			SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
		}
	}

	if(gChara[0].ctype == 1){
		/* 選択したコマンドの描画 */
		for(i=0; i<MAX_COMMAND; i++){
			if(gChara[0].command[0][i] != -1){
				rect.src.x = gChara[0].command[0][i] * C_SIZE/2;
				rect.src.y = 0;
				rect.src.w = rect.src.h = C_SIZE/2;
				if(gChara[0].cnum[0] <= 4)
					rect.dst.x = F_WIDTH + 20 + C_SIZE/2 * i;
				else
					rect.dst.x = F_WIDTH + 20 + C_SIZE/2 * i * 4 / gChara[0].cnum[0];//MAX_COMMAND;
				rect.dst.y = 5 * HEIGHT / 8;
				SDL_BlitSurface(MiniCommandWindow, &(rect.src), gMainWindow, &(rect.dst));
			}
		}
	}
}

/*****************************************************************
関数名 : DrawShip
機能	: 船(仮)、mob、ボスを描画する
引数	: なし
出力	: なし
*****************************************************************/
void DrawShip()
{
	int i;
	Rect rect = {{0, 0, F_WIDTH, HEIGHT}, {0, 0}};

	for(i=0; i<CT_NUM; i++){
		if(gChara[i].state == LIVING){
			rect.src.x = (((gChara[i].dir % 360) % 45) / 5 * S_SIZE);
			rect.src.y = ((gChara[i].dir % 360) / 45) * S_SIZE;
			rect.src.w = rect.src.h = S_SIZE;
			rect.dst.x = gChara[i].pos.x;
			rect.dst.y = gChara[i].pos.y;
			SDL_BlitSurface(ShipWindow[i], &(rect.src), gMainWindow, &(rect.dst));

			//hpを表す四角形を描画
			if(gChara[i].hp > gChara[i].maxhp/2)
				boxColor(gMainWindow, gChara[i].pos.x - S_SIZE / 2 , gChara[i].pos.y + S_SIZE,
						gChara[i].pos.x - S_SIZE / 2 + S_SIZE * 2 * (double)(gChara[i].hp) / gChara[i].maxhp, gChara[i].pos.y + S_SIZE + 10, 0x00ff0080);
			else
				boxColor(gMainWindow, gChara[i].pos.x - S_SIZE / 2 , gChara[i].pos.y + S_SIZE,
						gChara[i].pos.x - S_SIZE / 2 + S_SIZE * 2 * (double)(gChara[i].hp) / gChara[i].maxhp, gChara[i].pos.y + S_SIZE + 10, 0xff000080);
			rectangleColor(gMainWindow, gChara[i].pos.x - S_SIZE / 2, gChara[i].pos.y + S_SIZE,
					gChara[i].pos.x + S_SIZE * 3 / 2, gChara[i].pos.y + S_SIZE + 10 , 0xff000080);
		}
	}

	for(i=0; i<MAX_USEMOB; i++){
		if(gMob[i].state == LIVING){
			if(gMob[i].type == M_CHARA){
			//hpを表す四角形を描画
				if(gMob[i].hp > gMob[i].maxhp/2)
					boxColor(gMainWindow, gMob[i].pos.x - 50 , gMob[i].pos.y + gMob[i].h,
							gMob[i].pos.x - 50 + (gMob[i].w + 100) * (double)(gMob[i].hp) / gMob[i].maxhp, gMob[i].pos.y + gMob[i].h + 10, 0x00ff0080);
				else
					boxColor(gMainWindow, gMob[i].pos.x - 50 , gMob[i].pos.y + gMob[i].h,
							gMob[i].pos.x - 50 + (gMob[i].w +100) * (double)(gMob[i].hp) / gMob[i].maxhp, gMob[i].pos.y + gMob[i].h + 10, 0xff000080);
				rectangleColor(gMainWindow, gMob[i].pos.x - 50, gMob[i].pos.y + gMob[i].h,
						gMob[i].pos.x + gMob[i].w + 50, gMob[i].pos.y + gMob[i].h + 10 , 0xff000080);

				if(gMob[i].anipatnum <= 1)
					rect.src.x = 0;
				else{
					gMob[i].anipat = (gMob[i].anipat + 1) % (gMob[i].anipatnum * 2);
					rect.src.x = gMob[i].anipat/2 * gMob[i].w;
				}
				rect.src.y = 0;
			}
			else if(gMob[i].type == M_SHOT){
				rect.src.x = (((gMob[i].dir % 360) % 45) / 5 * gMob[i].w);
				rect.src.y = ((gMob[i].dir % 360) / 45) * gMob[i].h;
			}
			rect.src.w = gMob[i].w;
			rect.src.h = gMob[i].h;
			rect.dst.x = gMob[i].pos.x;
			rect.dst.y = gMob[i].pos.y;
			SDL_BlitSurface(MobWindow[gMob[i].no], &(rect.src), gMainWindow, &(rect.dst));

		}
	}

	if(tState == ADVENTURE && gBoss.state == LIVING){
		/*ボスの描画*/
		if(gBoss.anipatnum <= 1)
			rect.src.x = 0;
		else{
			gBoss.anipat = (gBoss.anipat + 1) % (gBoss.anipatnum * 2);
			rect.src.x = gBoss.anipat/2 * gBoss.w;
		}
		rect.src.y = 0;
		rect.src.w = gBoss.w;
		rect.src.h = gBoss.h;
		rect.dst.x = gBoss.pos.x;
		rect.dst.y = gBoss.pos.y;
		SDL_BlitSurface(BossWindow[gBoss.no], &(rect.src), gMainWindow, &(rect.dst));

		//hpを表す四角形を描画
		if(gBoss.hp > gBoss.maxhp/2)
			boxColor(gMainWindow, gBoss.pos.x - 100 , gBoss.pos.y + gBoss.h,
					gBoss.pos.x - 100 + (gBoss.w + 200) * (double)(gBoss.hp) / gBoss.maxhp, gBoss.pos.y + gBoss.h + 10, 0x00ff0080);
		else
			boxColor(gMainWindow, gBoss.pos.x - 100 , gBoss.pos.y + gBoss.h,
					gBoss.pos.x - 100 + (gBoss.w +200) * (double)(gBoss.hp) / gBoss.maxhp, gBoss.pos.y + gBoss.h + 10, 0xff000080);
		rectangleColor(gMainWindow, gBoss.pos.x - 100, gBoss.pos.y + gBoss.h,
				gBoss.pos.x + gBoss.w + 100, gBoss.pos.y + gBoss.h + 10 , 0xff000080);
	}

	// 1Pの区別用
	if(gChara[0].state == LIVING)
		filledCircleColor(gMainWindow, gChara[0].pos.x+S_SIZE/2, gChara[0].pos.y+S_SIZE/2, 10, 0xFFFFFFF); //自分を写す
}

/*****************************************************************
関数名 : DrawCount
機能	: カウントダウンの描画処理
引数	: なし
出力	: なし
*****************************************************************/
void DrawCount()
{
	Rect rect = {{0, 0, 0, 0}, {0, 0}};
	startcount--;
	rect.src.x = ((int)startcount / FPS)*300;
	rect.src.y = 0;
	rect.src.w = CountWindow->w/3;
	rect.src.h = CountWindow->h;
	rect.dst.x = (F_WIDTH - rect.src.w) / 2;
	rect.dst.y = (F_WIDTH - rect.src.h) / 2;
	SDL_BlitSurface(CountWindow, &(rect.src), gMainWindow, &(rect.dst));
	if(startcount == 0)
		mState = MAIN_MOVE;	//動作に移行
}

/*****************************************************************
関数名 : DrawShot
機能	: 先輩こいつ玉とか描画しはじめましたよ
引数	: なし
出力	: なし
*****************************************************************/
void DrawShot()
{
	int i, j;
	double x, y;
	Pos spos;
	for(i=0; i<MAX_SHOT; i++){
		if(gShot[i].state == LIVING){
			if(gShot[i].type == LASER){
				spos.x = gShot[i].startpos.x; //発射座標
				spos.y = gShot[i].startpos.y;
				if(gShot[i].id != BOSS){
					lineColor(gMainWindow, gChara[gShot[i].id].pos.x+S_SIZE/2, gChara[gShot[i].id].pos.y+S_SIZE/2+1,
								gShot[i].pos.x, gShot[i].pos.y+1, gShot[i].color);
				}
				else{
					for(j= -gShot[i].size/2; j<gShot[i].size/2+1; j++){
						x = j; y = (double)j*j/gShot[i].size;
						lineColor(gMainWindow, spos.x + (x * cos(-gShot[i].dir * M_PI / 180) - y * sin(-gShot[i].dir * M_PI / 180)),
												spos.y - (x * sin(-gShot[i].dir * M_PI / 180) + y * cos(-gShot[i].dir * M_PI / 180)),
												gShot[i].pos.x + (x * cos(-gShot[i].dir * M_PI / 180) - gShot[i].size / 4 * sin(-gShot[i].dir * M_PI / 180)),
												gShot[i].pos.y - (x * sin(-gShot[i].dir * M_PI / 180) + gShot[i].size / 4 * cos(-gShot[i].dir * M_PI / 180)),
												gShot[i].color);
					}
				}
			}
			else{
				filledCircleColor(gMainWindow, gShot[i].pos.x, gShot[i].pos.y, gShot[i].size, gShot[i].color);
			}
		}
	}
}

/*****************************************************************
関数名 : DrawMainCommand
機能	: 入力したコマンドを対戦中に表示する
引数	: なし
出力	: なし
*****************************************************************/
void DrawMainCommand(){
	int i, j, x, color;
	Rect rect = {{0, 0, 0, 0}, {0, 0}};
	if(gChara[0].cnum[gChara[0].ctype] <= 4)
		x = F_WIDTH + 30 + C_SIZE/2 * gChara[0].cnum[gChara[0].ctype];
	else
		x = F_WIDTH + 30 + C_SIZE/2 * (gChara[0].cnum[gChara[0].ctype] - 1) * 4 / gChara[0].cnum[gChara[0].ctype] + C_SIZE/2;
	if(cflg == C_STOP)
		color = 0xFF0000FF;//赤
	else
		color = 0x00FF00FF;//緑
	boxColor(gMainWindow, F_WIDTH+10, 5*HEIGHT/8+gChara[0].ctype*C_SIZE-10, x, 5*HEIGHT/8+gChara[0].ctype*C_SIZE+C_SIZE/2+10, color);
	/* 選択したコマンドの描画 */
	for(i=0; i<2; i++){
		for(j=0; j<MAX_COMMAND; j++){
			if(gChara[0].command[i][j] != -1){
				rect.src.x = gChara[0].command[i][j] * C_SIZE/2;
				rect.src.y = 0;
				rect.src.w = rect.src.h = C_SIZE/2;
				if(gChara[0].cnum[i] <= 4)
					rect.dst.x = F_WIDTH + 20 + C_SIZE/2 * j;
				else
					rect.dst.x = F_WIDTH + 20 + C_SIZE/2 * j * 4 / gChara[0].cnum[i];
				rect.dst.y = 5 * HEIGHT / 8 + i * C_SIZE;
				SDL_BlitSurface(MiniCommandWindow, &(rect.src), gMainWindow, &(rect.dst));
			}
		}
	}
}


/*****************************************************************
関数名 : DrawResult
機能	: 爆発を表示する
引数	: pos : 爆破箇所
出力	: なし
*****************************************************************/
void DrawBomb(){
	int i;
	Rect rect = {{0, 0, 0, 0}, {0, 0}};
	for(i=0; i<CT_NUM; i++){
		if(gChara[i].dcount < MAX_DCOUNT && gChara[i].state == DEAD){
			rect.src.x = (gChara[i].dcount/ 2 % 8) * 300;
			rect.src.y = 0;
			rect.src.w = 300;
			rect.src.h = 300;
			rect.dst.x = gChara[i].pos.x + S_SIZE / 2 - rect.src.w / 2;
			rect.dst.y = gChara[i].pos.y + S_SIZE / 2 - rect.src.h / 2;
			SDL_BlitSurface(BossWindow[1], &(rect.src), gMainWindow, &(rect.dst));
		}
	}
	if(gBoss.dcount < MAX_DCOUNT && gBoss.state == DEAD){
		rect.src.x = 0;
		rect.src.y = 0;
		rect.src.w = BombWindow->w;
		rect.src.h = BombWindow->h;
		rect.dst.x = gBoss.pos.x + gBoss.w / 2 - rect.src.w / 2;
		rect.dst.y = gBoss.pos.y + gBoss.h / 2 - rect.src.h / 2;
		SDL_BlitSurface(BombWindow, &(rect.src), gMainWindow, &(rect.dst));
	}
}

/*****************************************************************
関数名 : DrawResult
機能	: 結果画面を表示する
引数	: なし
出力	: なし
*****************************************************************/
void DrawResult()
{
	Rect rect = {{0, 0, F_WIDTH, HEIGHT}, {0, 0}};
	if(win == 1)
		SDL_BlitSurface(ResultWindow[0], &(rect.src), gMainWindow, &(rect.dst));
	else
		SDL_BlitSurface(ResultWindow[1], &(rect.src), gMainWindow, &(rect.dst));
}

/*****************************************************************
関数名 : InputKey
機能	: キー入力の受付
引数	: なし
出力	: なし
*****************************************************************/
void InputKey()
{
	SDL_Event event;
	if(SDL_PollEvent(&event)){
		switch(event.type){
        	case SDL_QUIT:
        		gState = GAME_END;
        		break;
        	case SDL_KEYDOWN:
        	//case SDL_KEYUP:
        		WindowEvent(event.key.keysym.sym);
        		break;
        	/* ジョイスティックはボタンをキーボード入力に変換して適用 */
        	case SDL_JOYAXISMOTION:
        		if(event.jaxis.axis && event.jaxis.value < 0)
        			WindowEvent(SDLK_UP);
        		else if(event.jaxis.axis && event.jaxis.value > 0)
        			WindowEvent(SDLK_DOWN);
        		else if(!event.jaxis.axis && event.jaxis.value < 0)
        			WindowEvent(SDLK_LEFT);
        		else if(!event.jaxis.axis && event.jaxis.value > 0)
        			WindowEvent(SDLK_RIGHT);
      			break;
        	case SDL_JOYBUTTONDOWN:
    			if(event.jbutton.button == 0 || event.jbutton.button == 3)
    				WindowEvent(SDLK_x); //■or○ボタン
    			else if(event.jbutton.button == 2)
    				WindowEvent(SDLK_z); //×ボタン
    			else if(event.jbutton.button == 4)
    				WindowEvent(SDLK_s); //Lボタン
    			else if(event.jbutton.button == 5)
    				WindowEvent(SDLK_d); //Rボタン
    			break;
        }
	 }
}

/*****************************************************************
関数名 : WindowEvent
機能	: メインウインドウに対するイベント処理を行う
引数	: SDLKey key : 入力の種類
出力	: なし
*****************************************************************/
void WindowEvent(SDLKey key)
{
	int i, j, set;
	if(key == SDLK_ESCAPE)
		gState = GAME_END;

	switch(gState){
	case GAME_TITLE:
		if(key == SDLK_x){ //タイトル->エディットへ
			if(tState == ADVENTURE)
				gState = GAME_BOSS;
			else if(tState == VS_MODE)
				gState = GAME_EDIT;
			else
				gState = GAME_END;
		}
		if(key == SDLK_UP)
			tState = (tState + MAX_TITLE - 1) % MAX_TITLE;
		if(key == SDLK_DOWN)
			tState = (tState + 1) % MAX_TITLE;
		break;
	case GAME_BOSS:
		switch(key){
		case SDLK_z:
			gState = GAME_TITLE;
			break;
		case SDLK_x:
			gState = GAME_EDIT;
			break;
		case SDLK_UP: //コマンドの選択
			gBoss.no = (gBoss.no + 1) % MAX_BOSS;
			break;
		case SDLK_DOWN:
			gBoss.no = (gBoss.no + MAX_BOSS - 1) % MAX_BOSS;
			break;
		default:
			break;
		}
		break;
	case GAME_EDIT:
		/* コマンド編集 */
		switch(key){
		case SDLK_z:
			gState = GAME_BOSS;
			break;
		case SDLK_x: //エディット->メインへ
			if(eState == EDIT_DECIDE)
				gState = GAME_MAIN;
			else
				eState = EDIT_DECIDE;
			break;
		case SDLK_UP: //コマンドの選択
			if(eState == EDIT_GUN)
				gChara[0].gun = (gChara[0].gun + MAX_PLAYERGUN - 1) % MAX_PLAYERGUN;
			else if(eState == EDIT_ARMOR)
				gChara[0].armor = (gChara[0].armor + MAX_ARMOR - 1) % MAX_ARMOR;
			break;
		case SDLK_DOWN:
			if(eState == EDIT_GUN)
				gChara[0].gun = (gChara[0].gun + 1) % MAX_PLAYERGUN;
			else if(eState == EDIT_ARMOR)
				gChara[0].armor = (gChara[0].armor + 1) % MAX_ARMOR;
			break;
		case SDLK_LEFT:
			eState = (eState + MAX_EDIT - 1) % MAX_EDIT;
			break;
		case SDLK_RIGHT:
			eState = (eState + 1) % MAX_EDIT;
			break;
		default:
			break;
		}
		break;
	case GAME_MAIN:
		/* コマンド入力 */
		switch(mState){
		case MAIN_COMMAND:
			switch(key){
			case SDLK_LEFT:
				cState = (cState + MAX_ECOMMAND-1) % MAX_ECOMMAND;
				break;
			case SDLK_RIGHT:
				cState = (cState + 1) % MAX_ECOMMAND;
				break;
			case SDLK_UP:
				if(cState == COMMAND_DIR)
					gCommand.dir = (gCommand.dir + 7) % 8;
				else if(cState == COMMAND_SHOT)
					gCommand.gun = (gCommand.gun - (8-1)) % 2 + 8;
				break;
			case SDLK_DOWN:
				if(cState == COMMAND_DIR)
					gCommand.dir = (gCommand.dir + 1) % 8;
				else if(cState == COMMAND_SHOT)
					gCommand.gun = (gCommand.gun - (8-1)) % 2 + 8;
				break;
			case SDLK_x:
				if(cState == COMMAND_RANDOM){	//おまかせ
					set = rand() % MAX_SET;
					for(i=0; i<MAX_COMMAND; i++){
						gChara[0].command[gChara[0].ctype][i] = (int)(commandset[set]/pow(10, MAX_COMMAND-i-1)) % 10;
					}
					gChara[0].cnum[gChara[0].ctype] = MAX_COMMAND;
				}
				else if(cState == COMMAND_DECIDE && gChara[0].cnum[gChara[0].ctype] >= 3){	//コマンド入力後の処理
					if(gChara[0].ctype == 0){
						gChara[0].ctype = 1;
					}
					else{
						mState = MAIN_COUNT;	//カウントに移行
						//COMのコマンド決定
						for(i=1; i<CT_NUM; i++){
							gChara[0].ctype = 0;
							set = rand() % MAX_SET;
							for(j=0; j<MAX_COMMAND; j++){
								gChara[i].command[0][j] = (int)(commandset[set]/pow(10, (MAX_COMMAND-j-1))) % 10;
								gChara[i].command[1][j] = (int)(commandset[set]/pow(10, (MAX_COMMAND-j-1))) % 10;
								//gChara[i].command[j] = (rand() % 2) + 8;
								gChara[i].cnum[0]++;
								gChara[i].cnum[1]++;
							}
						}
					}
				}
				else if(gChara[0].command[gChara[0].ctype][MAX_COMMAND-1] == -1){
					if(cState == COMMAND_DIR){
						gChara[0].command[gChara[0].ctype][gChara[0].cnum[gChara[0].ctype]] = gCommand.dir;
						gChara[0].cnum[gChara[0].ctype]++;
					}
					else if(cState == COMMAND_SHOT){
						gChara[0].command[gChara[0].ctype][gChara[0].cnum[gChara[0].ctype]] = gCommand.gun;
						gChara[0].cnum[gChara[0].ctype]++;
					}
					if(gChara[0].cnum[gChara[0].ctype] == MAX_COMMAND)
						cState = COMMAND_DECIDE;
				}
			break;
			case SDLK_z:
				if(gChara[0].cnum[gChara[0].ctype] != 0){
					gChara[0].cnum[gChara[0].ctype]--;
					gChara[0].command[gChara[0].ctype][gChara[0].cnum[gChara[0].ctype]] = -1;
				}
			break;
			default:
			break;
			}
			break;
		case MAIN_MOVE:
			if(key == SDLK_c && cflg == C_NO){
				cflg = C_PUSH;
			}
			break;
		case MAIN_RESULT://結果->コマンドへ
			if(key == SDLK_x)
				gState = GAME_TITLE;
			break;
		case MAIN_COUNT:
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/*****************************************************************
関数名 : DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow()
{
	/* FreeSurfaceの必要性とは */
	/**/
	SDL_Quit();
}

/* end of window.c */
