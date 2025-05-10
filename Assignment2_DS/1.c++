#include <iostream>
#include <string>
using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

struct Node {
    int id;
    Contact contact;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

Node* createNode(int id, Contact contact) {
    Node* node = new Node;
    node->id = id;
    node->contact = contact;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* insert(Node* node, int id, Contact contact, bool &duplicate) {
    if (node == NULL) return createNode(id, contact);

    if (id < node->id) {
        node->left = insert(node->left, id, contact, duplicate);
    } else if (id > node->id) {
        node->right = insert(node->right, id, contact, duplicate);
    } else {
        duplicate = true;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && id < node->left->id)
        return rightRotate(node);

    if (balance < -1 && id > node->right->id)
        return leftRotate(node);

    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* deleteNode(Node* root, int id) {
    if (root == NULL) return root;

    if (id < root->id) {
        root->left = deleteNode(root->left, id);
    } else if (id > root->id) {
        root->right = deleteNode(root->right, id);
    } else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->id = temp->id;
            root->contact = temp->contact;
            root->right = deleteNode(root->right, temp->id);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* search(Node* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (id < root->id)
        return search(root->left, id);
    return search(root->right, id);
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        cout << "ID: " << root->id << ", Name: " << root->contact.name
             << ", Phone: " << root->contact.phone
             << ", Email: " << root->contact.email << endl;
        inorder(root->right);
    }
}

void printTree(Node* root, int space = 0, int height = 10) {
    if (root == NULL) return;

    space += height;
    printTree(root->right, space);

    cout << endl;
    for (int i = height; i < space; i++) cout << " ";
    cout << root->id << endl;

    printTree(root->left, space);
}

int main() {
    Node* root = NULL;
    int choice;

    while (true) {
        cout << "\nAddress Book Application\n";
        cout << "------------------------\n";
        cout << "1. Add New Contact\n";
        cout << "2. Search for Contact\n";
        cout << "3. Delete Contact\n";
        cout << "4. List All Contacts (Sorted by ID)\n";
        cout << "5. Display Current Tree Structure\n";
        cout << "------------------------\n";
        cout << "Enter operation (1-5): ";
        cin >> choice;

        if (choice == 1) {
            int id;
            Contact c;
            cout << "Enter unique ID (integer): ";
            cin >> id;
            cin.ignore();

            cout << "Enter name: ";
            getline(cin, c.name);
            cout << "Enter phone: ";
            getline(cin, c.phone);
            cout << "Enter email: ";
            getline(cin, c.email);

            bool duplicate = false;
            root = insert(root, id, c, duplicate);
            if (duplicate) {
                cout << "Error: Contact with ID " << id << " already exists.\n";
            } else {
                cout << "Contact added successfully.\n";
            }

        } else if (choice == 2) {
            int id;
            cout << "Enter ID to search: ";
            cin >> id;
            Node* result = search(root, id);
            if (result == NULL) {
                cout << "Contact not found.\n";
            } else {
                cout << "Contact found:\n";
                cout << "ID: " << result->id << endl;
                cout << "Name: " << result->contact.name << endl;
                cout << "Phone: " << result->contact.phone << endl;
                cout << "Email: " << result->contact.email << endl;
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter ID to delete: ";
            cin >> id;
            Node* result = search(root, id);
            if (result == NULL) {
                cout << "Contact not found.\n";
            } else {
                root = deleteNode(root, id);
                cout << "Contact deleted successfully.\n";
            }

        } else if (choice == 4) {
            cout << "Contacts in Address Book (sorted by ID):\n";
            inorder(root);
        } else if (choice == 5) {
            cout << "Current AVL Tree:\n";
            printTree(root);
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
