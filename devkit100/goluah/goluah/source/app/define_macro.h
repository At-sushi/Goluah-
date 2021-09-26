
/*============================================================================

        マクロ 定義

==============================================================================*/
#pragma once

#ifndef MEMLEAK_CK_DEF
#include "define_const.h"
#endif

#define YEAH TRUE

#ifndef ZeroMemory2
#define ZeroMemory2(a) ZeroMemory(a, sizeof(a))
#endif

#ifndef DELETECLASS
#define DELETECLASS(a)                                                                                       \
  if (a) {                                                                                                   \
    delete (a);                                                                                              \
    a = NULL;                                                                                                \
  } //!< クラス破棄
#endif

#ifndef RELEASEDLL
#define RELEASEDLL(a)                                                                                        \
  if (a) {                                                                                                   \
    FreeLibrary(a);                                                                                          \
    a = NULL;                                                                                                \
  } //!< DLL開放
#endif

#ifndef RELSURFACE
#define RELSURFACE(x)                                                                                        \
  if (x) {                                                                                                   \
    g_draw.RelSurface(x);                                                                                    \
    x = NULL;                                                                                                \
  } //!< CDirectDrawサーフェイス開放
#endif

#ifndef FREEMALM
#define FREEMALM(a)                                                                                          \
  if (a) {                                                                                                   \
    free(a);                                                                                                 \
    a = NULL;                                                                                                \
  } //!< mallocで確保されたメモリ開放
#endif

#ifndef RELEASE
#define RELEASE(a)                                                                                           \
  if (a) {                                                                                                   \
    a->Release();                                                                                            \
    a = NULL;                                                                                                \
  }
#endif

#ifndef DELETEARRAY
#define DELETEARRAY(a)                                                                                       \
  if (a) {                                                                                                   \
    delete[] a;                                                                                              \
    a = NULL;                                                                                                \
  }
#endif

#ifndef SAFEDELETE
#define SAFEDELETE(a)                                                                                        \
  if (a) {                                                                                                   \
    delete (a);                                                                                              \
    a = NULL;                                                                                                \
  }
#endif

#ifndef foreach
#define foreach(a, b, c) for (b::iterator c = a.begin(); c != a.end(); c++) //!< STLリスト全数反復
#endif

#ifndef DELETE_ARRAY
#define DELETE_ARRAY(ptr)                                                                                    \
  if (ptr) {                                                                                                 \
    delete[] ptr;                                                                                            \
    ptr = NULL;                                                                                              \
  }
#endif

#ifndef SAFEDELETE
#define SAFEDELETE(ptr)                                                                                      \
  if (ptr) {                                                                                                 \
    delete ptr;                                                                                              \
    ptr = NULL;                                                                                              \
  }
#endif

#ifndef OPT2ALT
#define OPT2ALT(a) (((a)&CHARACTER_LOAD_OPTION_FACENUMBERMASK) >> 28) //!< face変更値取得
#endif

#ifndef DMY_STR
#define DMY_STR gbl.GetDummyString()
#endif
