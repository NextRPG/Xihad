#pragma once
#include <Particle/CDefaultEnv.h>
#include <ctime>
#include <queue>
#include <string>

namespace irr {
	class IrrlichtDevice;

	namespace scene {
		class ITextSceneNode;
	}
}

namespace xihad { namespace particle { namespace editor
{
	class ParticleEditorEnv : public CDefaultEnv
	{
	public:
		ParticleEditorEnv(Node* source, Node* target, irr::IrrlichtDevice* device, clock_t now);

		virtual void deferMessage(scene::ISceneNode* msgSource, float delaySecs, const char* message);

		virtual void clearMessages();

		virtual void update(clock_t current);

	private:
		clock_t now;
		struct DelayedMessage
		{
			clock_t		tm;
			std::string msg;

			bool operator<(const DelayedMessage& other) const
			{
				return tm > other.tm;	// Priority is low if time is later
			}
		};

		std::priority_queue<DelayedMessage> delayedMessages;
		irr::IrrlichtDevice* device;
		irr::scene::ITextSceneNode* textNode;
	};
}}}
