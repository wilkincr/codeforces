#include <iostream>
#include <utility>
#include <math.h>
#include <deque>
#include <vector>
#include <climits>
using namespace std;

// Constants
const int INF = 1e9 + 5;  // A large value used as initial minimum

// Trie node structure for storing binary representations of numbers
struct TrieNode{
    TrieNode* children[2]{nullptr, nullptr};
    int maxStoredIndex = -1;
};

void trieInsert(TrieNode* root, int insertNumber, int numberIndex) {
    // start insert from root
    TrieNode* current = root;
    for (int bit = 30; bit >= 0; --bit) {
        int currentBit = (insertNumber >> bit) & 1;
        if (!current->children[currentBit]) {
            current->children[currentBit] = new TrieNode();
        }
        current = current->children[currentBit];
        current->maxStoredIndex = max(current->maxStoredIndex, numberIndex);
    }
    // insert each bit of number
}

int trieQuery(TrieNode* node, int queryNumber, int k, int currentBit, bool alreadyGreater) {
    if (!node) {
        return -1;
    }
    if (alreadyGreater) {
        return node->maxStoredIndex;
    }
    if (currentBit < 0) {
        return node->maxStoredIndex;
    }

    // Current bit of k
    int kBit = (k >> currentBit) & 1;
    int queryBit = (queryNumber >> currentBit) & 1;
    // If k is 0, our target can have same or different
    if (kBit == 0) {
        int chooseSame = trieQuery(node->children[queryBit], queryNumber, k, currentBit - 1, false);
        int chooseDifferent = trieQuery(node->children[queryBit ^ 1], queryNumber, k, currentBit - 1, true);
        return max(chooseSame, chooseDifferent);
    }
    // k is 1, target needs to have different bit in order to be able to meet threshold
    return trieQuery(node->children[queryBit ^ 1], queryNumber, k, currentBit - 1, false);
}

void deleteTrie(TrieNode* node) {
    if (!node) return;
    deleteTrie(node->children[0]);
    deleteTrie(node->children[1]);
    delete node;
}


// Wrapper for trie query
int findBestMatchingIndex(TrieNode* root, int currentValue, int thresholdK) {
    return trieQuery(root, currentValue, thresholdK, 30, false);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCases;
    cin >> testCases;

    while (testCases--) {
        int arrayLength, thresholdK;
        cin >> arrayLength >> thresholdK;

        vector<int> a(arrayLength);
        for (int& value : a) cin >> value;

        if (thresholdK == 0) {
            // Any single element satisfies the condition since a[i] ^ a[i] == 0 ≥ 0
            cout << 1 << '\n';
            continue;
        }

        TrieNode* root = new TrieNode();  // New trie for this test case
        int minLength = INF;

        for (int i = 0; i < arrayLength; ++i) {
            int bestMatchingIndex = findBestMatchingIndex(root, a[i], thresholdK);
            if (bestMatchingIndex != -1) {
                minLength = min(minLength, i - bestMatchingIndex + 1);
            }
            trieInsert(root, a[i], i);  // Add current element to trie
        }

        cout << (minLength == INF ? -1 : minLength) << '\n';
        deleteTrie(root);
    }

    return 0;
}