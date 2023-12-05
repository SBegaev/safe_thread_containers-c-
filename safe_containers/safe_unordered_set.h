// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include <mutex>
#include <optional>
#include <unordered_set>
#include <vector>
#include <condition_variable>

template <class K>
class safe_unordered_set
{
public:

	std::optional<K> find(const K &key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		if (auto search = m_set.find(key); search != m_set.end())
		{
			return *search;
		}
		return std::nullopt;
	}

	void insert(const K &key)
	{
		std::unique_lock<std::mutex> ul(m_mtx);
		m_set.insert(key);
		ul.unlock();
		cv.notify_one();
	}

	bool erase(const K &key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_set.erase(key);
	}

	bool contains(const K &key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		if (m_set.find(key) != m_set.end())
		{
			return true;
		}
		return false;
	}

	std::vector<K> get_all()
	{
		std::vector<std::string> data;
		for (auto file : m_set)
		{
			data.push_back(std::move(file));
		}
		return data;
	}

	bool empty() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_set.empty();
	}

	void clear()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		m_set.clear();
	}

	size_t size() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_set.size();
	}

private:
	std::unordered_set<K> m_set;
	mutable std::mutex m_mtx;
	std::condition_variable cv;
};