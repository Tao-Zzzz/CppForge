#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <thread>

// 一个辅助函数对象，封装了 std::accumulate 的调用逻辑
template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result); // 计算 [first, last) 的和，存入 result
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last); // 元素个数

    if (!length)
        return init; // ① 如果范围为空，直接返回初始值

    unsigned long const min_per_thread = 25; // 每个线程至少处理的最小数据量

    // ② 估算最多可开多少线程（数据总数除以单线程最小任务数，向上取整）
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    // ③ 查询硬件支持的并发线程数（0 表示不确定，设为2兜底），取其与 max_threads 的较小者
    unsigned long const hardware_threads = std::thread::hardware_concurrency();

    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    // ④ 每个线程实际处理多少个元素
    unsigned long const block_size = length / num_threads;

    // 保存每个线程计算结果
    std::vector<T> results(num_threads);

    // 用于保存除主线程外的其他线程对象
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;

    // ⑤ 创建 num_threads - 1 个线程
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // ⑥ 计算当前线程处理的末尾位置

        // ⑦ 创建新线程计算 block_start 到 block_end 的和
        threads[i] = std::thread(
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i]) // 使用 std::ref 保证引用语义
        );

        block_start = block_end; // ⑧ 更新下一块的起始位置
    }

    // ⑨ 主线程计算最后一块（线程数不够时，主线程也要干活）
    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);

    // ⑩ 等待所有子线程完成
    for (auto &entry : threads)
        entry.join();

    // ⑪ 最终汇总所有线程的结果，加上初始值
    return std::accumulate(results.begin(), results.end(), init);
}

void use_parallel_acc()
{
    std::vector<int> vec;
    vec.reserve(10000);
    for (int i = 0; i < 10000; ++i)
    {
        vec.push_back(i); // 填入 0~9999
    }

    int sum = parallel_accumulate(vec.begin(), vec.end(), 0);
    std::cout << "sum is " << sum << std::endl;
}

int main(){
    use_parallel_acc();
    return 0;
}