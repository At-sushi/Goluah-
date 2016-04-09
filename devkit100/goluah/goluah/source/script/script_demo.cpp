#include "stdafx.h"
#include <lua.hpp>
#include "script_story.h"
#include "srcipt_demo.h"


static int l_vs(lua_State *L);

BOOL CGoluahDemoScript::CreateScriptElementList(SScriptElementList& list,char *filename,CAliasList *args)
{
	lua_State* L = luaL_newstate();

	if (L)
	{
		// �g�ݍ��݊֐��̓o�^
		lua_register(L, _T("vs"), &l_vs);

		lua_close(L);
		return TRUE;
	}
	else
		return FALSE;

}


static int l_vs(lua_State *L)
{
	// �t�B�[���h����
	lua_getfield(L, 1, _T("type"));
	lua_getfield(L, 1, _T("friend"));
	lua_getfield(L, 1, _T("enemy"));
	lua_getfield(L, 1, _T("stage"));
	lua_getfield(L, 1, _T("timelimit"));
	lua_getfield(L, 1, _T("nowin"));
	lua_getfield(L, 1, _T("wintext"));
	lua_getfield(L, 1, _T("losetext"));
	lua_getfield(L, 1, _T("bgm"));
	lua_getfield(L, 1, _T("nogameover"));
	return 0;
}
