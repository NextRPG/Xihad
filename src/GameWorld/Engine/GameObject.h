#pragma once
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/call_traits.hpp>
#include "CompositeUpdateHandler.h"
#include "matrix.h"
#include "vector3d.h"
#include "Transform.h"
#include "Properties.h"
#include "NullProperpties.h"

namespace xihad { namespace ngn
{
	struct GameObjectDepends;
	class GameScene;
	class Component;
	class Transform;
	/// 游戏中拥有位置信息的物体
	/** 
	 * 每个游戏对象可以有任意多个组件，游戏中的一切物体都可以是游戏对象，比如：英雄、敌人、
	 * 相机、灯光、剧情触发器、背景音乐。
	 * 
	 * <p>
	 * 所有游戏对象的共同点即是：拥有位置信息。对象中的很多组件都是通过此位置信息来协同
	 * 工作的；当然也有只是为了和对象绑定在一起的组件，比如角色的能力值组件。组件间的通信主要
	 * 是依赖 #findComponent() 实现。
	 * 
	 * <p>
	 * 游戏对象的职责主要包括：
	 *	- 维护组件集合
	 *	- 维护标签集合
	 *	- 游戏对象的层次结构
	 *	- 位置信息
	 * 
	 * @author etnlGD
	 * @date 2013年12月9日 20:42:23
	 */
	class GameObject : public CompositeUpdateHandler
	{
	public:
		typedef std::string Identifier;
		typedef std::string Tag;
		typedef boost::call_traits<Identifier>::param_type IdArgType;
		typedef boost::call_traits<Tag>::param_type TagArgType;
		typedef std::list<GameObject*>::iterator child_iterator;
		friend class child_iterator;

	public:
		/**
		 * @param depends 依赖项，可以在游戏对象之间共享该指针
		 * @param id 指定id
		 */
		GameObject(GameObjectDepends* depends, IdArgType id);
		
		/// 取得游戏对象所处场景
		virtual GameScene* getScene() const;

		/// 查找指定名称的组件
		/**
		 * 每个组件的类型名相当于类名，由于组件之间存在继承关系，所以，你可以指定一个父类的名称来
		 * 获取子类组件的指针。例如：组件A继承组件B并且向 GameObject 添加过 A 组件，那么
		 * findComponent("B") 会返回组件A的指针。
		 * <p>这里需要注意两点：
		 *	- 此处的继承并非C++中的继承关系，而是你可以在运行时指定的层次关系
		 *	- 因此，不能同时拥有两个继承自同一组件的组件，即多重继承的菱形关系是禁止的
		 * 
		 * @param componentName 指定组件类型名
		 * @return 指定类型的组件或其子类的指针，没有的话返回 NULL
		 */
		virtual Component* findComponent(const std::string& componentName);

		/// 找到拥有共同基类的组件
		/**
		 * 假设有这样的派生关系：A -> B, A -> C，并且向 GameObject 添加过C组件，那么
		 * findComponentHavingSameRoot("B") 返回C组件的指针
		 */
		virtual Component* findComponentHavingSameRoot(const std::string& componentName);

		/// 向对象添加组件
		/**
		 * 只需要指定类型名，引擎会去查找注册的工厂函数来生成相应的组件。只有当之前没有添加过这类
		 * 组件和有相应工厂才会创建组件并附加到游戏对象上
		 * 
		 * @param typeName 指定需要添加的组件类型名
		 * @param specificParam 指定组件构造需要的参数
		 * @return 如果 findComponentHavingSameRoot() 返回非空指针，那么返回该指针与false；
		 *		   否则，返回创建的组件指针，如果创建的指针非空，则第二返回值为true，否则为false
		 */
		virtual std::pair<Component*, bool> appendComponent(const std::string& typeName, 
				const Properties& specificParam = NullProperties());

		/// 从对象中删除组件
		/**
		 * 从对象中移除组件，移除之后，该组件将被摧毁
		 * 
		 * @param componentName 指定想要删除的组件类型
		 * @return true 如果确实存在这么一个组件，并被顺利删除
		 */
		virtual bool removeComponent(const std::string& componentName);

		/// 测试是否包含标签
		virtual bool hasTag(TagArgType tag) const;

		/// 向对象添加标签
		virtual void addTag(TagArgType tag);

		/// 从对象中移除标签
		virtual void removeTag(TagArgType tag);

		/// 删除所有标签
		virtual void clearTags();

		/// 取得对象的 ID 值
		/**
		 * 在同一个游戏场景中，所有对象的ID均不相同，并且一旦设置之后不可改变
		 * @return 对应的ID
		 */
		virtual IdArgType getID() const;

		/// 添加子节点
		/**
		 * @see setParant()
		 */
		void addChild(GameObject* child)
		{
			child->setParent(this);
		}

		/// 移除子节点
		/**
		 * 将子节点的父节点设置为 NULL
		 * @see setParent()
		 */
		void removeChild(GameObject* child)
		{
			if (child->getParent() == this)
			{
				child->setParent(nullptr);
			}
		}

		/// 判断父节点是否拥有标准变换
		/**
		 * 此方法用于优化某些操作，以减少矩阵操作，只能充当一个提示。
		 * 可以保证的是，如果父节点为 null ，那么肯定返回 true
		 * 
		 * @return  如果父节点的变换为标准变换（ 0 平移， 0 旋转， 1 缩放），那么不一定返回 true ；
		 *			但是如果父节点的变换不为标准变换，那么一定返回 false
		 */
		virtual bool identityParent() const;

		/// 设置父节点
		/**
		 * - 子节点是父节点中的一个子更新对象
		 * - 子节点的变换是相对父节点而言的
		 * - 如果没有父节点，那么不会得到更新的机会
		 * 
		 * @see UpdateHandler
		 */
		virtual void setParent(GameObject* parent);

		/// 取得父节点
		virtual GameObject* getParent() const;

		/// 指向第一个子节点的迭代器
		virtual child_iterator firstChild() const;

		/// 指向最后一个子节点后面的迭代器，你不能对这个迭代器解引用
		virtual child_iterator lastChild() const;

		/// 在当前变换上级联缩放
		void concatScale(const vector3df& concat)
		{
			resetScale(getScale()+concat);
		}

		/// 在当前变换上级联缩放
		void concatUniScale(float s)
		{
			concatScale(getScale()+s);
		}

		/// 在当前变换上级联旋转，角度制
		void concatRotate(const vector3df& concat)
		{
			resetRotate(getRotation()+concat);
		}

		/// 在当前变换上级联平移
		void concatTranslate(const vector3df& concat)
		{
			resetTranslate(getTranslate()+concat);
		}

		/**
		 * @see Transform::getScale()
		 */
		virtual vector3df getScale() const;
		
		/**
		 * @see Transform::getRotation()
		 */
		virtual vector3df getRotation() const;
		
		/**
		 * @see Transform::getTranslate
		 */
		virtual vector3df getTranslate() const;

		/**
		 * @see Transform::resetScale()
		 */
		virtual void resetScale(const vector3df& scale = vector3df(1,1,1));

		/**
		 * @see Transform::resetRotate()
		 */
		virtual void resetRotate(const vector3df& rotate = vector3df());

		/**
		 * @see Transform::resetTranslate()
		 */
		virtual void resetTranslate(const vector3df& trans = vector3df());

		/// 取得局部坐标系中的变换矩阵
		virtual const Matrix& getLocalTransformMatrix() const;

		/// 取得世界坐标系中的变换矩阵
		/**
		 * 此方法可能会很慢
		 * 
		 * @return transform matrix in world coordinate
		 */
		virtual const Matrix& getWorldTransformMatrix() const;

	protected:
		virtual void onStart();

		virtual void onUpdate( const Timeline& tm );

		virtual void onStop();

		virtual void onDestroy();

		virtual ~GameObject();

	private:
		void updateChildrenWorldMatrix();

	private:
		struct impl;
		Transform mTransform;
		boost::scoped_ptr<impl> mImpl;
	};
}}

