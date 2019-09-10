//
// Created by Andrei Nechaev on 2019-09-09.
//

#ifndef SPEAKER_QUEUE_H
#define SPEAKER_QUEUE_H

#include <deque>
#include <mutex>

constexpr size_t max_size = 1024;

template<class T>
class Queue {

public:
    Queue<T>(): deque_(max_size) {};

    virtual ~Queue() = default;

    void push(T val) {
        std::lock_guard<std::mutex> guard(w_mutex_);
        auto size = deque_.size();
        if (size > max_size) {
            deque_.erase(deque_.end() - size / 2);
        }
        deque_.push_front(val);
    }

    T pop() {
        if (deque_.empty()) {
            return nullptr;
        }

        std::lock_guard<std::mutex> guard(r_mutex_);
        return deque_.pop_back();
    }

    size_t size() const {
        return deque_.size();
    }


private:
    std::deque<T> deque_;
    std::mutex r_mutex_;
    std::mutex w_mutex_;
};

#endif //SPEAKER_QUEUE_H
