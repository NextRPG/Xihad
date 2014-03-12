#pragma once
#include <list>
#include "UpdateHandler.h"

namespace xihad { namespace ngn
{
	class Timeline;

	/// 组合更新对象
	/**
	 * start() update() 方法从前到后依次对子对象调用，而 stop() destroy() 方法则是逆序
	 * 对子对象调用
	 * 
	 * @author etnlGD
	 * @date 2013年12月13日 14:44:43
	 */
	class CompositeUpdateHandler : public UpdateHandler
	{
		typedef std::list<UpdateHandler*> Container;

	public:	// typedefs for iterator
		typedef Container::iterator iterator;
		typedef Container::const_iterator const_iterator;

	public:	// list container functions
		CompositeUpdateHandler();

		iterator childHandlerBegin() { return mChildHandlerList.begin(); }
		iterator childHandlerEnd()	 { return mChildHandlerList.end();	}
		iterator findChildHandler(UpdateHandler*);

		const_iterator childHandlerBegin() const { return mChildHandlerList.begin(); }
		const_iterator childHandlerEnd() const	 { return mChildHandlerList.end();	}
		const_iterator findChildHandler(UpdateHandler*) const;

		iterator eraseChildHandler(const_iterator where) { return mChildHandlerList.erase(where); }
		size_t getChildHandlerCount() const { return mChildHandlerList.size(); }
		bool hasChildHandler() const { return !mChildHandlerList.empty(); }

		/// 判断是否包含对象
		bool containsChildHandler(UpdateHandler* ) const;

		/// 附加更新对象
		/**
		 * handler 必须是一个非空并且没有被附加到其他 CompositeUpdateHandler 之上，才能够
		 * 成功附加。
		 * <p>如果本更新对象已经被 #start() ，那么当新的子对象被成功附加时，该子对象会立即被
		 * #start() 。
		 * <p>而如果将已经被 #start() 的对象附加到尚未 #start() 的组合更新对象，那么在组合
		 * 更新对象未 #start() 之前，该对象不会得到 #update() 的机会
		 * <p>出于效率问题，不会检查是否被添加过，用户需要自己保证这一点
		 * 
		 * @param handler 指定希望被附加的更新对象
		 * @return true if append
		 */ 
		virtual bool appendChildHandler(UpdateHandler*);

		/// 移除并摧毁子对象
		/**
		 * if child#destroy() return false then child won't be erased from this
		 * @see remove()
		 * @return true if handler is was contained and is #destroy()-ed now
		 */
		virtual bool destroyChildHandler(UpdateHandler* child);
		
	protected:	// template method for derived classes
		virtual ~CompositeUpdateHandler();

		virtual void onStart();

		virtual void onUpdate( const Timeline& tm );

		virtual void onStop();

		virtual void onDestroy();

	private:
		Container mChildHandlerList;
	};
}}

