using namespace std;

class MaxHeap {
private:
    vector<pair<string, int>> heap;
    int Parent(int child);
    void HeapifyUp(int index);
public:
    MaxHeap() {};
    ~MaxHeap() {};
    void Insert(pair<string, int> recipe);
    void PrintHeap(vector<pair<string, int>>& heap10);
};

int MaxHeap::Parent(int child) {
    if (child <= 0) {
        return -1;
    }
    else {
        int parent = (child - 1) / 2;
        return parent;
    }
}

void MaxHeap::HeapifyUp(int index) {
    if (index >= 0 && Parent(index) >= 0 && heap[Parent(index)].second < heap[index].second) {
        pair<string, int> tempPair = heap[index];
        heap[index] = heap[Parent(index)];
        heap[Parent(index)] = tempPair;
        HeapifyUp(Parent(index));
    }

}

void MaxHeap::Insert(pair<string, int> recipe) {
    heap.push_back(recipe);
    HeapifyUp(heap.size() - 1);

}

void MaxHeap::PrintHeap(vector<pair<string, int>>& heap10) {
    for (int i = 0; i < 10; i++) {
        heap10.push_back(heap.at(i));
    }
}