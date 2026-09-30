// #include <chrono>
// struct ScopedTimer {
//     const char* name;
//     std::chrono::steady_clock::time_point t0{std::chrono::steady_clock::now()};
//     ~ScopedTimer() {
//         auto ns = std::chrono::duration<double,std::nano>(
//                     std::chrono::steady_clock::now() - t0).count();
//         printf("[%s] %.0f ns\n", name, ns);
//     }
// };
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

class Timer {
public:
    explicit Timer(std::string name = "Timer")
        : name_(std::move(name)), start_(std::chrono::steady_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start_;
        std::cout << "[" << name_ << "] Elapsed time: " 
                  << elapsed.count() << " ms\n";
    }

    // RAII guards shouldn't be copied or moved around scope bounds
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

private:
    std::string name_;
    std::chrono::steady_clock::time_point start_;
};

int main() {
    {
        Timer t("Computation Task");
        
        volatile double sum = 0;
        for (int i = 0; i < 1'000'000; ++i) {
            sum += i;
        }
    } // ~Timer() called automatically when 't' goes out of scope

    {
        Timer t("Sleep Task");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } // ~Timer() called automatically here

    return 0;
}