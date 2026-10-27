class LinkedListNode {
public:
    int val;
    LinkedListNode* next;

    // LinkedListNode() {
    //     val = 0;
    //     next = nullptr;
    // }
    LinkedListNode() : val(0), next(nullptr) {};

    // LinkedListNode(int val) {
    //     this->val = val;
    //     next = nullptr;
    // }
    LinkedListNode(int val) : val(val), next(nullptr) {};

    // LinkedListNode(int val, LinkedListNode* next) {
    //     this->val = val;
    //     this->next = next;
    // }
    LinkedListNode(int val, LinkedListNode* next) : val(val), next(next) {};

    // ~LinkedListNode() {

    // }
    ~LinkedListNode() = default;

// State Queries
    
    // Is the linked list empty
    bool empty() {
        return (this == nullptr);
    }

    // Returns the length of linked list
    int size() {
        LinkedListNode* node = this;

        int length = 0;
        while(node != nullptr) {
            length++;
            node = node->next;
        }

        return length;
    }

    // Reverses a linked list
    // returns the head of the reversed list
    LinkedListNode* reverseLinkedList() {
        LinkedListNode* head = this;

        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        LinkedListNode* prevNode = nullptr;
        LinkedListNode* currNode = head;

        while(currNode != nullptr) {
            LinkedListNode* fwdNode = currNode->next;

            currNode->next = prevNode;
            prevNode = currNode;
            currNode = fwdNode;
        }

        return prevNode;
    }

    // Finds the middle node of a linked list
    // If length is odd, returns the only middle node
    // If the length is even, returns the second middle node
    LinkedListNode* getMiddleNode() {
        LinkedListNode* head = this;

        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        LinkedListNode* slowPtr = head;
        LinkedListNode* fastPtr = head->next;

        while(fastPtr != nullptr) {
            fastPtr = fastPtr->next;
            if (fastPtr != nullptr) {
                fastPtr = fastPtr->next;
            }

            slowPtr = slowPtr->next;
        }

        return slowPtr;
    }
};