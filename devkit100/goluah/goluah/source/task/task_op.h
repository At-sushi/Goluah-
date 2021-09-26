
/*===========================================================

        op.avi再生

=============================================================*/
#pragma once

#include "task.h"
/*!
        @brief なに？これ？
*/
class COpeningMovie : public CExclusiveTaskBase {
public:
  COpeningMovie();
  ~CYouWin();

  void Initialize();
  BOOL Execute(DWORD time);
  void Terminate();
  void Draw();

  DWORD GetID() { return 'Win'; }
  int GetDrawPriority() { return 1000; }

  void SetStoryText(char *txt);

protected:
  void CleanUp();

  //変数
protected:
  DWORD counter;          //カウンタ
  BOOL m_end_flag;        //終了フラグ
  UINT m_textlen;         //表示テキスト長
  DWORD wt;               //勝った方のチーム
  MYSURFACE *dds_face[3]; //表示顔

  LPDIRECT3DTEXTURE8 tex_fb; //試合終了時のフロントバッファのコピー

  int f_x[3];     // face2の表示位置
  float f_z[3];   // face2の表示位置
  BOOL show_text; //テキスト表示するかどうか
  DWORD jun[3];   //表示の順番

  char *m_text_from_story; //ストーリー側から指定されたテキスト
  char *m_text; //↑またはg_battleresultから取得したテキスト。deleteの必要なし。
};
