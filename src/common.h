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

#define S_SIZE 50			/* 船(仮)の大きさ */
#define C_SIZE 100			/* コマンドアイコンの大きさ */
#define MAX_CT 4			/* キャラクターの最大数 */
#define MAX_COMMAND 6		/* コマンドの最大格納数 */
#define MAX_BOSSCOMMAND 8	/* ボス用コマンドの最大格納数 */
#define MAX_SHOT	300
#define MAX_USEMOB	50		/* 使用可能なMobの数 */
#define MAX_COUNT	20		/* 最大カウント、減らすほど速度up */
#define MAX_DCOUNT 50
#define FPS		25		/* フレームパーセク */

#define PLAYER 0
#define BOSS	4
/* ゲームの状態 */
typedef enum{
    GAME_TITLE,		/* タイトル */
	GAME_BOSS,		/* ボス編集 */
	GAME_EDIT,	/* コマンド編集 */
    GAME_MAIN,		/* メイン */
	GAME_END		/* 終了 */
}GameState;

/* タイトルの状態 */
typedef enum{
    ADVENTURE	= 0,	/* タイトル */
	VS_MODE		= 1,	/* 編集 */
    TITLE_QUIT	= 2		/* 終了 */
}TitleState;

/* 編集モードの状態 */
typedef enum{
    EDIT_GUN		= 0,	/* 武器選択 */
	EDIT_ARMOR		= 1,	/* 防具選択 */
	EDIT_DECIDE 	= 2		/* 決定 */
}EditState;

/* メインモードの状態 */
typedef enum{
    MAIN_COMMAND,	/* コマンド入力 */
	MAIN_COUNT,		/* 対戦前のカウント */
	MAIN_MOVE,	 	/* 動作 */
	MAIN_RESULT   	/* 結果 */
}MainState;

/*コマンドの選択タブ */
typedef enum{
    COMMAND_DIR		= 0,	/* 方向選択 */
    COMMAND_SHOT	= 1,	/* 照準or発射 */
	COMMAND_RANDOM	= 2,	/* おまかせ */
	COMMAND_DECIDE	= 3		/* 決定 */
}CommandState;

enum{
	C_SCOPE = 8,	/* 照準コマンド */
	C_FIRE  = 9		/* 発射コマンド */
};

/* 選べるやつの最大数 */
enum{
	MAX_TITLE		= 3,
	MAX_EDIT		= 3,	/* 武器、防具、決定 */
	MAX_ECOMMAND	= 4		/* 方向、発射、おまかせ、決定 */
};

/* 武器・防具の種類 */
enum{
	GUN_1SHOT		= 0, /* 1ショット */
	GUN_3SHOT		= 1, /* 3ショット */
	GUN_BUBBLE		= 2, /* バブル */
	GUN_1LASER		= 3, /* レーザー */

	GUN_MILK		= 4, /* ミルクの科学 */
	GUN_MSHOT		= 5,
	GUN_5SHOT		= 6, /* 5ショット */
	GUN_3LASER		= 7, /* 3レーザー */
	GUN_NUCLEAR		= 8,
	GUN_HOMO0		= 9,
	GUN_HOMO1		= 10,
	GUN_MISSILE		= 11,
	GUN_FIRE		= 12,

	MAX_GUN			= 13, /* ボス用の武器も含めた武器の数 */
	MAX_PLAYERGUN	= 4, /* プレイヤーが使用可能な武器の数 */
	MAX_BOSSGUN		= 3, /* ボスが一度に使用可能な武器の数 */

	ARMOR_LIGHT		= 0, /* 軽 */
	ARMOR_MIDDLE	= 1, /* 中 */
	ARMOR_HEAVY		= 2, /* 重 */
	ARMOR_MISSILE	= 3,
	MAX_ARMOR		= 4,
};

/* mobのタイプ */
enum{
	HOMO0	= 0,
	HOMO1	= 1,
	MISSILE	= 2,
	FIRE	= 3,
	MAX_MOB	= 4,

	M_CHARA	= 0,	/* キャラとして機能 */
	M_SHOT	= 1		/* ミサイル */
};

/* ショットのタイプ */
enum{
	SHOT	= 0,
	LASER	= 1,
	MOB		= 2
};

/* ボスの種類 */
enum{
	SENKOUSHA		= 0,
	GAHARA			= 1,
	SUDACHI			= 2,
	SHIBBOLETH		= 3,
	HOMO			= 4,
	MAX_BOSS		= 5,

	DELETE			= -1
};

/* ボスの動作 */
enum{
	MOVE_NOTHING	= 0,
	MOVE_UP			= 1,
	MOVE_DOWN		= 2,
	MOVE_LEFT		= 3,
	MOVE_RIGHT		= 4,
	MOVE_RANDOM		= 5,
	MOVE_TURN_S		= 6,
	MOVE_TURN_L		= 7,
	MOVE_SHAKE_W	= 8,
	MOVE_SHAKE_H	= 9,

	BOSS_SCOPE		= 1,
	BOSS_FIRE		= 2
};

/* コマンド変更の変遷 */
enum{
	C_NO	= 0,
	C_PUSH	= 1,
	C_WAIT	= 2,
	C_STOP	= 3
};

/* 武器の情報 */
typedef struct{
	int type;	/* 弾 or レーザー or Mob */
	int atk;	/* 攻撃力 */
	int mobno;	/* 召喚するmobの番号 */
	int mobnum;	/* 召喚可能なmobの最大数 */
	int speed;	/* 弾速 */
	int size;	/* 弾の大きさ */
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

/* Mobの情報 */
typedef struct{
	int type;		/* 小型ロボ or ミサイル */
    int command; 	/* 発射コマンド, 6桁 */
	int gun;		/* 装備中の武器 */
	int hp;			/* 最大HP */
	int atk;
	int speed;		/* 速度 */
	int w;
	int h;
	int anipat;		/* アニメのパターン */
} MobData;

typedef struct{
	int no;							/* 個体 */
	int type;
	int id;							/* 所有者 */
    Pos pos;						/* 発射座標 */
	CharaState state;
    int dir;						/* キャラクターの角度 */
    int startdir;					/* 変更開始時の角度 */
    int goaldir;					/* 変更後の角度 */
    int command[MAX_COMMAND];
    int cnum;
	int gun;						/* 装備中の武器 */
	int maxhp;						/* 最大HP */
	int hp;							/* 現在HP */
	int atk;
	int speed;
	int delay;
	int w;
	int h;
	int anipat;						/* アニメのパターン */
	int anipatnum;					/* アニメのパターンの最大数 */
} MobInfo;

/* キャラクターの情報 */
typedef struct{
    Pos pos;					/* 座標 */
	CharaState state;
    int dir;					/* キャラクターの角度 */
    int startdir;				/* 変更開始時の角度 */
    int goaldir;				/* 変更後の角度 */
    int command[2][MAX_COMMAND]; 	/* コマンドの格納 */
    int cnum[2];				/* コマンドの使用数(3〜6) */
    int ctype;			/* コマンドの種類(0か1) */
	int gun;					/* 装備中の武器 */
	int armor;					/* 装備中の防具 */
	int maxhp;					/* 最大HP */
	int hp;						/* 現在HP */
	int atk;					/* 攻撃力 */
	int speed;
	int dcount;					/* 戦闘不能になってからの経過時間 */
	int sflg;					/* spell flag */
	int scount;					/* spell count */
} CharaInfo;

/* ボスの情報 */
typedef struct{
	Pos shotpos[MAX_BOSSGUN];		/* 弾の発射される座標 */
    int movecommand;				/* 移動コマンド, 6桁 */
    int shotcommand[MAX_BOSSGUN]; 	/* 発射コマンド, 6桁 */
	int gun[MAX_BOSSGUN];			/* 装備中の武器 */
	int hp;							/* 最大HP */
	int speed;						/* 速度 */
	int w;
	int h;
	int anipat;						/* アニメのパターン */
	int next;						/* 次の形態 */
} BossData;

typedef struct{
	int no;											/* 個体 */
    Pos pos;										/* 座標 */
    Pos shotpos[MAX_BOSSGUN];						/* 発射座標 */
    int dir[MAX_BOSSGUN];							/* キャラクターの角度 */
    int startdir[MAX_BOSSGUN];						/* 変更開始時の角度 */
    int goaldir[MAX_BOSSGUN];						/* 変更後の角度 */
    int movecommand[MAX_BOSSCOMMAND];				/* 移動コマンド */
    int shotcommand[MAX_BOSSGUN][MAX_BOSSCOMMAND]; 	/* 発射コマンド */
	CharaState state;
	int gun[MAX_BOSSGUN];							/* 装備中の武器 */
	int maxhp;										/* 最大HP */
	int hp;											/* 現在HP */
	int atk[MAX_BOSSGUN];
	int speed;
	int w;
	int h;
	int anipat;										/* アニメのパターン */
	int anipatnum;									/* アニメのパターンの最大数 */
	int next;										/* 次の形態 */
	int dcount;
	int sflg;					/* spell flag */
	int scount;					/* spell count */
} BossInfo;

/* 玉の情報 */
typedef struct{
	int id; /* 撃ったキャラの番号 */
	int type;
	Pos pos;
	Pos startpos;
	int dir;
	int atk;
	int speed;
	int size;
	int color;
	CharaState state;
} ShotInfo;

/* コマンドの種類 */
typedef struct{
    int dir;  /* コマンドの向き(0〜7) */
    int gun; /* 8:照準, 9:発射 */
} CommandInfo;

int CT_NUM;		/* 合計人数 */
int count;		/* 移動処理のためのカウント */
int nowcommand;	/* 現在適用されているコマンド */
int win;		/* 勝敗, 1:勝ち, 2:負け */
int cflg;		/* コマンド変更フラグ */
int startcount;	/* はじめる前にカウントとかしたいじゃん */
GameState gState;
TitleState tState;
EditState eState;
MainState mState;
CommandState cState;
GunInfo gGun[MAX_GUN];
ArmorInfo gArmor[MAX_ARMOR];
ShotInfo gShot[MAX_SHOT];
CharaInfo gChara[MAX_CT];
BossData bData[MAX_BOSS];
BossInfo gBoss;
MobData mData[MAX_MOB];
MobInfo gMob[MAX_USEMOB];
CommandInfo gCommand;

#endif /* BRS_SRC_COMMON_H_ */
