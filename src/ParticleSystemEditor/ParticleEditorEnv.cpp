#include "ParticleEditorEnv.h"
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/ITextSceneNode.h>
#include <CppBase/StringConventer.h>
#include <iostream>

using namespace irr;
using namespace std;
namespace xihad { namespace particle { namespace editor
{
	ParticleEditorEnv::ParticleEditorEnv( Node* source, Node* target, IrrlichtDevice* device, clock_t now) :
		CDefaultEnv(source, target), device(device), now(now)
	{
		textNode = device->getSceneManager()->addTextSceneNode(
			device->getGUIEnvironment()->getBuiltInFont(), L"", video::SColor(255,255,0,0), 0);
		textNode->setPosition(core::vector3df(-8, 10, 0));
	}

	void ParticleEditorEnv::deferMessage( scene::ISceneNode* msgSource, float delaySecs, const char* message )
	{
		DelayedMessage dmsg;
		dmsg.tm = now + static_cast<clock_t>(delaySecs*1000);
		dmsg.msg = message;
		delayedMessages.push(dmsg);
	}

	void ParticleEditorEnv::update( clock_t current )
	{
		now = current;

		while (!delayedMessages.empty())
		{
			auto& dmsg = delayedMessages.top();
			if (dmsg.tm < now)
			{
				wstring wmsg;
				StringConventer::UTF8_to_wstring(dmsg.msg.c_str(), wmsg);
				textNode->setText(wmsg.c_str());

				delayedMessages.pop();
			}
			else
			{
				break;
			}
		}
	}

	void ParticleEditorEnv::clearMessages()
	{
		while (!delayedMessages.empty())
			delayedMessages.pop();

		textNode->setText(L"");
	}
}}}

