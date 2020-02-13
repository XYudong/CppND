#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

// concurrent message queue

template <class T>
class MessageQueue {
public:
  void push(const T& msg);
  void push(T&& msg);

  T pop();

  int getSize();

private:
  std::queue<T> _queue;
  const int MAX_NUM = 10;

  std::mutex _mtx;
  std::condition_variable _cond_min;
  std::condition_variable _cond_max;
};

template <class T>
void MessageQueue<T>::push(const T& msg) {
  std::unique_lock<std::mutex> ulck(_mtx);
  _cond_max.wait(ulck, [this]() {return _queue.size() <= MAX_NUM;});

  _queue.push(msg);
  ulck.unlock();
  _cond_min.notify_one();
  return;
}

template <class T>
void MessageQueue<T>::push(T&& msg) {
  std::unique_lock<std::mutex> ulck(_mtx);
  _cond_max.wait(ulck, [this]() {return _queue.size() <= MAX_NUM;});

  _queue.push(std::move(msg));
  // std::cout << "_queue size: " << _queue.size() <<std::endl;
  ulck.unlock();
  _cond_min.notify_one();
  return;
}

template <class T>
T MessageQueue<T>::pop() {
  std::unique_lock<std::mutex> ulck(_mtx);
  _cond_min.wait(ulck, [this]() {return !_queue.empty();});

  T msg = std::move(_queue.front());
  _queue.pop();
  ulck.unlock();
  _cond_max.notify_one();
  return msg;
}

template<class T>
int MessageQueue<T>::getSize() {
  std::lock_guard<std::mutex> lck(_mtx);
  return _queue.size();
}

#endif