#include <string>
#include <vector>

using namespace std;

class MaxHeap { // based on pseudocode from class slides and https://www.geeksforgeeks.org/binary-heap/
private:
    vector<pair<string, int>> heap;
    int Parent(int child);
    void HeapifyUp(int index);
public:
    MaxHeap() {};
    ~MaxHeap() {};
    void Insert(pair<string, int> recipe);
    void PrintHeap(vector<pair<string, int>>& heap16);
};
// given a node index, returns the parent node index
int MaxHeap::Parent(int child) {
    // case for inputting node at index 0
    if (child <= 0) {
        return -1;
    }
    else {
        int parent = (child - 1) / 2;
        return parent;
    }
}
// performs heapifyup to maintain order of heap
void MaxHeap::HeapifyUp(int index) {
    // swaps given node with parent node if given node contains greater value than parent node
    if (index >= 0 && Parent(index) >= 0 && heap[Parent(index)].second < heap[index].second) {
        pair<string, int> tempPair = heap[index];
        heap[index] = heap[Parent(index)];
        heap[Parent(index)] = tempPair;
        HeapifyUp(Parent(index));
    }

}
// inserts node into heap
void MaxHeap::Insert(pair<string, int> recipe) {
    heap.push_back(recipe);
    HeapifyUp(heap.size() - 1);
}
// returns vector containing the top 16 nodes of the heap
void MaxHeap::PrintHeap(vector<pair<string, int>>& heap16) {
    for (int i = 0; i < 16; i++) {
        heap16.push_back(heap.at(i));
    }
}