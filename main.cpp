#include <stdio.h>
#include <stdlib.h>
#include "AVLtree.h"

int main()
{
	AVLtree avlTree = NULL;
	BinaryTree binaryTree = NULL;

	int size = 10;
	for (int i = 0;i < size;++i)
	{
		avlTree = InsertToAVL(avlTree, i);
		binaryTree = InsertToBinaryTree(binaryTree, i);
	}

	printf("二叉树高度为%d   相同数据的AVL树高度为%d\n", Height(binaryTree), Height(avlTree));

	PrintInOrder(avlTree);
	putchar('\n');
	PrintPreOrder(avlTree, 0);
	

	for (int i = 0;i < size;++i)
	{
		printf("下面是删除%d\n",i);
		avlTree=DeleteNode(avlTree, i);
		PrintInOrder(avlTree);
		putchar('\n');
		PrintPreOrder(avlTree, 0);
	}

	return 0;
}