/*****************************************************************
ファイル名	: main.c
機能			: メインルーチン
*****************************************************************/

#include <SDL/SDL.h>
#include "common.h"
#include "func.h"

static int GameTitle();
static int GameEdit();
static int GameMain();
static void TimerEvent(int time);

int main(int argc,char *argv[])
{
	/* ウインドウの初期化 */
	if(InitWindow() == -1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}

	InitSystem();

	while(gState != GAME_END){
		switch(gState){
		case GAME_TITLE:
			GameTitle();
			break;
		case GAME_EDIT:
			GameEdit();
			break;
		case GAME_MAIN:
			GameMain();
			break;
		default:
			break;
		}
	}

	DestroyWindow();
	return 0;
}

/*****************************************************************
関数名 : GameTitle
機能	: タイトルの処理
引数	: なし
出力	: ゲームの状態
*****************************************************************/
int GameTitle()
{
    InitTitle();
    while(gState == GAME_TITLE){
        InputKey();
        DrawTitle();
    }
    return gState;
}

/*****************************************************************
関数名 : GameEdit
機能	: 武器編集の処理
引数	: なし
出力	: ゲームの状態
*****************************************************************/
int GameEdit()
{
	InitEdit();
    while(gState == GAME_EDIT){
        InputKey();
        DrawEdit();
    }
    return gState;
}


/*****************************************************************
関数名 : GameMain
機能	: メインの処理
引数	: なし
出力	: ゲームの状態
*****************************************************************/
int GameMain()
{
    InitMain();
    if(tState == ADVENTURE)
    	InitAdventure();
    /* タイマー */
    Uint32 interval = SDL_GetTicks() + 100;

    /* ゲーム内時刻：約0.1秒で1カウント */
    int time = 0;
    Uint32 now;

    while(gState == GAME_MAIN){
		InputKey();
    	now = SDL_GetTicks();
    	if(now >= interval){
			TimerEvent(++time);
			/* 0.04秒(25fps)ごとにタイマー処理するよう設定 */
			interval = now + 40;
    	}
    }
    return gState;
}

/*****************************************************************
関数名 : TimerEvent
機能	: メインでのタイマーイベント
引数	: time : 25fpsなのでif(time%25==0)->で1秒毎に何かしてくれるらしい
出力	: なし
*****************************************************************/
void TimerEvent(int time){
	/* time%5->0.2秒毎, time%25->1秒毎くらい */
	if(mState == MAIN_MOVE){
		UseCommand();
	}
	DrawMain();
}

/* end of main.c */
