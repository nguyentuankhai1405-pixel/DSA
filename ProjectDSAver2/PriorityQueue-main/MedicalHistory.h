#ifndef MEDICAL_HISTORY_H
#define MEDICAL_HISTORY_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Một nút trên cây, lưu một thông tin (bệnh hoặc dị ứng)
struct TreeNode {
    string data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : data(val), left(nullptr), right(nullptr) {}
};

// Lớp quản lý Cây Lịch sử Bệnh án
class MedicalHistoryTree {
private:
    TreeNode* chronicRoot; // Gốc của cây con chứa bệnh mãn tính
    TreeNode* allergyRoot; // Gốc của cây con chứa dị ứng thuốc

    // Hàm đệ quy để thêm nút vào cây con (sắp xếp theo alphabet)
    TreeNode* insert(TreeNode* root, const string& value) {
        if (root == nullptr) {
            return new TreeNode(value);
        }
        if (value < root->data) {
            root->left = insert(root->left, value);
        } else if (value > root->data) {
            root->right = insert(root->right, value);
        }
        return root;
    }

    // Hàm đệ quy để tìm kiếm trong cây con
    bool search(TreeNode* root, const string& value) const {
        if (root == nullptr) {
            return false;
        }
        if (root->data == value) {
            return true;
        }
        return (value < root->data) ? search(root->left, value) : search(root->right, value);
    }

    // Hàm đệ quy để lấy tất cả dữ liệu từ cây con
    void inorderTraversal(TreeNode* root, vector<string>& result) const {
        if (root != nullptr) {
            inorderTraversal(root->left, result);
            result.push_back(root->data);
            inorderTraversal(root->right, result);
        }
    }

public:
    MedicalHistoryTree() : chronicRoot(nullptr), allergyRoot(nullptr) {}

    void addChronic(const string& illness) {
        chronicRoot = insert(chronicRoot, illness);
    }

    void addAllergy(const string& drug) {
        allergyRoot = insert(allergyRoot, drug);
    }

    bool isAllergicTo(const string& drug) const {
        return search(allergyRoot, drug);
    }

    vector<string> getChronicList() const {
        vector<string> list;
        inorderTraversal(chronicRoot, list);
        return list;
    }

    vector<string> getAllergyList() const {
        vector<string> list;
        inorderTraversal(allergyRoot, list);
        return list;
    }
};

#endif