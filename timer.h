#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        return duration.count() / 1000.0;  // Convert milliseconds to seconds
    }

    // setTimeout function
    void setTimeout(std::function<void()> callback, int delay) {
        reset();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    }

    // setInterval function
    void setInterval(std::function<void()> callback, int interval) {
        reset();
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            callback();
            reset();
        }
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
