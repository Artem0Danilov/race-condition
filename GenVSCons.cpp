#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

std::vector<int> numbers;
std::mutex mtx;

class generator {
public:
    void run() {
        int counter = 0;
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mtx.lock();
            numbers.push_back(counter++);
            std::cout << "Generator added: " << counter - 1 << std::endl;
            mtx.unlock();
        }
    }
};

class consumer {
public:
    void run() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            mtx.lock();
            if (!numbers.empty()) {
                int num = numbers.front();
                numbers.erase(numbers.begin());
                std::cout << "Consumer consumed: " << num << std::endl;
            }
            mtx.unlock();
        }
    }
};

void start_generator() {
    generator gen;
    gen.run();
}

void start_consumer() {
    consumer cons;
    cons.run();
}

int main() {
    std::thread gen_thread(start_generator);
    std::thread consumer_thread1(start_consumer);
    std::thread consumer_thread2(start_consumer);
    std::thread consumer_thread3(start_consumer);

    gen_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();
    consumer_thread3.join();
}
