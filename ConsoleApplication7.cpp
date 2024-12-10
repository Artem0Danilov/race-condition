#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <chrono>

std::vector<int> numbers;  
std::mutex mtx;  

class Generator {
public:
    void generate() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            int random_number = rand() % 100 + 1;  

            mtx.lock();
            numbers.push_back(random_number);
            mtx.unlock();
        }
    }
};

class Consumer {
public:
    void consume() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));  

            int value = -1;

            mtx.lock();
            if (!numbers.empty()) {
                value = numbers.front();  
                numbers.erase(numbers.begin()); 
            }
            mtx.unlock();

            if (value != -1) {
                std::cout << value << std::endl;
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned>(std::time(0)));

    Generator generator;
    Consumer consumer1, consumer2;

    // Создаем потоки и запускаем их
    std::thread generator_thread(&Generator::generate, &generator);
    std::thread consumer_thread1(&Consumer::consume, &consumer1);
    std::thread consumer_thread2(&Consumer::consume, &consumer2);


    generator_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();

}
