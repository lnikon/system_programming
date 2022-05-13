#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <iostream>

template <typename T>
class blocking_stack
{
    private:
        struct node
        {
            std::shared_ptr<T> data;
            node* next;

            node(const T& data_)
                : data(std::make_shared<T>(data_))
            {}
        };

        node* head;
        std::mutex mutex;
        std::condition_variable cv;

    public:
        void push(const T& data)
        {
            std::lock_guard<std::mutex> lg(mutex);
            node* const new_node = new node(data);
            new_node->next = head;
            head = new_node;
            cv.notify_one();
        }

        std::shared_ptr<T> pop()
        {
            std::unique_lock<std::mutex> ul(mutex);
            cv.wait(ul, [this]() { return head != nullptr; });
            node* old_head = head;
            head = old_head->next;
            return old_head ? old_head->data : std::shared_ptr<T>();
        }
};

void push(const std::size_t count, blocking_stack<int>& s)
{
        for (std::size_t i = 0; i < count; i++)
        {
            s.push(i);
        }
}

void pop(const std::size_t count, blocking_stack<int>& s)
{
    for (std::size_t i = 0; i < count; i++)
    {
        auto d = s.pop();
        if (d)
        {
            // std::cout << "d=" << *d << std::endl;
        }
    }
}

int main()
{
    blocking_stack<int> s;

    const std::size_t count = 1'000'000;

    std::thread th1(push, count, std::ref(s));
    std::thread th2(pop, count, std::ref(s));

    th1.join();
    th2.join();

    return 0;
}
