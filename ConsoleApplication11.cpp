#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

// Глобальная переменная вектор целых чисел
std::vector<int> globalVector;

// Мьютекс для синхронизации доступа к вектору
std::mutex vectorMutex;

// Класс для генерации чисел и добавления их в глобальный вектор
class Generator {
public:
    void operator()() {
        int number = 0;  // Начальное число
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Пауза 100 миллисекунд

            // Добавление числа в вектор с синхронизацией
            std::lock_guard<std::mutex> lock(vectorMutex); // Защита от состояния гонки
            globalVector.push_back(number++);
            std::cout << "Generated: " << number - 1 << std::endl;  // Выводим сгенерированное число
        }
    }
};

// Класс для потребления чисел из глобального вектора
class Consumer {
public:
    void operator()(int consumerId) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));  // Пауза 300 миллисекунд

            std::lock_guard<std::mutex> lock(vectorMutex); // Защита от состояния гонки

            if (!globalVector.empty()) {
                int number = globalVector.front();  // Берем первое число
                globalVector.erase(globalVector.begin());  // Удаляем первое число из вектора

                // Обрабатываем и выводим его
                std::cout << "Consumer " << consumerId << " consumed and processed: " << number << std::endl;
            }
        }
    }
};

int main() {
    Generator generator;  // Создаем объект генератора
    Consumer consumer1, consumer2, consumer3;  // Создаем 3 объекта потребителей

    // Создаем потоки для генератора и потребителей
    std::thread generatorThread(generator);
    std::thread consumerThread1(consumer1, 1);  // Потребитель 1
    std::thread consumerThread2(consumer2, 2);  // Потребитель 2
    std::thread consumerThread3(consumer3, 3);  // Потребитель 3

    // Ожидаем завершения потоков (хотя в данном случае они будут работать бесконечно)
    generatorThread.join();
    consumerThread1.join();
    consumerThread2.join();
    consumerThread3.join();

    return 0;
}
