#include <iostream>
using namespace std;

class Node
{
public:
    int id, height;
    string name, phone, email;
    Node *Left, *Right;

    Node(int id, string name, string phone, string email)
    {
        this->id = id;
        this->name = name;
        this->phone = phone;
        this->email = email;
        Left = Right = NULL;
        height = 1;
    }
};

class AVLTree
{
private:
    Node *root;

public:
    AVLTree()
    {
        root = NULL;
    }

    int height(Node *r)
    {
        if (r == NULL)
            return 0;
        return r->height;
    }

    int getBalance(Node *r)
    {
        if (r == NULL)
            return 0;
        return height(r->Left) - height(r->Right);
    }

    Node *rotateRight(Node *r)
    {
        Node *newRoot = r->Left;
        Node *temp = newRoot->Right;
        newRoot->Right = r;
        r->Left = temp;
        r->height = max(height(r->Left), height(r->Right)) + 1;
        newRoot->height = max(height(newRoot->Left), height(newRoot->Right)) + 1;
        return newRoot;
    }

    Node *rotateLeft(Node *r)
    {
        Node *newRoot = r->Right;
        Node *temp = newRoot->Left;
        newRoot->Left = r;
        r->Right = temp;
        r->height = max(height(r->Left), height(r->Right)) + 1;
        newRoot->height = max(height(newRoot->Left), height(newRoot->Right)) + 1;
        return newRoot;
    }

    Node *Insert(Node *r, int id, string name, string phone, string email)
    {
        if (r == NULL)
            return new Node(id, name, phone, email);

        if (id < r->id)
            r->Left = Insert(r->Left, id, name, phone, email);
        else if (id > r->id)
            r->Right = Insert(r->Right, id, name, phone, email);
        else
            return r;

        r->height = max(height(r->Left), height(r->Right)) + 1;

        int balance = getBalance(r);

        if (balance > 1 && id < r->Left->id)
            return rotateRight(r);

        if (balance < -1 && id > r->Right->id)
            return rotateLeft(r);

        if (balance > 1 && id > r->Left->id)
        {
            r->Left = rotateLeft(r->Left);
            return rotateRight(r);
        }

        if (balance < -1 && id < r->Right->id)
        {
            r->Right = rotateRight(r->Right);
            return rotateLeft(r);
        }

        return r;
    }

    void Insert(int id, string name, string phone, string email)
    {
        root = Insert(root, id, name, phone, email);
    }

    Node *Search(Node *r, int key)
    {
        if (r == NULL || r->id == key)
            return r;

        if (key < r->id)
            return Search(r->Left, key);

        return Search(r->Right, key);
    }

    bool Search(int key)
    {
        return Search(root, key) != NULL;
    }

    void Inorder(Node *r)
    {
        if (r == NULL)
            return;

        Inorder(r->Left);
        cout << r->id << " " << r->name << " " << r->phone << " " << r->email << endl;
        Inorder(r->Right);
    }

    void Inorder()
    {
        Inorder(root);
    }

    void DisplayTree(Node *r, string indent)
    {
        if (r != NULL)
        {
            cout << indent << r->id << endl;
            DisplayTree(r->Left, indent + "L---");
            DisplayTree(r->Right, indent + "R---");
        }
    }

    void DisplayTree()
    {
        DisplayTree(root, "");
    }
};

int main()
{
    AVLTree tree;
    int choice, id;
    string name, phone, email;

    while (true)
    {
        cout << "1. Add New Contact" << endl;
        cout << "2. Search for Contact" << endl;
        cout << "3. List All Contacts (Sorted by ID)" << endl;
        cout << "4. Display Current Tree Structure" << endl;
        cout << "Enter operation (1-4): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter unique ID: ";
            cin >> id;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter email: ";
            cin >> email;
            tree.Insert(id, name, phone, email);
            cout << "Contact added successfully." << endl;
            break;

        case 2:
            cout << "Enter ID to search: ";
            cin >> id;
            if (tree.Search(id))
                cout << "Contact found." << endl;
            else
                cout << "Contact not found." << endl;
            break;

        case 3:
            cout << "Contacts in Address Book (sorted by ID):" << endl;
            tree.Inorder();
            break;

        case 4:
            cout << "Current AVL Tree:" << endl;
            tree.DisplayTree();
            break;

        default:
            cout << "Invalid operation." << endl;
        }
    }

    return 0;
}
