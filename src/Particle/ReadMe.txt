/**
 * 自定义 Affector 的步骤（假设为 PauseAffector）：
 * 1. 实现 IParticleAffector 子类 ParticlePauseAffector，定义默认构造函数
 * 
 * 2. 向 Lua 中注册该类。需要在合适的 cpp 文件中添加如下代码（可以是 ParticlePauseAffector.cpp / xxx_metadata.cpp）
 *	<p>
 *		#include <luaT/luaT.h>
 *		#include "ParticlePauseAffector.h"
 *	
 * 3. 导出新类的成员函数供 Lua 在脚本中调用。假设类成员函数的参数、返回值都是原始类型或vector3df等，代码示例如下：
 *	<p>
 *		luaT_defRegsBgn(arbitrary_name)
 *			luaT_mnamedfunc(ParticlePauseAffector, member_function_name),
 *			// ....,
 *			// ....,
 *		luaT_defRegsEnd
 *		luaT::MetatableFactory<ParticlePauseAffector, IPartilceAffector>::create(L, arbitrary_name);
 *	<p> 
 *		可以参考 luaopen_ParticleSystem.cpp 中的格式
 *		
 *		
 * 4. 通过如下代码注册新的 Affector
 *	<p>
 *		IStackPusherFactory* f = new DefaultStackPusherFactory<ParticlePauseAffector>;
 *		receiver->factory->Affectors["Pause"] = f;
 *		f->drop();
 */