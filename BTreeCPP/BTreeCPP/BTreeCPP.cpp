/*
#include <iostream>
#include <conio.h>

using namespace std;

#define MAX 4
#define MIN 2
struct ratN
{
    int a, b;
};

struct btreeNode {
    int val[MAX + 1], count;
    btreeNode* link[MAX + 1];
};

btreeNode* root;

// creating new node
btreeNode* createNode(int val, btreeNode* child) {
    btreeNode* newNode = new btreeNode;
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Places the value in appropriate position
void addValToNode(int val, int pos, btreeNode* node, btreeNode* child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// split the node
void splitNode(int val, int* pval, int pos, btreeNode* node, btreeNode* child, btreeNode** newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = new btreeNode;
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
        addValToNode(val, pos, node, child);
    }
    else {
        addValToNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// sets the value val in the node
int setValueInNode(int val, int* pval, btreeNode* node, btreeNode** child) {

    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1]) {
        pos = 0;
    }
    else {
        for (pos = node->count;
            (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            cout << "Duplicates not allowed\n";
            return 0;
        }
    }
    if (setValueInNode(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            addValToNode(*pval, pos, node, *child);
        }
        else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// insert val in B-Tree
void insertion(int val) {
    int flag, i;
    btreeNode* child;

    flag = setValueInNode(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

// copy successor for the value to be deleted
void copySuccessor(btreeNode* myNode, int pos) {
    btreeNode* dummy;
    dummy = myNode->link[pos];

    for (; dummy->link[0] != NULL;)
        dummy = dummy->link[0];
    myNode->val[pos] = dummy->val[1];

}

// removes the value from the given node and rearrange values
void removeVal(btreeNode* myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->count) {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->count--;
}

// shifts value from parent to right child
void doRightShift(btreeNode* myNode, int pos) {
    btreeNode* x = myNode->link[pos];
    int j = x->count;

    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->count++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->count];
    myNode->link[pos] = x->link[x->count];
    x->count--;
    return;
}

// shifts value from parent to left child
void doLeftShift(btreeNode* myNode, int pos) {
    int j = 1;
    btreeNode* x = myNode->link[pos - 1];

    x->count++;
    x->val[x->count] = myNode->val[pos];
    x->link[x->count] = myNode->link[pos]->link[0];

    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->count--;

    while (j <= x->count) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}

// merge nodes
void mergeNodes(btreeNode* myNode, int pos) {
    int j = 1;
    btreeNode* x1 = myNode->link[pos], * x2 = myNode->link[pos - 1];

    x2->count++;
    x2->val[x2->count] = myNode->val[pos];
    x2->link[x2->count] = myNode->link[0];

    while (j <= x1->count) {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
    }

    j = pos;
    while (j < myNode->count) {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->count--;
    free(x1);
}

// adjusts the given node
void adjustNode(btreeNode* myNode, int pos) {
    if (!pos) {
        if (myNode->link[1]->count > MIN) {
            doLeftShift(myNode, 1);
        }
        else {
            mergeNodes(myNode, 1);
        }
    }
    else {
        if (myNode->count != pos) {
            if (myNode->link[pos - 1]->count > MIN) {
                doRightShift(myNode, pos);
            }
            else {
                if (myNode->link[pos + 1]->count > MIN) {
                    doLeftShift(myNode, pos + 1);
                }
                else {
                    mergeNodes(myNode, pos);
                }
            }
        }
        else {
            if (myNode->link[pos - 1]->count > MIN)
                doRightShift(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}

// delete val from the node
int delValFromNode(int val, btreeNode* myNode) {
    int pos, flag = 0;
    if (myNode) {
        if (val < myNode->val[1]) {
            pos = 0;
            flag = 0;
        }
        else {
            for (pos = myNode->count;
                (val < myNode->val[pos] && pos > 1); pos--);
            if (val == myNode->val[pos]) {
                flag = 1;
            }
            else {
                flag = 0;
            }
        }
        if (flag) {
            if (myNode->link[pos - 1]) {
                copySuccessor(myNode, pos);
                flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
                if (flag == 0) {
                    cout << "Given data is not present in B-Tree\n";
                }
            }
            else {
                removeVal(myNode, pos);
            }
        }
        else {
            flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos]) {
            if (myNode->link[pos]->count < MIN)
                adjustNode(myNode, pos);
        }
    }
    return flag;
}

// delete val from B-tree
void deletion(int val, btreeNode* myNode) {
    btreeNode* tmp;
    if (!delValFromNode(val, myNode)) {
        cout << "Given value is not present in B-Tree\n";
        return;
    }
    else {
        if (myNode->count == 0) {
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
    return;
}

// search val in B-Tree
void searching(int val, int* pos, btreeNode* myNode) {
    if (!myNode) {
        return;
    }

    if (val < myNode->val[1]) {
        *pos = 0;
    }
    else {
        for (*pos = myNode->count;
            (val < myNode->val[*pos] && *pos > 1); (*pos)--);
        if (val == myNode->val[*pos]) {
            cout << "Given data is Found\n";
            return;
        }
    }
    searching(val, pos, myNode->link[*pos]);
    return;
}

// B-Tree Traversal
void traversal(btreeNode* myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            traversal(myNode->link[i]);
            cout << myNode->val[i + 1] << ' ';
        }
        traversal(myNode->link[i]);
    }
}

int main() {
    int val, opt;
    while (true) {
        cout << "1. Insertion\t2. Deletion\n";
        cout << "3. Searching\t4. Traversal\n";
        cout << "5. Exit\nEnter your choice: ";
        cin >> opt;
        cout << endl;
        switch (opt) {
        case 1:
            cout << "Enter your input:";
            cin >> val;
            insertion(val);
            break;
        case 2:
            cout << "Enter the element to delete:";
            cin >> val;
            deletion(val, root);
            break;
        case 3:
            cout << "Enter the element to search:";
            cin >> val;
            searching(val, &opt, root);
            break;
        case 4:
            traversal(root);
            break;
        case 5:
            exit(0);
        }
        cout << endl;
    }

    system("pause");
}

*/

#include <iostream>
#include <cstdlib> // for exit()

using namespace std;

#define MAX 4
#define MIN 2

struct RationalNumber {
    int numerator;
    int denominator;
};

struct BtreeNode {
    RationalNumber val[MAX + 1];
    int count;
    BtreeNode* link[MAX + 1];
};

BtreeNode* root = nullptr;

// Function to find the greatest common divisor
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to simplify the rational number
void simplifyRational(RationalNumber& num) {
    int common = gcd(num.numerator, num.denominator);
    num.numerator /= common;
    num.denominator /= common;
}

// Creating a new node
BtreeNode* createNode(RationalNumber val, BtreeNode* child) {
    BtreeNode* newNode = new BtreeNode;
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Places the value in the appropriate position
void addValToNode(RationalNumber val, int pos, BtreeNode* node, BtreeNode* child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// Split the node
void splitNode(RationalNumber val, RationalNumber* pval, int pos, BtreeNode* node, BtreeNode* child, BtreeNode** newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = new BtreeNode;
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
        addValToNode(val, pos, node, child);
    }
    else {
        addValToNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Sets the value val in the node
int setValueInNode(RationalNumber val, RationalNumber* pval, BtreeNode* node, BtreeNode** child) {

    int pos;
    if (!node) {
        *pval = val;
        *child = nullptr;
        return 1;
    }

    if (val.numerator < node->val[1].numerator) {
        pos = 0;
    }
    else {
        for (pos = node->count;
            (val.numerator < node->val[pos].numerator && pos > 1); pos--);
        if (val.numerator == node->val[pos].numerator && val.denominator == node->val[pos].denominator) {
            cout << "Duplicates not allowed\n";
            return 0;
        }
    }
    if (setValueInNode(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            addValToNode(*pval, pos, node, *child);
        }
        else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Insert val into B-Tree
void insertion(RationalNumber val) {
    int flag;
    RationalNumber i;
    BtreeNode* child = nullptr;

    simplifyRational(val); // Simplify the rational number before insertion

    flag = setValueInNode(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

// Copy successor for the value to be deleted
void copySuccessor(BtreeNode* myNode, int pos) {
    BtreeNode* dummy;
    dummy = myNode->link[pos];

    for (; dummy->link[0] != nullptr;)
        dummy = dummy->link[0];
    myNode->val[pos] = dummy->val[1];
}

// Remove the value from the given node and rearrange values
void removeVal(BtreeNode* myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->count) {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->count--;
}

// Shift value from parent to right child
void doRightShift(BtreeNode* myNode, int pos) {
    BtreeNode* x = myNode->link[pos];
    int j = x->count;

    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->count++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->count];
    myNode->link[pos] = x->link[x->count];
    x->count--;
    return;
}

// Shift value from parent to left child
void doLeftShift(BtreeNode* myNode, int pos) {
    int j = 1;
    BtreeNode* x = myNode->link[pos - 1];

    x->count++;
    x->val[x->count] = myNode->val[pos];
    x->link[x->count] = myNode->link[pos]->link[0];

    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->count--;

    while (j <= x->count) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}

// Merge nodes
void mergeNodes(BtreeNode* myNode, int pos) {
    int j = 1;
    BtreeNode* x1 = myNode->link[pos], * x2 = myNode->link[pos - 1];

    x2->count++;
    x2->val[x2->count] = myNode->val[pos];
    x2->link[x2->count] = myNode->link[0];

    while (j <= x1->count) {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
    }

    j = pos;
    while (j < myNode->count) {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->count--;
    delete x1;
}

// Adjusts the given node
void adjustNode(BtreeNode* myNode, int pos) {
    if (!pos) {
        if (myNode->link[1]->count > MIN) {
            doLeftShift(myNode, 1);
        }
        else {
            mergeNodes(myNode, 1);
        }
    }
    else {
        if (myNode->count != pos) {
            if (myNode->link[pos - 1]->count > MIN) {
                doRightShift(myNode, pos);
            }
            else {
                if (myNode->link[pos + 1]->count > MIN) {
                    doLeftShift(myNode, pos + 1);
                }
                else {
                    mergeNodes(myNode, pos);
                }
            }
        }
        else {
            if (myNode->link[pos - 1]->count > MIN)
                doRightShift(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}

// Delete val from the node
int delValFromNode(RationalNumber val, BtreeNode* myNode) {
    int pos, flag = 0;
    if (myNode) {
        if (val.numerator < myNode->val[1].numerator) {
            pos = 0;
            flag = 0;
        }
        else {
            for (pos = myNode->count;
                (val.numerator < myNode->val[pos].numerator && pos > 1); pos--);
            if (val.numerator == myNode->val[pos].numerator && val.denominator == myNode->val[pos].denominator) {
                flag = 1;
            }
            else {
                flag = 0;
            }
        }
        if (flag) {
            if (myNode->link[pos - 1]) {
                copySuccessor(myNode, pos);
                flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
                if (flag == 0) {
                    cout << "Given data is not present in B-Tree\n";
                }
            }
            else {
                removeVal(myNode, pos);
            }
        }
        else {
            flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos]) {
            if (myNode->link[pos]->count < MIN)
                adjustNode(myNode, pos);
        }
    }
    return flag;
}

// Delete val from B-tree
void deletion(RationalNumber val, BtreeNode* myNode) {
    BtreeNode* tmp;
    if (!delValFromNode(val, myNode)) {
        cout << "Given value is not present in B-Tree\n";
        return;
    }
    else {
        if (myNode->count == 0) {
            tmp = myNode;
            myNode = myNode->link[0];
            delete tmp;
        }
    }
    root = myNode;
    return;
}

// Search val in B-Tree
void searching(RationalNumber val, int* pos, BtreeNode* myNode) {
    if (!myNode) {
        return;
    }

    if (val.numerator < myNode->val[1].numerator) {
        *pos = 0;
    }
    else {
        for (*pos = myNode->count;
            (val.numerator < myNode->val[*pos].numerator && *pos > 1); (*pos)--);
        if (val.numerator == myNode->val[*pos].numerator && val.denominator == myNode->val[*pos].denominator) {
            cout << "Given data is Found\n";
            return;
        }
    }
    searching(val, pos, myNode->link[*pos]);
    return;
}

// B-Tree Traversal
void traversal(BtreeNode* myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            traversal(myNode->link[i]);
            cout << myNode->val[i + 1].numerator << "/" << myNode->val[i + 1].denominator << ' ';
        }
        traversal(myNode->link[i]);
    }
}

int main() {
    RationalNumber val;
    int opt;
    while (true) {
        cout << "1. Insertion\t2. Deletion\n";
        cout << "3. Searching\t4. Traversal\n";
        cout << "5. Exit\nEnter your choice: ";
        cin >> opt;
        cout << endl;
        switch (opt) {
        case 1:
            cout << "Enter your input (numerator denominator): ";
            cin >> val.numerator >> val.denominator;
            simplifyRational(val); // Simplify the rational number before insertion
            insertion(val);
            break;
        case 2:
            cout << "Enter the element to delete (numerator denominator): ";
            cin >> val.numerator >> val.denominator;
            simplifyRational(val); // Simplify the rational number before deletion
            deletion(val, root);
            break;
        case 3:
            cout << "Enter the element to search (numerator denominator): ";
            cin >> val.numerator >> val.denominator;
            simplifyRational(val); // Simplify the rational number before searching
            searching(val, &opt, root);
            break;
        case 4:
            traversal(root);
            break;
        case 5:
            exit(0);
        }
        cout << endl;
    }
}
