/*****************************************************************
ファイル名	: func.h
機能			: 外部関数の定義
*****************************************************************/

#ifndef FUNC_H_
#define FUNC_H_

#include "common.h"

/* window.c */
extern int  InitWindow();
extern void DrawTitle();
extern void DrawMain();
extern void InputKey();
extern void DestroyWindow();

/* system.c */
extern void InitTitle();
extern void InitMain();
extern void UseCommand();

#endif /* FUNC_H_ */
