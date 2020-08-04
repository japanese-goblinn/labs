//
// Created by Kirill Gorbachyonok on 11/3/19.
//

#include <iostream>
#include <vector>
#include <thread>

void atomic_counter(int numTasks, int numThreads, bool withSleep) {
    std::atomic<int> globalCount = 0;
    std::vector<uint8_t> values(numTasks, 0);
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&] {
            int local_count = globalCount.fetch_add(1);
            while (local_count < numTasks) {
                values.at(local_count)++;
                local_count = globalCount.fetch_add(1);
                if (withSleep) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(10));
                }
            }
        });
    }
    for (auto &thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Time taken by function with " << numThreads << " threads" << ": "
              << duration.count() << " milliseconds" << std::endl;
    if (std::all_of(values.cbegin(), values.cend(), [](int i) {
        return i == 1;
    })) {
        std::cout << "All are 1" << std::endl;
    } else {
        std::cout << "Not all are 1" << std::endl;
    }
}

int main() {
    int boo = 1024 * 1024;

    atomic_counter(boo, 4, false);
    atomic_counter(boo, 8, false);
    atomic_counter(boo, 16, false);
    atomic_counter(boo, 32, false);

    atomic_counter(boo, 4, true);
    atomic_counter(boo, 8, true);
    atomic_counter(boo, 16, true);
    atomic_counter(boo, 32, true);

    return 0;
}
