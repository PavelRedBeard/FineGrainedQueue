#include <iostream>
#include <mutex>

using namespace std;

struct Node
{
    int value;
    Node* next;
    mutex* node_mutex;
};

class FineGrainedQueue
{
    Node* head;
    mutex* queue_mutex;

public:
    void insertIntoMiddle(int value, int pos)
    {
        Node* prev, * cur;
        queue_mutex->lock();

        prev = this->head;
        cur = this->head->next;
        prev->node_mutex->lock();
        queue_mutex->unlock();
        if (cur)
        {
            cur->node_mutex->lock();
        }
        int count = 0;
        Node* newNode{};
        newNode->value = value;
        newNode->next = nullptr;

        while (cur)
        {
            if (count == pos)
            {
                prev->next = newNode;
                newNode->next = cur;
                prev->node_mutex->unlock();
                cur->node_mutex->unlock();
                return;
            }
            if (cur->next == nullptr)
            {
                cur->next = newNode;
                prev->node_mutex->unlock();
                cur->node_mutex->unlock();
                return;
            }
            Node* old_prev = prev;
            prev = cur;
            cur = cur->next;
            old_prev->node_mutex->unlock();
            if (cur)
            {
                cur->node_mutex->lock();
            }
            count++;
        }
        prev->node_mutex->unlock();
    }

};
