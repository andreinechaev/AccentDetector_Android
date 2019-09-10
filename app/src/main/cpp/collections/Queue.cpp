//
// Created by Andrei Nechaev on 2019-09-09.
//

#include "Queue.h"


//template<class T>
//void Queue<T>::push(T val) {
//    std::lock_guard<std::mutex> guard(w_mutex_);
//    if (deque_.size() > max_size) {
//        do {
//            deque_.pop_back();
//        } while (deque_.size() * 2 > max_size);
//    }
//    deque_.push_front(val);
//}
//
//template<class T>
//T Queue<T>::pop() {
//    if (deque_.empty()) {
//        return nullptr;
//    }
//
//    std::lock_guard<std::mutex> guard(r_mutex_);
//    return deque_.pop_back();
//}
//
//template<class T>
//Queue<T>::Queue() = default;
//
//template<class T>
//Queue<T>::~Queue() = default;


