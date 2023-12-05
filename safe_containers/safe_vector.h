// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include <mutex>
#include <vector>

template <class T>
class safe_vector
{
public:

	T at(size_t index)
	{
    std::scoped_lock<std::mutex> lock(m_mtx);
    return m_vec.at(index);
	}

	T operator[](size_t index)
	{
    std::scoped_lock<std::mutex> lock(m_mtx);
    return m_vec[index];
	}

	void push_back(T value)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		m_vec.push_back(std::move(value));
	}

	T front()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.front();
	}

	T back()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.back();
	}

	typename std::vector<T>::iterator begin()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.begin();
	}

	typename std::vector<T>::iterator end()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.end();
	}

	void erase(const typename std::vector<T>::iterator& first,
		         const typename std::vector<T>::iterator& last)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		m_vec.erase(first, last);
	}
	
	void clear()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		m_vec.clear();
	}

	bool empty() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.empty();
	}

	size_t size() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_vec.size();
	}

private:
	std::vector<T> m_vec;
	mutable std::mutex m_mtx;
};