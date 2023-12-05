#pragma once

#include <unordered_map>
#include <mutex>

template <class K, class V>
class safe_unordered_map
{
public:

	V at(const K& key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_map.at(key);
	}

	void insert(const K& key, V value)
	{
		std::unique_lock<std::mutex> ul(m_mtx);
		m_map[key] = std::move(value);
		ul.unlock();
		cv.notify_one();
	}

	bool erase(const K& key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_map.erase(key);
	}

	bool contains(const K& key)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		if (m_map.find(key) != m_map.end())
		{
			return true;
		}
		return false;
	}

	bool find_keys_by_value(std::vector<K>& keys, const V& value)
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		for (const auto& it : m_map)
		{
			if (it.second == value)
			{
				keys.push_back(it.first);
			}
		}
		if (keys.empty()) return false;
			
		return true;
	}

	bool empty() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_map.empty();
	}

	void clear()
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		m_map.clear();
	}

	size_t size() const
	{
		std::scoped_lock<std::mutex> lock(m_mtx);
		return m_map.size();
	}

private:
	std::unordered_map<K, V> m_map;
	mutable std::mutex m_mtx;
	std::condition_variable cv;
};