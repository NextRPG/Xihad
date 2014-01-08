#pragma once
#include <list>
#include "ManagedUpdateHandler.h"
#include "ICompositeUpdateHandler.h"
#include "Destroyer.h"
#pragma warning(disable: 4250)

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
	class CompositeUpdateHandler : 
		public ManagedUpdateHandler, 
		public virtual ICompositeUpdateHandler,
		private virtual Destroyer
	{
	public:
		typedef std::list<UpdateHandler*>::iterator iterator;
		typedef std::list<UpdateHandler*>::const_iterator const_iterator;

		/// 取得第一个子更新对象
		iterator begin()
		{
			return mUpdateImpls.begin();
		}

		/// 取得子更新对象的结尾
		iterator end()
		{
			return mUpdateImpls.end();
		}

		/// 取得第一个子更新对象
		const_iterator begin() const
		{
			return mUpdateImpls.begin();
		}

		/// 取得子更新对象的结尾
		const_iterator end() const
		{
			return mUpdateImpls.end();
		}

		virtual bool containsUpdateHandler(UpdateHandler* handler) const override;

		virtual bool appendUpdateHandler(UpdateHandler* handler) override;

		virtual bool removeUpdateHandler(UpdateHandler* handler) override;

	protected:
		virtual ~CompositeUpdateHandler() { }

		virtual void onStart();

		virtual void onUpdate( const Timeline& tm );

		virtual void onStop();

		virtual void onDestroy();

	private:
		virtual bool isDestroying() override;
		virtual void onChildDestroy(Destroyable* toDestroty) override;
		std::list<UpdateHandler*>::const_iterator iteratorFor(UpdateHandler* handler) const;

	private:
		std::list<UpdateHandler*> mUpdateImpls;
		bool mDestroying;
	};
}}

