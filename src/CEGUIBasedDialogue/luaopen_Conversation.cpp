#include <luaT/luaT.h>
#include "SpeakerSupport.h"
#include <CEGUI/String.h>
#include "Conversation.h"

using namespace luaT;
namespace xihad { namespace dialogue
{
	using namespace ngn;

	luaT_static void setIcon(SpeakerSupport* speaker, const char* icon) 
	{
		speaker->setIcon(icon);
	}}
	
	luaT_static void speak(SpeakerSupport* speaker, const char* text) 
	{
		speaker->speak(text);
	}}
	
	luaT_static int getStatus(lua_State* L) 
	{
		luaT_variable(L, 1, SpeakerSupport*, speaker);
		SpeakerSupport::Status stat = speaker->getStatus();
		assert(stat >= 0 && stat < 4);

		const char* statStr[] = {
			"close", "open", "deactive", "active",
		};
		lua_pushstring(L, statStr[stat]);
		return 1;
	}}
	
	static int newSpeaker(lua_State* L)
	{
		luaT_variable(L, 1, Conversation*, conv);
		luaT_variable(L, 2, const char*, name);
		luaT_variable(L, 3, int, paddingX);
		luaT_variable(L, 4, int, paddingY);
		luaT_variable(L, 5, int, lineSpace);
		luaT_variable(L, 6, int, width);
		luaT_variable(L, 7, int, height);

		SpeakerSupport* ret = conv->newSpeaker(name, 
			ngn::dimension2di(paddingX, paddingY), lineSpace, width, height);
		
		luaT::push<SpeakerSupport*>(L, ret);
		return 1;
	}

	luaT_static Conversation* createConversation()
	{
		return new Conversation;
	}}

	static int luaopen_Conversation_impl(lua_State* L)
	{
		luaT_defRegsBgn(conv_reg)
			luaT_lnamedfunc(newSpeaker),
			luaT_mnamedfunc(Conversation, requestStop),
		luaT_defRegsEnd
		MetatableFactory<Conversation, UpdateHandler>::create(L, conv_reg);

		luaT_defRegsBgn(speaker_reg)
			luaT_mnamedfunc(SpeakerSupport, open),
			luaT_mnamedfunc(SpeakerSupport, close),
			luaT_mnamedfunc(SpeakerSupport, active),
			luaT_mnamedfunc(SpeakerSupport, deactive),
			luaT_cnamedfunc(setIcon),
			luaT_cnamedfunc(speak),
			luaT_mnamedfunc(SpeakerSupport, setIconRelativeX),
			luaT_mnamedfunc(SpeakerSupport, setDialoguePosition),
			luaT_mnamedfunc(SpeakerSupport, speakAll),
			luaT_mnamedfunc(SpeakerSupport, setSpeed),
			luaT_mnamedfunc(SpeakerSupport, isTicking),
			luaT_lnamedfunc(getStatus),
		luaT_defRegsEnd
		MetatableFactory<SpeakerSupport>::create(L, speaker_reg);

		luaT_defRegsBgn(conv_lib_reg)
			luaT_cnnamefunc(createConversation, new),
		luaT_defRegsEnd
		luaL_register(L, "Conversation", conv_lib_reg);

		return 1;
	}

	extern "C" __declspec(dllexport) int luaopen_CEGUIBasedDialogue( lua_State* L )
	{
		return luaopen_Conversation_impl(L);
	}
}}

