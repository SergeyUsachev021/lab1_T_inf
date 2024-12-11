#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>


// Структура узла дерева Хаффмана
struct Node {
    char symbol; // Символ
    double probability; // Вероятность
    Node* left; // Указатель на левое поддерево
    Node* right; // Указатель на правое поддерево

    Node(char symbol, double probability) : symbol(symbol), probability(probability), left(nullptr), right(nullptr) {}
};

// Функция для сравнения узлов (по вероятности)
bool compare(Node* a, Node* b) {
    return a->probability > b->probability; // Возвращаем true, если a имеет большую вероятность
}

// Функция для построения дерева Хаффмана
Node* buildHuffmanTree(const std::vector<std::pair<char, double>>& symbolProbabilities) {
    std::vector<Node*> minHeap;

    // Создаем вектор узлов из вероятностей символов
    for (const auto& pair : symbolProbabilities) {
        minHeap.push_back(new Node(pair.first, pair.second));
    }

    // Построение дерева Хаффмана
    while (minHeap.size() > 1) {
        // Сортируем вектор по вероятности
        sort(minHeap.begin(), minHeap.end(), compare);

        // Извлекаем два узла с наименьшей вероятностью
        Node* left = minHeap.back(); minHeap.pop_back();
        Node* right = minHeap.back(); minHeap.pop_back();

        // Создаем новый узел с суммарной вероятностью
        Node* newNode = new Node('\0', left->probability + right->probability);
        newNode->left = left;
        newNode->right = right;

        minHeap.push_back(newNode); // Добавляем новый узел обратно в вектор
    }

    return minHeap.front(); // Возвращаем корень дерева
}

// Рекурсивная функция для генерации кодов
void generateCodes(Node* root, const std::string& str, std::vector<std::string>& codes) {
    if (!root) return;

    if (root->left == nullptr && root->right == nullptr) {
        codes[static_cast<unsigned char>(root->symbol)] = str; // Сохраняем код для символа
    }

    generateCodes(root->left, str + "0", codes);  // Левый потомок
    generateCodes(root->right, str + "1", codes); // Правый потомок
}

// Функция для освобождения памяти дерева w
void deleteTree(Node* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

// Функция определения энтропии
double entropy(const std::vector<std::pair<char, double>>& symbolProbabilities){
    double entropy = 0;

    for (const auto& pair : symbolProbabilities){
        entropy += pair.second * log2f(pair.second);
    }
    entropy *= -1;

    return entropy;
}

// функция для определения средней длины комбинации
double averageLengthCombination(const std::vector<std::pair<char, double>>& symbolProbabilities,
                                const std::vector<std::string>& codes){
    double averageLengthCombination = 0;

    for (const auto& pair : symbolProbabilities){
        averageLengthCombination += pair.second * codes[static_cast<unsigned char>(pair.first)].length();
    }
    return averageLengthCombination;
}
// Основная функция
int main() {
    std::vector<std::pair<char, double>> symbolProbabilities = {
        {'A', 0.28}, {'B', 0.22}, {'C', 0.15}, {'D', 0.11},
        {'E', 0.11}, {'F', 0.07}, {'G', 0.04}, {'H', 0.02}
    };

    Node* root = buildHuffmanTree(symbolProbabilities); // Построение дерева

    std::vector<std::string> huffmanCodes(256); // Вектор для хранения кодов Хаффмана
    generateCodes(root, "", huffmanCodes); // Генерация кодов

    std::cout << "Huffman code:\n";
    for (const auto& pair : symbolProbabilities) {
        std::cout << pair.first << ": " << huffmanCodes[static_cast<unsigned char>(pair.first)] << "\n";
    }

    double entropyNum = entropy(symbolProbabilities);
    std::cout << "энтропия = " << entropyNum << "\n";

    double averageLengthCodeCombination = averageLengthCombination(symbolProbabilities, huffmanCodes);
    std::cout << "кодовая комбинация средней длины = " << averageLengthCodeCombination << "\n";

    double redundancy = averageLengthCodeCombination - entropyNum;
    std::cout << "избыточность = " << redundancy;

    deleteTree(root); // Освобождаем память

    return 0;
}