#include "stdafx.h"
#include <lua.hpp>
#include "script_story.h"
#include "srcipt_demo.h"

static int l_vs(lua_State *L);

BOOL CGoluahDemoScript::CreateScriptElementList(SScriptElementList &list, char *filename, CAliasList *args) {
  lua_State *L = luaL_newstate();

  if (L) {
    // 組み込み関数の登録
    lua_register(L, "vs", &l_vs);

    lua_close(L);
    return TRUE;
  } else
    return FALSE;
}

static int l_vs(lua_State *L) {
  // フィールド所得
  lua_getfield(L, 1, "type");
  lua_getfield(L, 1, "friend");
  lua_getfield(L, 1, "enemy");
  lua_getfield(L, 1, "stage");
  lua_getfield(L, 1, "timelimit");
  lua_getfield(L, 1, "nowin");
  lua_getfield(L, 1, "wintext");
  lua_getfield(L, 1, "losetext");
  lua_getfield(L, 1, "bgm");
  lua_getfield(L, 1, "nogameover");
  return 0;
}
