#include "Etterna/Globals/global.h"
#include "ArchHooks.h"
#include "Etterna/Models/Lua/LuaReference.h"
#include "RageUtil/Misc/RageThreads.h"
#include "arch/arch_default.h"
#include "Etterna/Singletons/PrefsManager.h"
#include "Core/Services/Locator.hpp"
#include "Core/Arch/Arch.hpp"

bool ArchHooks::g_bToggleWindowed = false;
// Keep from pulling RageThreads.h into ArchHooks.h
static RageMutex g_Mutex("ArchHooks");
ArchHooks* HOOKS = NULL; // global and accessible from anywhere in our program

ArchHooks::ArchHooks()
  : m_bHasFocus(true)
  , m_bFocusChanged(false)
{
}

ArchHooks*
ArchHooks::Create()
{
	return new ARCH_HOOKS;
}


/* XXX: Most singletons register with lua in their constructor.  ArchHooks is
 * instantiated before Lua, so we encounter a dependency problem when
 * trying to register HOOKS. Work around it by registering HOOKS function
 * which sm_main will call after instantiating Lua. */
void
ArchHooks::RegisterWithLua()
{
	Lua* L = LUA->Get();
	lua_pushstring(L, "HOOKS");
    Locator::getArchHooks()->PushSelf(L);
	lua_settable(L, LUA_GLOBALSINDEX);
	LUA->Release(L);
}

// lua start
#include "Etterna/Models/Lua/LuaBinding.h"
#include "Etterna/Models/Lua/LuaReference.h"

class LunaArchHooks : public Luna<ArchHooks>
{
  public:
//	DEFINE_METHOD(AppHasFocus, AppHasFocus());
//	DEFINE_METHOD(GetArchName, GetArchName());
//	DEFINE_METHOD(GetClipboard, GetClipboard());
	static int ShowCursor(T* p, lua_State* L)
	{
		Core::Arch::setCursorVisible(BArg(1));
		return 0;
	}

	LunaArchHooks()
	{
//		ADD_METHOD(AppHasFocus);
//		ADD_METHOD(GetArchName);
//		ADD_METHOD(GetClipboard);
		ADD_METHOD(ShowCursor);
	}
};
LUA_REGISTER_CLASS(ArchHooks);
