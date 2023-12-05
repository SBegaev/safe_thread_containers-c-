// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include <algorithm>
#include <queue>
#include <mutex>

template <class T>
class safe_queue
{
public:
    void push(T t)
    {
        std::scoped_lock<std::mutex> lock(m_mtx);
        m_queue.push({ t });
    }

    bool try_pop(T &t)
    {
        std::scoped_lock<std::mutex> lock(m_mtx);

        if(m_queue.empty())
            return false;

        t = m_queue.front();
        m_queue.pop();
        return true;
    }

    bool empty() const
    {
        std::scoped_lock<std::mutex> lock(m_mtx);
        return m_queue.empty();
    }

    void clear()
    {
      std::scoped_lock<std::mutex> lock(m_mtx);
      std::queue<T> empty;
      std::swap(m_queue, empty);
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mtx;
};
