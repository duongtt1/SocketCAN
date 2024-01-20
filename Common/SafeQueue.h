#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <stdexcept>

/**
 * @brief A thread-safe queue with a maximum size.
 * 
 * This class provides a thread-safe queue implementation with a specified maximum size.
 * It ensures that the queue size is maintained within the defined limit.
 * 
 * @tparam T Type of the elements stored in the queue.
 */
template <typename T> 
class SafeQueue {
public:
    /**
     * @brief Constructs a SafeQueue with a specified maximum size.
     * 
     * @param maxSize The maximum size of the queue.
     */
    explicit SafeQueue(size_t maxSize) : maxSize(maxSize) {}

    /**
     * @brief Pushes a value into the queue.
     * 
     * If the size of the queue exceeds the maximum size, the oldest element is removed.
     * 
     * @param value The value to be pushed into the queue.
     */
    void push(T value) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (dataQueue.size() >= maxSize) {
            dataQueue.pop();
        }
        dataQueue.push(std::move(value));
        lock.unlock();
        queueCondVar.notify_one();
    }

    /**
     * @brief Pops a value from the queue.
     * 
     * Waits until the queue is not empty before popping a value.
     * 
     * @return The popped value from the queue.
     */
    T pop() {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondVar.wait(lock, [this] { return !dataQueue.empty(); });

        T value = std::move(dataQueue.front());
        dataQueue.pop();

        return value;
    }

    /**
     * @brief Checks if the queue is empty.
     * 
     * @return True if the queue is empty, false otherwise.
     */
    bool empty() const {
        std::unique_lock<std::mutex> lock(queueMutex);
        return dataQueue.empty();
    }

    /**
     * @brief Checks if the queue is fully filled to its maximum size.
     * 
     * @return True if the queue is fully filled, false otherwise.
     */
    bool fully() const {
        std::unique_lock<std::mutex> lock(queueMutex);
        return dataQueue.size() == maxSize;
    }

    /**
     * @brief Gets the current size of the queue.
     * 
     * @return The current size of the queue.
     */
    size_t size() const {
        std::unique_lock<std::mutex> lock(queueMutex);
        return dataQueue.size();
    }

private:
    std::queue<T>           dataQueue;           /**< The underlying data queue. */
    mutable std::mutex      queueMutex;          /**< Mutex to ensure thread safety. */
    std::condition_variable queueCondVar;        /**< Condition variable for synchronization. */
    size_t                  maxSize;             /**< The maximum size of the queue. */
};

#endif // SAFEQUEUE_H
