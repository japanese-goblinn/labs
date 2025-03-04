//
// Created by Kirill Gorbachyonok on 11/5/19.
//

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <numeric>


class FixedQueue {

    int size;
    int mask;
    alignas(64) std::atomic<int> FRONT;
    alignas(64) std::atomic<uint8_t>* buffer;

public:

    alignas(64) std::atomic<int> REAR;

    FixedQueue(int size) {
        this->size = size;
        this->mask = size - 1;
        buffer = new std::atomic<uint8_t>[size]();
        FRONT = 0;
        REAR = 0;
    }

    ~FixedQueue() {
        delete [] buffer;
    }

    void push(uint8_t val) {
        while (true) {

            auto rear = REAR.load();

            if (rear == FRONT + size) {
                continue;
            }

            auto thread_val = buffer[rear & mask].load();

            if (thread_val != 0) {
                continue;
            }

            if (REAR.compare_exchange_weak(rear, rear + 1)) {
                if (buffer[rear & mask].compare_exchange_weak(thread_val, val)) {
                    return;
                }
            }
        }
    }

    bool pop(uint8_t& val) {

        while (true) {

            auto front = FRONT.load();

            if (front == REAR) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                front = FRONT.load();
                if (REAR == front) {
                    return false;
                }
            }

            auto thread_val = buffer[front & mask].load();

            if (thread_val == 0) {
                continue;
            }

            if (FRONT.compare_exchange_weak(front, front + 1)) {
                if (buffer[front & mask].compare_exchange_weak(thread_val, 0)) {
                    val = thread_val;
                    return true;
                }
            }
        }
    }
};

int main(int argc, const char * argv[]) {

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
            FixedQueue q(16);

            std::cout << "Amount of producers: " << producerNum << std::endl;
            std::cout << "Amount of consumers: " << consumerNum << std::endl;

            for (int i = 0; i < producerNum; i++) {
                producerThreads.emplace_back([i, &q, &taskNum] {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < taskNum; i++) {
                        q.push(1);
                    }
                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                    printf("Producer %d time is %f milliseconds\n", i + 1, (double)(duration.count()));
                });
            }

            for (int i = 0; i < consumerNum; i++) {
                consumerThreads.emplace_back([i, &q, &sums, &taskNum, &producerNum] {
                    auto start = std::chrono::high_resolution_clock::now();
                    std::atomic<int> local_sum = 0;
                    uint8_t q_e;
                    while (true) {
                        auto cond = q.pop(q_e);
                        if (cond) {
                            local_sum += q_e;
                        } else if (!cond && q.REAR == taskNum * producerNum) {
                            sums[i] = local_sum;
                            auto stop = std::chrono::high_resolution_clock::now();
                            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                            printf("Consumer %d time is %f milliseconds\n", i + 1, (double)(duration.count()));
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
