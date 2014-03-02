// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__
#define __I_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "dimension2d.h"

namespace irr
{
namespace scene
{

class IParticleAffector;
struct SRangedParticleAffector
{
	float lifeBgn;
	float lifeEnd;
	IParticleAffector* affector;
};

class IParticleEmitter;
class IParticleRenderer;

//! A particle system scene node for creating snow, fire, exlosions, smoke...
/** A scene node controlling a particle System. The behavior of the particles
can be controlled by setting the right particle emitters and affectors.
You can for example easily create a campfire by doing this:

\code
	scene::IParticleSystemSceneNode* p = scenemgr->addParticleSystemSceneNode();
	p->setParticleSize(core::dimension2d<f32>(20.0f, 10.0f));
	scene::IParticleEmitter* em = p->createBoxEmitter(
		core::aabbox3d<f32>(-5,0,-5,5,1,5),
		core::vector3df(0.0f,0.03f,0.0f),
		40,80, video::SColor(0,255,255,255),video::SColor(0,255,255,255), 1100,2000);
	p->setEmitter(em);
	em->drop();
	scene::IParticleAffector* paf = p->createFadeOutParticleAffector();
	p->addAffector(paf);
	paf->drop();
\endcode

*/
class IParticleSystemSceneNode : public ISceneNode
{
public:
	//! Constructor
	IParticleSystemSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position = core::vector3df(0,0,0),
		const core::vector3df& rotation = core::vector3df(0,0,0),
		const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

	//! Sets if the particles should be global.
	/** If they are, the particles are affected by the movement of the
	particle system scene node too, otherwise they completely ignore it.
	Default is true. */
	virtual void setParticlesAreGlobal(bool global=true) = 0;

	virtual void setMaxParticleCount(s32) = 0;

	virtual s32 getMaxParticleCount() const = 0;

	//! Remove all currently visible particles
	virtual void clearParticles() = 0;

	virtual u32 getParticleCount() = 0;

	//! Do manually update the particles.
 	//! This should only be called when you want to render the node outside the scenegraph,
 	//! as the node will care about this otherwise automatically.
	virtual void doParticleSystem(f32 time) = 0;

	//! Sets the particle emitter, which creates the particles.
	/** A particle emitter can be created using one of the createEmitter
	methods. For example to create and use a simple PointEmitter, call
	IParticleEmitter* p = createPointEmitter(); setEmitter(p); p->drop();
	\param emitter: Sets the particle emitter. You can set this to 0 for
	removing the current emitter and stopping the particle system emitting
	new particles. */
	virtual void addEmitter(IParticleEmitter*, f32 timeToBeginEmitt = 0.f, f32 timeToStopEmitt = 16000000.f) = 0;

	virtual void clearEmitters() = 0;

	virtual void setRenderer(IParticleRenderer*) = 0;

	virtual IParticleRenderer* getRenderer() = 0;

	//! Adds new particle effector to the particle system.
	/** A particle affector modifies the particles. For example, the FadeOut
	affector lets all particles fade out after some time. It is created and
	used in this way:
	\code
	IParticleAffector* p = createFadeOutParticleAffector();
	addAffector(p);
	p->drop();
	\endcode
	Please note that an affector is not necessary for the particle system to
	work.
	\param affector: New affector. */
	virtual void addAffector(IParticleAffector* affector, float lifeBgn = 0.f, float lifeEnd = 1.f) = 0;

	virtual bool removeAffector(IParticleAffector* affector) = 0;

	//! Removes all particle affectors in the particle system.
	virtual void clearAffectors() = 0;
};

} // end namespace scene
} // end namespace irr


#endif

