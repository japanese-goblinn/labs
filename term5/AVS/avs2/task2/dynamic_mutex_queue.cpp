//
// Created by Kirill Gorbachyonok on 11/3/19.
//

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <numeric>

class Queue {
    std::queue<uint8_t> q;
    std::mutex m;

public:
    void push(uint8_t val) {
        std::lock_guard<std::mutex> lock(m);
        q.push(val);
    }

    bool pop(uint8_t& val) {
        m.lock();
        if (q.empty()) {
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            m.lock();
        }
        if (q.empty()) {
            m.unlock();
            return false;
        }
        val = q.front();
        q.pop();
        m.unlock();
        return true;
    }
};

int main() {
    std::mutex write;
    std::vector<int> producerNums = {1, 2, 4};
    std::vector<int> consumerNums = {1, 2, 4};
    int taskNum = 4 * 1024 * 1024;

    for (int &producerNum: producerNums) {
        for (int &consumerNum: consumerNums) {
            std::vector<std::thread> producerThreads;
            producerThreads.reserve(producerNum);
            std::vector<std::thread> consumerThreads;
            consumerThreads.reserve(consumerNum);
            std::vector<int> sums(consumerNum, 0);
            Queue q;

            std::cout << "Amount of producers: " << producerNum << std::endl;
            std::cout << "Amount of consumers: " << consumerNum << std::endl;

            for (int i = 0; i < producerNum; i++) {
                producerThreads.emplace_back([i, &q, &taskNum, &write] {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < taskNum; i++) {
                        q.push(1);
                    }
                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                    write.lock();
                    std::cout << "Producer " << i + 1 << " time is " << duration.count()
                        << " milliseconds" << std::endl;
                    write.unlock();
                });
            }

            for (int i = 0; i < consumerNum; i++) {
                consumerThreads.emplace_back([i, &q, &sums, &write] {
                    auto start = std::chrono::high_resolution_clock::now();
                    int local_sum = 0;
                    uint8_t q_e;
                    while (true) {
                        if (q.pop(q_e)) {
                            local_sum += q_e;
                        } else {
                            sums[i]= local_sum;
                            auto stop = std::chrono::high_resolution_clock::now();
                            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                            write.lock();
                            std::cout << "Consumer " << i + 1 << " time is " << duration.count()
                                << " milliseconds" << std::endl;
                            write.unlock();
                            return;
                        }
                    }
                });
            }

            for (auto &t: producerThreads) {
                if (t.joinable()) {
                    t.join();
                }
            }
            for (auto &t: consumerThreads) {
                if (t.joinable()) {
                    t.join();
                }
            }

            std::cout << "Sum is " << std::accumulate(sums.cbegin(), sums.cend(), 0)
                      << " and it should be " << taskNum * producerNum << std::endl;
            std::cout << "\n";
        }
    }
}

