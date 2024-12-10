#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

// Глобальный вектор для хранения чисел
std::vector<int> shared_vector;
std::mutex vector_mutex;  // Мьютекс для синхронизации доступа к вектору

// Функция для добавления чисел в вектор (например, постоянное добавление чисел от 1 до 10)
void generate() {
    for (int i = 1; i <= 100000; ++i) {  
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        vector_mutex.lock();
        shared_vector.push_back(i);
        vector_mutex.unlock();
    }
}

// Функция для потребления чисел из вектора
void consume(int id) {
    while (true) {  // Потребляем до тех пор, пока есть что потреблять
        int number = -1;

        vector_mutex.lock();
        if (!shared_vector.empty()) {
            number = shared_vector.front();
            shared_vector.erase(shared_vector.begin());  // Удаляем первое число
        }
        vector_mutex.unlock();

        if (number != -1) {
            // Потребитель обрабатывает число (с задержкой)
            std::cout << "Consumer " << id << " took " << number << " from vector\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300));  // Эмуляция обработки числа
            std::cout << "Consumer " << id << " processed " << number << "\n";
        }
    }
}

int main() {
    std::thread generator_thread(generate);

    // Запускаем 3 потока потребителей
    std::thread consumer_thread1(consume, 1);
    std::thread consumer_thread2(consume, 2);
    std::thread consumer_thread3(consume, 3);

    // Ждем завершения всех потоков
    generator_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();
    consumer_thread3.join();


    return 0;
}
