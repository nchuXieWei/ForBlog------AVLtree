#ifndef XWAVLTREE
#define XWAVLTREE

struct treeNode {
	int data;
	int height;
	int frequency;
	struct treeNode *left;
	struct treeNode *right;
};
typedef struct treeNode *AVLtree;
typedef struct treeNode *BinaryTree;

void PrintPreOrder(AVLtree t,int layer);
AVLtree DeleteNode(AVLtree t, int data);
AVLtree InsertToAVL(AVLtree t, int data);
BinaryTree InsertToBinaryTree(BinaryTree t, int data);
int Height(struct treeNode *t);
void PrintInOrder(AVLtree t);

#endif