/*****************************************************************
ファイル名	: func.h
機能			: 外部関数の定義
*****************************************************************/

#ifndef BRS_SRC_FUNC_H_
#define BRS_SRC_FUNC_H_

#include "common.h"

/* window.c */
extern int  InitWindow();
extern void DrawTitle();
extern void DrawEdit();
extern void DrawMain();
extern void InputKey();
extern void DestroyWindow();

/* system.c */
extern void InitSystem();
extern void InitTitle();
extern void InitEdit();
extern void InitMain();
extern void UseCommand();

#endif /* BRS_SRC_FUNC_H_ */
