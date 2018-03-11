#pragma once

#include<memory>

template<typename T>
struct slist_node
{
	T              data;
	slist_node<T>* next;

	slist_node(const T& t) : data{ t }, next{ nullptr } {}
	slist_node(T&& t) : data{ std::move(t) }, next{ nullptr } {}
};

/* support queue */
template<typename T>
class slist
{
public:
	typedef slist this_type;
	typedef slist type;
	typedef slist_node<T> node_type;
private:
	node_type * head;
	size_t      length;
public:



};

/* support queue */
template<typename T>
class qlist
{
public:
	typedef qlist                this_type;
	typedef qlist                type;
	typedef slist_node<T>        node_type;
private:
	node_type * head;
	node_type * tail;
	size_t      length;

	this_type& set_first_node(node_type* node, size_t n)
	{
		head = tail = node;
		length = n;
		return *this;
	}

public:

	/*default */
	constexpr qlist() = default;

	this_type& push_back(T const& t) 
	{
		node_type* new_node = new node_type{ t };
		if (length == 0) return set_first_node(new_node, 1);
		tail->next = new_node;
		tail = new_node;
		++length;
		return *this;
	}

	this_type& push_front(T const& t)
	{
		node_type* new_node = new node_type{ t };
		if (length == 0) return set_first_node(new_node, 1);
		new_node->next = head;
		head = new_node;
		++length;
		return *this;
	}

	this_type& pop_front()
	{
		if (length == 0) return *this;
		else if (length == 1) {
			delete head;
			return set_first_node(nullptr, 0);
		}
		node_type* next = head->next;
		delete head;
		head = next;
		--length;
	}

};
