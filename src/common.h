/*****************************************************************
ファイル名	: common.h
機能			: 共通変数･定数の宣言を行う
*****************************************************************/

//https://github.com/you-chan/BRS

#ifndef BRS_SRC_COMMON_H_
#define BRS_SRC_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>

#define PORT			(u_short)8888	/* ポート番号 */

#define WIDTH  1024	/* ウィンドウの幅 */
#define HEIGHT 768		/* ウィンドウの高さ */
#define F_WIDTH 768	/* フィールドの幅 */

#define S_SIZE  50	 /* 船(仮)の大きさ */
#define C_SIZE  100 /* コマンドアイコンの大きさ */
#define C_SCOPE 8	 /* 照準 */
#define C_FIRE  9	 /* 発射 */

#define MAX_CT 4			/* キャラクターの最大数 */
#define MAX_COMMAND 4			/* コマンドの最大格納数 */
#define MAX_SHOT MAX_CT * 512
#define MAX_COUNT 60			/* 最大カウント、減らすほど速度up */
#define MAX_SPEED 4			/* 最大再生スピード倍率 */
int lastcount;
int gspeed;						/* ゲームスピード */

/* ゲームの状態 */
typedef enum{
    GAME_TITLE,	/* タイトル */
	GAME_EDIT,	/* 編集 */
    GAME_MAIN,	/* メイン */
	GAME_END	/* 終了 */
}GameState;

/* 編集モードの状態 */
typedef enum{
    EDIT_GUN	= 0,	/* 武器選択 */
	EDIT_ARMOR	= 1,	/* 防具選択 */
	EDIT_DECIDE = 2		/* 決定 */
}EditState;

/* メインモードの状態 */
typedef enum{
    MAIN_COMMAND, /* コマンド入力 */
	MAIN_MOVE,	  /* 動作 */
	MAIN_RESULT   /* 結果 */
}MainState;

/*コマンドの選択タブ */
typedef enum{
    COMMAND_DIR		= 0,	/* 方向選択 */
    COMMAND_SHOT	= 1,	/* 照準or発射 */
	COMMAND_DECIDE	= 2
}CommandState;

/* 武器・防具の種類 */
enum{
	MAX_EDIT	= 3,	/* 編集項目の数 */

	GUN_1SHOT  = 0, /* 1ショット */
	GUN_3SHOT  = 1, /* 3ショット */
	GUN_BUBBLE = 2, /* バブル */
	GUN_MILK   = 3, /* ミルクの科学 */
	MAX_GUN    = 4,

	ARMOR_LIGHT		= 0, /* 軽 */
	ARMOR_MIDDLE	= 1, /* 中 */
	ARMOR_HEAVY		= 2, /* 重 */
	MAX_ARMOR		= 3
};

/* 武器の情報 */
typedef struct{
	int atk;	/* 攻撃力 */
	int color;	/* 玉の色(16進数) */
}GunInfo;

/* 防具の情報 */
typedef struct{
	int hp;		/* 耐久力 */
	int speed;	/* 速さ */
}ArmorInfo;

/* キャラクターの状態 */
typedef enum{
	DEAD   = 0,//戦闘不能
	LIVING = 1//生存
}CharaState;

/* 座標 */
typedef struct{
    double x;
    double y;
} Pos;

/* キャラクターの情報 */
typedef struct{
    Pos pos;					/* 座標 */
    Pos startpos;				/* 移動開始時の座標 */
    Pos goalpos;				/* 移動目標 */
    int dir;					/* キャラクターの角度 */
    int startdir;				/* 変更開始時の角度 */
    int goaldir;				/* 変更後の角度 */
    int command[MAX_COMMAND]; 	/* コマンドの格納 */
	CharaState state;
	int maxhp;					/* 最大HP */
	int hp;						/* 現在HP */
	int gun;					/* 装備中の武器 */
	int armor;					/* 装備中の防具 */
} CharaInfo;

/* 玉の情報 */
typedef struct{
	int id; /* 撃ったキャラの番号 */
	Pos pos;
	int dir;
	int color;
	CharaState state;
} ShotInfo;

/* コマンドの種類 */
typedef struct{
    int dir;  /* コマンドの向き(1〜7) */
    int gun; /* 8:照準, 9:発射 */
} CommandInfo;

int CT_NUM;		 /* 参加人数 */
int count;		 /* 移動処理のためのカウント */
int nowcommand;	 /* 現在適用されているコマンド */
int winner;		 /* 勝者 */
GameState gState;
EditState eState;
MainState mState;
CommandState cState;
GunInfo gGun[MAX_GUN];
ArmorInfo gArmor[MAX_ARMOR];
ShotInfo gShot[MAX_SHOT];
CharaInfo gChara[MAX_CT];
CommandInfo gCommand;

#endif /* BRS_SRC_COMMON_H_ */
