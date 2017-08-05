#include <stdlib.h>
#include <stdio.h>
#include "AVLtree.h"


int Max(int a, int b)
{
	return (a > b) ? a : b;
}


//返回结点height
int Height(struct treeNode *t)
{
	if (t == NULL)
		return -1;
	else
		return t->height;
}



//左单旋转，令oldRoot的左孩子替代oldRoot的位置，oldRoot的左孩子改为oldRoot原左孩子的右孩子
AVLtree SingleRotateWithLeft(AVLtree oldRoot)
{
	AVLtree newRoot = oldRoot->left;
	oldRoot->left = newRoot->right;
	newRoot->right = oldRoot;
	oldRoot->height = Max(Height(oldRoot->left), Height(oldRoot->right)) + 1;
	newRoot->height = Max(Height(newRoot->left), oldRoot->height) + 1;

	return newRoot;
}



//右单旋转，令oldRoot的右孩子替代oldRoot的位置，oldRoot的右孩子改为oldRoot原右孩子的左孩子
AVLtree SingleRotateWithRight(AVLtree oldRoot)
{
	AVLtree newRoot = oldRoot->right;
	oldRoot->right = newRoot->left;
	newRoot->left = oldRoot;
	oldRoot->height = Max(Height(oldRoot->left), Height(oldRoot->right)) + 1;
	newRoot->height = Max(Height(newRoot->left), oldRoot->height) + 1;

	return newRoot;
}



//左双旋转，先令oldRoot的左孩子进行右单旋转，再令oldRoot进行左单旋转
AVLtree DoubleRotateWithLeft(AVLtree oldRoot)
{
	oldRoot->left = SingleRotateWithRight(oldRoot->left);
	return SingleRotateWithLeft(oldRoot);
}


//右双旋转，先令oldRoot的右孩子进行左单旋转，再令oldRoot进行右单旋转
AVLtree DoubleRotateWithRight(AVLtree oldRoot)
{
	oldRoot->right = SingleRotateWithLeft(oldRoot->right);
	return SingleRotateWithRight(oldRoot);
}



//向AVL树插入数据
AVLtree InsertToAVL(AVLtree t, int data)
{
	//若t为NULL，则创建新结点，于函数最后返回新结点
	if (t == NULL)
	{
		t = (AVLtree)malloc(sizeof(struct treeNode));
		t->frequency = 1;
		t->data = data;
		t->height = 0;
		t->left = t->right = NULL;
	}
	//若插入数据小于当前结点数据
	else if (data < t->data)
	{
		//将数据插入到t的左孩子，此时可能导致t的左孩子高度比右孩子高2，需要看情况选择旋转方式
		t->left = InsertToAVL(t->left, data);
		if (Height(t->left) - Height(t->right) == 2)
		{
			if (data < t->left->data)
				t = SingleRotateWithLeft(t);
			else
				t = DoubleRotateWithLeft(t);
		}
		else
			t->height = Max(Height(t->right), Height(t->left)) + 1; //即使左右子树高度差不为2，也有可能需要更新当前结点高度
	}
	//若插入数据大于当前结点数据
	else if (data > t->data)
	{
		//将数据插入到t的右子树，此时可能导致t的右孩子高度比左孩子高2，需要看情况选择旋转方式
		t->right = InsertToAVL(t->right, data);
		if (Height(t->right) - Height(t->left) == 2)
		{
			if (data > t->right->data)
				t = SingleRotateWithRight(t);
			else
				t = DoubleRotateWithRight(t);
		}
		else
			t->height = Max(Height(t->right), Height(t->left)) + 1; //即使左右子树高度差不为2，也有可能需要更新当前结点高度
	}
	//若数据已存在，则递增结点的frequency
	else
		t->frequency++;

	
	return t;
}



//插入数据到普通二叉查找树
BinaryTree InsertToBinaryTree(BinaryTree t, int data)
{
	if (t == NULL)
	{
		t = (BinaryTree)malloc(sizeof(struct treeNode));
		t->data = data;
		t->frequency = 1;
		t->height = 0;
		t->left = t->right = NULL;
	}
	else if (data < t->data)
	{
		t->left = InsertToBinaryTree(t->left, data);
		t->height = Height(t->left) + 1;
	}
	else if (data > t->data)
	{
		t->right = InsertToBinaryTree(t->right, data);
		t->height = Height(t->right) + 1;
	}
	else
		t->frequency++;

	return t;
}

//中序遍历输出树
void PrintInOrder(AVLtree t)
{
	if (t == NULL)
		return;

	PrintInOrder(t->left);
	printf("%d  ", t->data);
	PrintInOrder(t->right);
}

//先序遍历输出树
void PrintPreOrder(AVLtree t,int layer)
{
	if (t == NULL)
		return;
	
	for (int i = 0;i < layer;++i)
		printf("  ");
	printf("%d\n", t->data);
	PrintPreOrder(t->left, layer + 1);
	PrintPreOrder(t->right, layer + 1);
}



//用于删除子树中的最小结点，需保证所给t不为NULL
AVLtree DeleteMin(AVLtree t,int *pMinData)
{
	//若当前结点没有左孩子则必为最小结点，我们将其数据保存于pMinData，然后将其释放并返回其右孩子
	if (t->left == NULL)
	{
		(*pMinData) = t->data;
		AVLtree temp = t->right;
		free(t);
		return temp;
	}
	//若当前结点不是最小结点，则我们继续前往左子树寻找并删除最小结点
	//左子树删除了结点所以只可能出现左子树高度低于、等于右子树的情况
	//若左子树高度低于右子树2，此时我们根据右子树的状态决定对当前结点进行何种旋转
	else
	{
		t->left = DeleteMin(t->left,pMinData);
		if (Height(t->right) - Height(t->left) == 2)
		{
			if (Height(t->right->left) > Height(t->right->right))
			{
				t = DoubleRotateWithRight(t);
			}
			else
			{
				t = SingleRotateWithRight(t);
			}
		}
		//若删除操作后，左子树高度等于右子树（说明原来左子树高于右子树），我们依然要更新当前结点高度
		else
			t->height = Max(Height(t->left), Height(t->right)) + 1;
	}
	return t;
}



//删除AVL树中的结点
AVLtree DeleteNode(AVLtree t, int data)
{
	if (t == NULL)
		return t;
	//若给定数据小于当前结点，则前往左子树删除目标结点
	//若删除成功，则只可能出现左子树高度低于、等于右子树，对于低于右子树高度2的情况，我们根据右子树状态决定对当前结点的旋转
	//若删除操作后，左子树高度等于右子树（说明原来左子树高于右子树），我们依然要更新当前结点高度，这个操作在函数末尾进行
	if (data < t->data)
	{
		t->left = DeleteNode(t->left, data);
		if (Height(t->right) - Height(t->left) == 2)
		{
			if (Height(t->right->left) > Height(t->right->right))
			{
				t = DoubleRotateWithRight(t);
			}
			else
			{
				t = SingleRotateWithRight(t);
			}
		}
	}
	//若给定数据大于当前结点，则前往右子树删除目标结点
	//若删除成功，则只可能出现右子树高度低于、等于左子树，对于低于左子树高度2的情况，我们根据左子树状态决定对当前结点的旋转
	//若删除操作后，右子树高度等于左子树（说明原来右子树高于左子树），我们依然要更新当前结点高度，这个操作在函数末尾进行
	else if (data > t->data)
	{
		t->right = DeleteNode(t->right, data);
		if (Height(t->left) - Height(t->right) == 2)
		{
			if (Height(t->right->left) > Height(t->right->right))
			{
				t = SingleRotateWithLeft(t);
			}
			else
			{
				t = DoubleRotateWithLeft(t);
			}
		}
	}
	//若当前结点即需要删除的结点，且有两个孩子，则我们删除其右子树中的最小结点并将当前结点数据修改为该最小结点数据
	//删除后的操作同上
	else if (t->left && t->right && t->frequency == 1)
	{
		int minData;
		t->right = DeleteMin(t->right, &minData);
		t->data = minData;
		if (Height(t->left) - Height(t->right) == 2)
		{
			if (Height(t->right->left) > Height(t->right->right))
			{
				t = SingleRotateWithLeft(t);
			}
			else
			{
				t = DoubleRotateWithLeft(t);
			}
		}
	}
	//若当前结点需要删除且只有一个孩子或没有孩子，则返回其唯一孩子（也可能是NULL）并释放当前结点
	else if (t->frequency == 1)
	{
		AVLtree temp = NULL;
		temp = (t->left) ? t->left : t->right;
		free(t);
		return temp;
	}
	else
		t->frequency--;


	t->height = Max(Height(t->right), Height(t->left)) + 1;
	return t;
}