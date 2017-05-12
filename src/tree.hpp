///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_TREE_HPP
#define FORCE3_TREE_HPP

#include <cstddef>
#include <limits>
#include <vector>
#include <functional>
#include <memory>
#include <cassert>

namespace details { // private api
	template <typename T>
	class TreeNode {
	public:
		TreeNode()
				: parent{nullptr}, firstChild{nullptr}, lastChild{nullptr}, prevSibling{nullptr}, nextSibling{nullptr},
				  data{} {
		}

		TreeNode(const T& value)
				: parent{nullptr}, firstChild{nullptr}, lastChild{nullptr}, prevSibling{nullptr}, nextSibling{nullptr},
				  data{value} {
		}

		TreeNode(T&& value)
				: parent{nullptr}, firstChild{nullptr}, lastChild{nullptr}, prevSibling{nullptr}, nextSibling{nullptr},
				  data{std::move(value)} {
		}

		TreeNode<T>* parent;

		TreeNode<T>* firstChild;

		TreeNode<T>* lastChild;

		TreeNode<T>* prevSibling;

		TreeNode<T>* nextSibling;

		T data;

		std::size_t depth;
	};
}

/**
 * @brief Dynamic generic tree.
 * @tparam T Type of element.
 * @tparam Allocator Allocator type
 *
 */
template <typename T, typename Allocator = std::allocator<details::TreeNode<T>>>
class Tree {
public:
	template <typename U>
	using TreeNode = details::TreeNode<U>;

	typedef typename Allocator::value_type              value_type;
	typedef std::size_t                                 size_type;
	typedef std::ptrdiff_t                              difference_type;
	typedef value_type&                                 reference;
	typedef const reference                             const_reference;
	typedef value_type*                                 pointer;
	typedef const pointer                               const_pointer;

	/**
	 * @brief Default constructor
	 */
	Tree()
			: m_allocator{}
			, m_dummyRoot{m_allocator.allocate(1)}
			, m_dummyLast{m_allocator.allocate(1)} {

		new (m_dummyRoot) TreeNode<T>();
		new (m_dummyLast) TreeNode<T>();

		m_dummyRoot->nextSibling = m_dummyLast;
		m_dummyLast->prevSibling = m_dummyRoot;
	};

	/**
	 * @brief Copy constructor
	 */
	Tree(const Tree<T, Allocator>& tree) noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
		// TODO
	}

	/**
	 * @brief Move constructor
	 */
	Tree(Tree<T, Allocator>&& tree)
			: m_dummyRoot(std::move(tree.m_dummyRoot))
			, m_dummyLast(std::move(tree.m_dummyLast)) {
	}

	/**
	 * @brief Destructor
	 */
	~Tree() {
		if (m_dummyRoot->nextSibling != m_dummyLast) {
			erase(pre_order_iterator(m_dummyRoot->nextSibling));
		}

		m_dummyRoot->~TreeNode();
		m_dummyLast->~TreeNode();

		m_allocator.deallocate(m_dummyLast, 1);
		m_allocator.deallocate(m_dummyRoot, 1);
	}

private:
	struct base_iterator; // Forward declaration

public:
	/**
	 * @brief Iterator to iterate on the child of a node
	 */
	struct sibling_iterator : base_iterator {
		sibling_iterator(TreeNode<T>* node) : base_iterator(node) {
		}

		bool operator==(const sibling_iterator& it) const {
			return this->m_node == it.m_node;
		}

		bool operator!=(const sibling_iterator& it) const {
			return !operator==(it);
		}

		sibling_iterator&  operator++() {
			this->m_node = this->m_node->nextSibling;

			return *this;
		}

		sibling_iterator&  operator--() {
			this->m_node = this->m_node->prevSibling;

			return *this;
		}

		sibling_iterator   operator++(int) {
			sibling_iterator it = *this;

			return ++it;
		}
		sibling_iterator   operator--(int) {
			sibling_iterator it = *this;

			return --it;
		}

		sibling_iterator&  operator+=(unsigned int step) {
			while (step--) {
				++(*this);
			}

			return *this;
		}

		sibling_iterator&  operator-=(unsigned int step) {
			while (step--) {
				--(*this);
			}

			return *this;
		}
	};

	/**
	 * @brief Pre-order traversal iterator.
	 */
	struct pre_order_iterator : base_iterator {
		pre_order_iterator(TreeNode<T>* node)
				: base_iterator(node) {
		}

		bool operator==(const pre_order_iterator& it) const {
			return this->m_node == it.m_node;
		}

		bool operator!=(const pre_order_iterator& it) const {
			return !(operator==(it));
		}

		pre_order_iterator& operator++() {
			if(this->m_node->firstChild != nullptr) {
				this->m_node = this->m_node->firstChild;
			} else {
				while(this->m_node->nextSibling == nullptr) {
					this->m_node = this->m_node->parent;
					assert(this->m_node != nullptr); // invalid iterator
				}

				this->m_node = this->m_node->nextSibling;
			}

			return *this;
		}

		pre_order_iterator& operator--() {
			if(this->m_node->prevSibling != nullptr) {
				this->m_node = this->m_node->prevSibling;

				while(this->m_node->lastChild != nullptr) {
					this->m_node = this->m_node->lastChild;
				}
			} else {
				this->m_node = this->m_node->parent;
				assert(this->m_node != nullptr); // invalid iterator
			}

			return *this;
		}

		pre_order_iterator operator++(int) const {
			pre_order_iterator tmp(*this);

			return ++tmp;
		}

		pre_order_iterator operator--(int) const {
			pre_order_iterator tmp(*this);

			return --tmp;
		}
	};

	/**
	 * @brief Fixed depth pre-order traversal iterator.
	 */
	struct fixed_depth_pre_order_iterator : base_iterator {
		fixed_depth_pre_order_iterator(TreeNode<T>* node)
				: base_iterator{node}
				, m_depth{}
				, m_relativeDepth{0}
				, m_firstPass{true}
				, m_startingNode{} {
		}

		fixed_depth_pre_order_iterator(TreeNode<T>* node, size_type depth)
				: base_iterator{node}
				, m_depth{depth}
				, m_relativeDepth{0}
				, m_firstPass{true}
				, m_startingNode{node} {
		}

		bool operator==(const fixed_depth_pre_order_iterator& it) const {
			if (m_firstPass && it.m_node == this->m_node) {
				m_firstPass = false;
				return false;
			} else {
				return it.m_node == this->m_node;
			}
		}

		bool operator!=(const fixed_depth_pre_order_iterator& it) const {
			return !(operator==(it));
		}

		fixed_depth_pre_order_iterator& operator++() {
			if(this->m_node->firstChild != nullptr && m_relativeDepth < m_depth) {
				this->m_node = this->m_node->firstChild;
				++m_relativeDepth;
			} else {
				while(this->m_node->nextSibling == nullptr) {
					--m_relativeDepth;
					this->m_node = this->m_node->parent;

					if (this->m_node == m_startingNode) {
						return *this;
					}

					assert(this->m_node != nullptr); // invalid iterator
				}

				this->m_node = this->m_node->nextSibling;
			}

			return *this;
		}
	protected:
		size_type m_depth;
		size_type m_relativeDepth;
		mutable bool m_firstPass;
		TreeNode<T>* m_startingNode;
	};

	/**
	 * Returns a read/write iterator on the first node of the tree.
	 * @tparam It Iterator type (default pre_order_iterator)
	 * @return an iterator on the root of the tree.
	 */
	template <typename It = pre_order_iterator>
	It begin() {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #begin_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyRoot->nextSibling);
	}

	template <typename It = pre_order_iterator>
	It begin() const {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #begin_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyRoot->nextSibling);
	}

	template <typename It = pre_order_iterator>
	It cbegin() const {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #begin_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyRoot->nextSibling);
	}

	/**
	 * @brief Returns a read/write iterator to one past the last node of the tree.
	 * @tparam It Iterator type (default pre_order_iterator)
	 * @return an iterator to one past the last node of the tree.
	 */
	template <typename It = pre_order_iterator>
	It end() {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #end_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyLast);
	}

	template <typename It = pre_order_iterator>
	It end() const {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #end_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyLast);
	}

	template <typename It = pre_order_iterator>
	It cend() const {
		static_assert(!std::is_same<It, sibling_iterator>::value, "You must use #end_children method with a sibling_iterator.");
		return pre_order_iterator(m_dummyLast);
	}

	/**
	 * @brief Returns an iterator pointing to the specified it.
	 * @tparam It Iterator type
	 * @param it a valid iterator.
	 * @param depth limit depth of the iteration.
	 * @return A fixed depth pre_order_iterator.
	 */
	template <typename It>
	fixed_depth_pre_order_iterator begin(const It& it, size_type depth) {
		return fixed_depth_pre_order_iterator(it.m_node, depth);
	}

	template <typename It>
	fixed_depth_pre_order_iterator begin(const It& it, size_type depth) const {
		return fixed_depth_pre_order_iterator(it.m_node, depth);
	}

	template <typename It>
	fixed_depth_pre_order_iterator cbegin(const It& it, size_type depth) const {
		return fixed_depth_pre_order_iterator(it.m_node, depth);
	}

	/**
	 * @brief Returns a read/write iterator to one past the last element of the iteration.
	 * @tparam It Iterator type
	 * @param it the same iterator passed to #begin(const It& it, size_type depth)
	 * @return an iterator
	 */
	template <typename It>
	fixed_depth_pre_order_iterator end(const It& it) {
		return fixed_depth_pre_order_iterator(it.m_node);
	}

	template <typename It>
	fixed_depth_pre_order_iterator end(const It& it) const {
		return fixed_depth_pre_order_iterator(it.m_node);
	}

	template <typename It>
	fixed_depth_pre_order_iterator cend(const It& it) const {
		return fixed_depth_pre_order_iterator(it.m_node);
	}

	/**
	 * @brief Returns a read/write iterator to the first children of the given node.
	 * @tparam It Iterator type
	 * @param it a valid iterator.
	 * @return an iterator.
	 */
	template <typename It = pre_order_iterator>
	sibling_iterator begin_children(const It& it = cbegin()) {
		static_assert(!std::is_same<It, sibling_iterator>::value, "It must not be a sibling_iterator");

		if (it.m_node->firstChild != nullptr) {
			return sibling_iterator(it.m_node->firstChild);
		}

		return end_children(it);
	}

	template <typename It = pre_order_iterator>
	sibling_iterator end_children(const It& it = cbegin()) {
		static_assert(!std::is_same<It, sibling_iterator>::value, "It must not be a sibling_iterator");

		sibling_iterator childIterator(nullptr);

		childIterator.m_node->parent = it.m_node;

		return childIterator;
	}

	/**
	 * @brief Insert value before the given iterator.
	 * @tparam It Iterator type
	 * @param it an iterator
	 * @param value
	 * @return an iterator pointing to the new node
	 */
	template <typename It>
	It insert(const It& it, const T& value) {
		TreeNode<T>* node = m_allocator.allocate(1);
		new(node) TreeNode<T>(value);

		node->parent = it.m_node->parent;
		node->nextSibling = it.m_node;
		node->prevSibling = it.m_node->prevSibling;
		it.m_node->prevSibling = node;

		if (node->prevSibling == nullptr) {
			if (node->parent != nullptr) {
				node->parent->firstChild = node;
			}
		} else {
			node->prevSibling->nextSibling = node;
		}

		return It(node);
	}

	template <typename It>
	It insert(const It& it, T&& value) {
		return emplace(it, std::move(value));
	}

	/**
	 * @brief Construct the value before the given iterator
	 * @tparam It Iterator type
	 * @tparam Args Arguments Types
	 * @param it an iterator
	 * @param args constructor arguments
	 * @return an iterator pointing to the new node
	 */
	template <typename It, typename... Args>
	It emplace(const It& it, Args&&... args) {
		TreeNode<T>* node = m_allocator.allocate(1);
		new(node) TreeNode<T>(std::move(args...));

		node->parent = it.m_node->parent;
		node->nextSibling = it.m_node;
		node->prevSibling = it.m_node->prevSibling;
		it.m_node->prevSibling = node;

		if (node->prevSibling == nullptr) {
			if (node->parent != nullptr) {
				node->parent->firstChild = node;
			}
		} else {
			node->prevSibling->nextSibling = node;
		}

		return It(node);
	}

	/**
	 * @brief Add a child at the end of the child list of the given element.
	 * @tparam It Iterator type
	 * @param it an iterator
	 * @param value
	 * @return An iterator pointing to the new child
	 */
	template <typename It>
	It addChild(const It& it, const T& value) {
		assert(m_dummyRoot->nextSibling != m_dummyLast);

		TreeNode<T>* node = m_allocator.allocate(1);
		new(node) TreeNode<T>(value);

		node->parent = it.m_node;
		if (it.m_node->lastChild != nullptr) {
			it.m_node->lastChild->nextSibling = node;
		} else {
			it.m_node->firstChild = node;
		}

		node->prevSibling = it.m_node->lastChild;
		it.m_node->lastChild = node;

		return It(node);
	}

	template <typename It>
	It addChild(const It& it, T&& value) {
		assert(m_dummyRoot->nextSibling != m_dummyLast);

		TreeNode<T>* node = m_allocator.allocate(1);
		new(node) TreeNode<T>(value);

		node->parent = it.m_node;
		if (it.m_node->lastChild != nullptr) {
			it.m_node->lastChild->nextSibling = node;
		} else {
			it.m_node->firstChild = node;
		}

		node->prevSibling = it.m_node->lastChild;
		it.m_node->lastChild = node;

		return It(node);
	}

	template <typename It, typename... Args>
	It addChildren(const It& it, Args&&... children) {
		return addChildren_impl(it, std::forward<Args>(children)...);
	}

	/**
	 * @brief Removes the specified element and its children.
	 * @tparam It Iterator type
	 * @param it a valid iterator
	 * @return an iterator on the next element.
	 */
	template <typename It>
	It erase(const It& it) {
		assert(it.m_node != nullptr && it.m_node != m_dummyLast);

		It next = it;

		TreeNode<T>* node = it.m_node;
		eraseChildren(it);

		if (node->parent != nullptr) {
			if (node->nextSibling == nullptr) {
				node->parent->lastChild = node->prevSibling;
			} else {
				node->nextSibling->prevSibling = node->prevSibling;
			}

			if (node->prevSibling == nullptr) {
				node->parent->firstChild = node->nextSibling;
			} else {
				node->prevSibling->nextSibling = node->nextSibling;
			}

			++next;
		} else {
			m_dummyRoot->nextSibling = m_dummyLast;
			m_dummyLast->prevSibling = m_dummyRoot;
		}

		m_allocator.deallocate(node, 1);


		return next;
	}

	/**
	 * @brief Removes children of the specified element.
	 * @tparam It Iterator type.
	 * @param it a valid iterator.
	 */
	template <typename It>
	void eraseChildren(const It& it) {
		assert(it.m_node != nullptr);

		TreeNode<T>* current = it.m_node->firstChild;
		TreeNode<T>* previous = nullptr;

		while (current != nullptr) {
			previous = current;
			current = current->nextSibling;

			eraseChildren(It(previous));
			previous->~TreeNode();
			m_allocator.deallocate(previous, 1);
		}

		it.m_node->firstChild = nullptr;
		it.m_node->lastChild = nullptr;
	}

	/**
	 * @brief Returns the root
	 *
	 * If the tree is empty, the iterator isn't defined.
	 *
	 * @tparam It Iterator type (default pre_order_iterator)
	 * @return
	 */
	template <typename It = pre_order_iterator>
	It root() const {
		assert(m_dummyRoot->nextSibling != m_dummyLast);
		return It(m_dummyRoot->nextSibling);
	}

	/**
	 * @brief Returns the depth of the node pointed by the iterator.
	 * @tparam It Iterator type
	 * @param it a valid iterator
	 * @return the depth of the node pointed by the iterator.
	 */
	template <typename It>
	static size_type depth(const It& it) {
		assert(it.m_node != nullptr);

		TreeNode<T>* node = it.m_node;

		size_type ret = 0;
		while(node->parent!=0) {
			node = node->parent;
			++ret;
		}

		return ret;
	}

	/**
	 * @brief Checks if the collection is empty
	 * @return true if the tree is empty, false otherwise.
	 */
	bool empty() const {
		return m_dummyRoot->nextSibling != nullptr;
	}

private:
	struct base_iterator {
		typedef typename Allocator::value_type              value_type;
		typedef std::size_t                                 size_type;
		typedef std::ptrdiff_t                              difference_type;
		typedef value_type&                                 reference;
		typedef const reference                             const_reference;
		typedef value_type*                                 pointer;
		typedef const pointer                               const_pointer;
		typedef std::bidirectional_iterator_tag             iterator_category;

	protected:
		base_iterator(TreeNode<T>* node)
				: m_node{node}
				, m_depthLevel{0} {
		}

	public:
		T& operator*() const {
			return m_node->data;
		}

		T* operator->() const {
			return &(operator*());
		}

	protected:
		TreeNode<T>* m_node;
		size_type m_depthLevel;

		template <typename, typename>
		friend class Tree;
	};

	template <typename It, typename First, typename... Args>
	It addChildren_impl(const It& it, First&& first, Args&& ... values) {
		addChild(it, std::forward<First>(first));
		return addChildren_impl(it, std::forward<Args>(values)...);
	}

	template <typename It, typename First>
	It addChildren_impl(const It& it, First&& first) {
		return addChild(it, std::forward<First>(first));
	}

	Allocator m_allocator;

	TreeNode<T>* m_dummyRoot;
	TreeNode<T>* m_dummyLast;
};

#endif //FORCE3_TREE_HPP
