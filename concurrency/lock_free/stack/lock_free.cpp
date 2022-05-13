#include <atomic>
#include <thread>
#include <memory>
#include <iostream>

template <typename T>
class lock_free_stack
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

        std::atomic<node*> head;
        std::atomic<node*> to_be_deleted;
        std::atomic<unsigned> threads_in_pop;

        void delete_nodes(node* nodes)
        {
            while (nodes)
            {
                node* next = nodes->next;
                delete nodes;
                nodes = next;
            }
        }

        void chain_pending_nodes(node* nodes)
        {
            node* last = nodes;
            while (node * const next = last->next)
            {
                last = next;
            }
            chain_pending_nodes(nodes, last);
        }

        void chain_pending_nodes(node* first, node* last)
        {
            last->next = to_be_deleted;
            while (!to_be_deleted.compare_exchange_weak(last->next, first));
        }

        void chain_pending_node(node* node)
        {
            chain_pending_nodes(node, node);
        }

        void try_reclaim(node* old_head)
        {
            if (threads_in_pop == 1)
            {
                node* nodes_to_delete = to_be_deleted.exchange(nullptr);
                if (!--threads_in_pop)
                {
                    delete_nodes(nodes_to_delete);
                }
                else if(nodes_to_delete)
                {
                    chain_pending_nodes(nodes_to_delete);
                }
                delete old_head;
            }
            else
            {
                chain_pending_node(old_head);
                --threads_in_pop;
            }
        }

    public:
        void print()
        {
            node* curr = head.load();
            while (curr && curr->next)
            {
                std::cout << curr->data << ' ';
                curr = curr->next;
            }
            std::cout << std::endl;
        }

        void push(const T& data)
        {
            node* const new_node = new node(data);
            new_node->next = head.load();
            while (!head.compare_exchange_weak(new_node->next, new_node));
        }

        std::shared_ptr<T> pop()
        {
            ++threads_in_pop;
            node* old_head = head.load();
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next));
            std::shared_ptr<T> res;
            if (old_head)
            {
                res.swap(old_head->data);
            }
            try_reclaim(old_head);
            return res;
        }
};

void push(const std::size_t count, lock_free_stack<int>& s)
{
        for (std::size_t i = 0; i < count; i++)
        {
            s.push(i);
        }
}

void pop(const std::size_t count, lock_free_stack<int>& s)
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
    lock_free_stack<int> s;

    const std::size_t count = 1'000'000;

    std::thread th1(push, count, std::ref(s));
    std::thread th2(pop, count, std::ref(s));

    th1.join();
    th2.join();

    return 0;
}
