/*****************************************************************
ファイル名	: window.c
機能			: ユーザーインターフェース処理
*****************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "common.h"
#include "func.h"

static char TitleImgFile[]  = "img/title.png";
static char ResultImgFile[2][16] = {"img/win.png", "img/lose.png"};
static char ShipImgFile[4][16]  =
	{"img/ship01.png", "img/ship02.png", "img/ship03.png", "img/ship04.png"};
static char GunImgFile[] = "img/gun.png";
static char ArmorImgFile[] = "img/armor.png";
static char ArrowImgFile[] = "img/arrow.png";
static char DecideImgFile[] = "img/decide.png";
static char CommandImgFile[]    = "img/command01.png";
static char miniCommandImgFile[]    = "img/command02.png";

static SDL_Surface *gMainWindow;
static SDL_Surface *TitleWindow;
static SDL_Surface *ResultWindow[2];
static SDL_Surface *ShipWindow[MAX_CT];
static SDL_Surface *GunWindow;
static SDL_Surface *ArmorWindow;
static SDL_Surface *ArrowWindow;
static SDL_Surface *DecideWindow;
static SDL_Surface *CommandWindow;
static SDL_Surface *miniCommandWindow;
static SDL_Joystick *joystick; //ジョイスティックを特定・利用するための構造体

/* 画像転送用 */
typedef struct {
    SDL_Rect src;
    SDL_Rect dst;
} Rect;

static void DrawShip();
static void DrawShot();
static void DrawCommand();
static void DrawResult();
static void WindowEvent(SDLKey key);

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
    TitleWindow = IMG_Load(TitleImgFile);
    if(TitleWindow == NULL){
        printf("failed to open title image.");
        return -1;
    }

    for(i=0; i<2; i++){
    	ResultWindow[i] = IMG_Load(ResultImgFile[i]);
    	if(ResultWindow[i] == NULL){
    		printf("failed to open result image.\n");
    		return -1;
    	}
    }

    for(i=0; i < MAX_CT; i++){
		ShipWindow[i] = IMG_Load(ShipImgFile[i % 4]);
		if(ShipWindow[i] == NULL){
			printf("failed to open ship image.");
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

    ArrowWindow = IMG_Load(ArrowImgFile);
        if(ArrowWindow == NULL){
        	printf("failed to open arrow image.");
        	return -1;
        }

    DecideWindow = IMG_Load(DecideImgFile);
   		if(DecideWindow == NULL){
    		printf("failed to open decide image.");
    		return -1;
    	}

	CommandWindow = IMG_Load(CommandImgFile);
    if(CommandWindow == NULL){
        printf("failed to open command image.");
        return -1;
    }

	miniCommandWindow = IMG_Load(miniCommandImgFile);
    if(CommandWindow == NULL){
        printf("failed to open minicommand image.");
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
	Rect rect = {{0, 0, WIDTH, HEIGHT}, {0, 0}};
	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0xffffff);
	SDL_BlitSurface(TitleWindow, &(rect.src), gMainWindow, &(rect.dst));
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : DrawEdit
機能	: 編集画面の描画
引数	: なし
出力	: なし
*****************************************************************/
void DrawEdit()
{
	Rect rect = {{0, 0, 100, 320}, {F_WIDTH/8, 5 * HEIGHT /8 - 110}}; //とりあえず100*100
	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0x808080);
	boxColor(gMainWindow, F_WIDTH, 0, WIDTH, HEIGHT, 0x000000FF);

	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));

	rect.src.x = gChara[0].gun*100;
	rect.src.y = 0;
	rect.src.w = rect.src.h = 100;
	rect.dst.x = 1 * F_WIDTH /8;
	rect.dst.y = 5 * HEIGHT /8;
	SDL_BlitSurface(GunWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = gChara[0].armor*100;
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(ArmorWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = 0;
	rect.dst.x = 5 * F_WIDTH /8;
	SDL_BlitSurface(DecideWindow, &(rect.src), gMainWindow, &(rect.dst));

	//100 + i * (HEIGHT - 3 * 100) / (MAX_EDIT - 1);

	/*選択中でない装備タブを灰色に*/
	boxColor(gMainWindow, F_WIDTH*(((eState+1)*2+1)%6)/8, 5*HEIGHT/8, F_WIDTH*(((eState+1)*2+1)%6)/8+100, 5*HEIGHT/8+100, 0x00000060);
	boxColor(gMainWindow, F_WIDTH*(((eState*2)+5)%6)/8, 5*HEIGHT/8, F_WIDTH*(((eState*2)+5)%6)/8+100, 5*HEIGHT/8+100, 0x00000060);
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
	SDL_FillRect(gMainWindow,NULL,0x303030);
	boxColor(gMainWindow, F_WIDTH, 0, WIDTH, HEIGHT, 0x000000FF);
	if(mState == MAIN_RESULT) //結果画面
		DrawResult();
	else
		DrawShip();

	if(mState == MAIN_COMMAND) //コマンド入力
		DrawCommand();
	else if(mState == MAIN_MOVE) //コマンド適用
		DrawShot();
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名 : DrawShip
機能	: 船(仮)を描画する
引数	: なし
出力	: なし
*****************************************************************/
void DrawShip()
{
	int i;
	Rect rect = {{0, 0, F_WIDTH, HEIGHT}, {0, 0}};

	for(i=0; i<CT_NUM; i++){
		if(gChara[i].state == LIVING){
			rect.src.x = (((gChara[i].dir % 180) % 45) / 5 * S_SIZE);
			rect.src.y = ((gChara[i].dir % 180) / 45) * S_SIZE;
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

	// 1Pの区別用
	if(gChara[0].state == LIVING)
		filledCircleColor(gMainWindow, gChara[0].pos.x+S_SIZE/2, gChara[0].pos.y+S_SIZE/2, 10, 0xFFFFFFF); //自分を写す
}

/*****************************************************************
関数名 : DrawShot
機能	: 先輩こいつ玉とか描画しはじめましたよ
引数	: なし
出力	: なし
*****************************************************************/
void DrawShot()
{
	int i;
	for(i=0; i<MAX_SHOT; i++){
		if(gShot[i].state == LIVING)
			filledCircleColor(gMainWindow, gShot[i].pos.x, gShot[i].pos.y, 10, gShot[i].color);
	}
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
	Rect rect = {{0, 0, 100, 320}, {F_WIDTH/8, 5 * HEIGHT /8 - 110}};

	/* 選択中のコマンドの描画 */
	boxColor(gMainWindow, 0, 0, F_WIDTH, HEIGHT, 0x00000040);

	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(ArrowWindow, &(rect.src), gMainWindow, &(rect.dst));

	rect.src.x = gCommand.dir * C_SIZE;
	rect.src.y = 0;
	rect.src.w = rect.src.h = C_SIZE;
	rect.dst.x = 1 * F_WIDTH /8;
	rect.dst.y = 5 * HEIGHT /8;
	SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = gCommand.gun * C_SIZE;
	rect.dst.x = 3 * F_WIDTH /8;
	SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
	rect.src.x = 0;
	rect.dst.x = 5 * F_WIDTH /8;
	SDL_BlitSurface(DecideWindow, &(rect.src), gMainWindow, &(rect.dst));

	/*選択していないコマンドを灰色に*/
	boxColor(gMainWindow, F_WIDTH*(((cState+1)*2+1)%6)/8, 5*HEIGHT/8, F_WIDTH*(((cState+1)*2+1)%6)/8+C_SIZE, 5*HEIGHT/8+C_SIZE, 0x00000060);
	boxColor(gMainWindow, F_WIDTH*(((cState*2)+5)%6)/8, 5*HEIGHT/8, F_WIDTH*(((cState*2)+5)%6)/8+C_SIZE, 5*HEIGHT/8+C_SIZE, 0x00000060);


	/* 選択したコマンドの描画 */
	for(i=0; i<MAX_COMMAND; i++){
		if(gChara[0].command[i] != -1){
			rect.src.x = gChara[0].command[i] * C_SIZE;
			rect.src.y = 0;
			rect.src.w = rect.src.h = C_SIZE;
			rect.dst.x = F_WIDTH / 4 + C_SIZE * i * 4 / MAX_COMMAND;
			rect.dst.y = HEIGHT / 8;
			SDL_BlitSurface(CommandWindow, &(rect.src), gMainWindow, &(rect.dst));
		}
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
	if(winner == 0)
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
	if(key == SDLK_ESCAPE)
		gState = GAME_END;

	switch(gState){
	case GAME_TITLE:
		if(key == SDLK_x) //タイトル->エディットへ
			gState = GAME_EDIT;
		break;
	case GAME_EDIT:
		/* 編集 */
		switch(key){
		case SDLK_z:
			gState = GAME_TITLE;
			break;
		case SDLK_x: //エディット->メインへ
			if(eState == EDIT_DECIDE)
				gState = GAME_MAIN;
			else
				eState = EDIT_DECIDE;
			break;
		case SDLK_UP: //コマンドの選択
			if(eState == EDIT_GUN)
				gChara[0].gun = (gChara[0].gun + MAX_GUN - 1) % MAX_GUN;
			else if(eState == EDIT_ARMOR)
				gChara[0].armor = (gChara[0].armor + MAX_ARMOR - 1) % MAX_ARMOR;
			break;
		case SDLK_DOWN:
			if(eState == EDIT_GUN)
				gChara[0].gun = (gChara[0].gun + 1) % MAX_GUN;
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
				cState = (cState + 3-1) % 3;
				break;
			case SDLK_RIGHT:
				cState = (cState + 1) % 3;
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
				if(gChara[0].command[MAX_COMMAND-1] == -1){
					if(cState == COMMAND_DIR){
						gChara[0].command[nowcommand] = gCommand.dir;
						nowcommand++;
					}
					else if(cState == COMMAND_SHOT){
						gChara[0].command[nowcommand] = gCommand.gun;
						nowcommand++;
					}
					if(nowcommand == MAX_COMMAND)
						cState = COMMAND_DECIDE;
				}
				else if(cState == COMMAND_DECIDE){	//コマンド入力後の処理
					nowcommand = 0;
					mState = MAIN_MOVE;	//動作に移行
				}
			break;
			case SDLK_z:
				if(nowcommand != 0){
					nowcommand--;
					gChara[0].command[nowcommand] = -1;
				}
			break;
			default:
				break;
			}
			break;
		case MAIN_MOVE:						//加減速
			if(key == SDLK_LEFT && gspeed != 1)
				gspeed--;
			else if(key == SDLK_RIGHT && gspeed != MAX_SPEED)
				gspeed++;
			break;
		case MAIN_RESULT://結果->コマンドへ
			if(key == SDLK_x)
				InitMain();
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
