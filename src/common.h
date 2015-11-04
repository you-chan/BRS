/*****************************************************************
ファイル名	: common.h
機能			: 共通変数･定数の宣言を行う
*****************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>

#define PORT			(u_short)8888	/* ポート番号 */

#define WIDTH  800	/* ウィンドウの幅 */
#define HEIGHT 640	/* ウィンドウの高さ */

#define S_SIZE  50	 /* 船(仮)の大きさ */
#define C_SIZE  100 /* コマンドアイコンの大きさ */
#define C_SCOPE 8	 /* 照準 */
#define C_SHOT  9	 /* 発射 */

#define CT_MAXNUM 7	/* キャラクターの最大数 */
#define MAX_COMMAND 4	/* コマンドの最大格納数 */
#define MAX_COUNT 50	/* 最大カウント、減らすほど速度up */

/* ゲームの状態 */
typedef enum{
    GAME_TITLE,	/* タイトル */
    GAME_MAIN,	/* メイン */
	GAME_END	/* 終了 */
}GameState;

/* メインモードの状態 */
typedef enum{
    MAIN_COMMAND, /* コマンド入力 */
	MAIN_MOVE,	  /* 動作 */
	MAIN_RESULT   /* 結果 */
}MainState;

/*コマンドの選択タブ */
typedef enum{
    COMMAND_DIR,	/* 方向選択 */
    COMMAND_SHOT	/* 照準or発射 */
}CommandState;

/* キャラクターの状態 */
typedef enum{
	LIVING,	//生存
	DEAD	//戦闘不能
}CharaState;

/* 座標 */
typedef struct{
    int x;
    int y;
} Pos;

/* キャラクターの情報 */
typedef struct{
    Pos pos;					/* 座標 */
    Pos startpos;				/* 移動開始時の座標 */
    Pos goalpos;				/* 移動目標 */
    Pos shotpos;				/* 弾の座標 */
    int shotflg;				/* 弾の有無 */
    int dir;					/* キャラクターの角度 */
    int startdir;				/* 変更開始時の角度 */
    int goaldir;				/* 変更後の角度 */
    int speed;					/* 速さ */
    int command[MAX_COMMAND]; 	/* コマンドの格納 */
	CharaState state;
} CharaInfo;

/* コマンドの種類 */
typedef struct{
    int dir;  /* コマンドの向き(1〜7) */
    int shot; /* 8:照準, 9:発射 */
} CommandInfo;

int CT_NUM;		 /* 参加人数 */
int count;		 /* 移動処理のためのカウント */
int nowcommand;	 /* 現在適用されているコマンド */
int winner;		 /* 勝者 */
GameState gState;
MainState mState;
CommandState cState;
CharaInfo gChara[CT_MAXNUM];
CommandInfo gCommand;

#endif /* COMMON_H_ */
