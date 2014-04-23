#pragma once
#include <Engine/MemoryLeakDetector.h>

namespace xihad 
{
	/// Iterator pair of a container
	/**
	 * @author etnlGD
	 * @date 2013Äê12ÔÂ15ÈÕ 02:03:03
	 */
	template <typename Container>
	class IteratorPair
	{
	private:
		template <typename T>
		struct ConstSelector
		{
			typedef typename T::iterator _iter_type;
		};

		template <typename T>
		struct ConstSelector<const T>
		{
			typedef typename T::const_iterator _iter_type;
		};

	public:
		/// iterator type
		/**
		 * if the Container type is modified with const, then the iterator
		 * type should be Container::const_iterator, else Container::iterator
		 */
		typedef typename ConstSelector<Container>::_iter_type iterator;

		/// Construct from container, init cursor to container's begin position
		IteratorPair(Container& container) :
			curr(container.begin()), end(container.end())
		{
			XIHAD_MLD_NEW_OBJECT;
		}

		IteratorPair(const IteratorPair& other) :
			curr(other.curr), end(other.end) 
		{
			XIHAD_MLD_NEW_OBJECT;
		}

		~IteratorPair() 
		{
			XIHAD_MLD_DEL_OBJECT;
		}

		/// Test whether the iterator is not the end
		/**
		 * @return true if current itertor not equal to iterator end
		 */
		inline bool notEnd()
		{
			return curr != end;
		}

		/// Get the opposite result of notEnd()
		inline bool reachEnd()
		{
			return !(notEnd());
		}

		/// Move iterator to the next position
		/**
		 * @return always return true
		 */
		inline bool next()
		{
			++curr;
			return true;
		}

		/// Get pointer of currently pointed data
		inline typename iterator::pointer operator->()
		{
			return curr.operator->();
		}

		/// Get reference of currently pointed data
		inline typename iterator::reference operator*()
		{
			return *curr;
		}

		/// Get iterator of currently pointed data
		iterator currentPosition()
		{
			return curr;
		}

		/// Get the end iterator
		iterator endPosition()
		{
			return end;
		}

	private:
		iterator curr;
		iterator end;
	};

}
