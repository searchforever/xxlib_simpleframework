﻿#include "xx_uv.h"
#include "lua.hpp"
#include "xx_lua_helpers.h"
#include "xx_lua_bbuffer.h"
#include "xx_lua_uvloop.h"
#include "xx_lua_uvtcpclient.h"

int Lmain(lua_State *L)
{
	luaL_openlibs(L);

	if (int r = luaL_loadfile(L, "src.lua"))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}
	lua_call(L, 0, LUA_MULTRET);

	return 0;
}

int Test()
{
	xx::MemPool mp;
	xx::UvLoop uvloop(&mp);
	uvloop.InitRpcTimeoutManager(1000, 10);

	auto L = luaL_newstate();
	if (!L) return -1;
	xx::ScopeGuard sgL([&] {lua_close(L); });

	xx::LuaRegisterMemPool(L, &mp);
	xx::LuaRegisterUvLoop(L, &uvloop);
	xx::LuaRegisterNull(L);
	xx::LuaRegisterInt64ToDateTime(L);
	xx::LuaRegisterInt64ToString(L);
	xx::LuaRegisterMakeRef(L);
	xx::LuaUvLoop::LuaRegister(L);
	xx::LuaBBuffer::LuaRegister(L);
	xx::LuaUvTcpClient::LuaRegister(L);

	lua_pushcclosure(L, &Lmain, 0);

	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}

	return 0;
}

int main()
{
#ifdef _WIN32   
	// 控制台显示乱码纠正, 设置字符集  system("chcp 65001");
	SetConsoleOutputCP(65001);
	CONSOLE_FONT_INFOEX info = { 0 };
	// 以下设置字体来支持中文显示。  
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = 18;
	info.dwFontSize.X = 10;
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"新宋体");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
#endif

	Test();
	std::cin.get();
	//return Test();
}
