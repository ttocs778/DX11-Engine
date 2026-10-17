#pragma once

template<typename T, int size> class PoolAllocator
{
    union Node
    {
        T data;
        Node* pNext;
    };
    Node array[size];
    Node* firstFree;

public:
    //Initialize the pool
    PoolAllocator()
    {
        firstFree = &array[0];  // Set first free to the start of the array

        // Link all nodes to form a free list
        for (int i = 0; i < size - 1; i++)
        {
            array[i].pNext = &array[i + 1];
        }
        array[size - 1].pNext = nullptr;  // Last node points to null
    }

    //Allocate an element from the Pool and return a pointer to it.
    //return nullptr if the there are no elements available
    T* Allocate()
    {
        if (firstFree == nullptr)
            return nullptr;  // No available memory

        Node* allocatedNode = firstFree;   // Get the first free node
        firstFree = firstFree->pNext;      // Move firstFree to next free node

        return &allocatedNode->data;       // Return pointer to data
    }

    //Release element "t" to the Pool.
    void Free(T* t)
    {
        if (!t)
            return; // Do nothing if nullptr is passed

        Node* node = reinterpret_cast<Node*>(t);  // Convert back to Node
        node->pNext = firstFree;                  // Link it to the front of the free list
        firstFree = node;                         // Update firstFree pointer
    }
};
