#include <stdlib.h>
#include <stdio.h>
#include "AVLtree.h"


int Max(int a, int b)
{
	return (a > b) ? a : b;
}


//���ؽ��height
int Height(struct treeNode *t)
{
	if (t == NULL)
		return -1;
	else
		return t->height;
}



//����ת����oldRoot���������oldRoot��λ�ã�oldRoot�����Ӹ�ΪoldRootԭ���ӵ��Һ���
AVLtree SingleRotateWithLeft(AVLtree oldRoot)
{
	AVLtree newRoot = oldRoot->left;
	oldRoot->left = newRoot->right;
	newRoot->right = oldRoot;
	oldRoot->height = Max(Height(oldRoot->left), Height(oldRoot->right)) + 1;
	newRoot->height = Max(Height(newRoot->left), oldRoot->height) + 1;

	return newRoot;
}



//�ҵ���ת����oldRoot���Һ������oldRoot��λ�ã�oldRoot���Һ��Ӹ�ΪoldRootԭ�Һ��ӵ�����
AVLtree SingleRotateWithRight(AVLtree oldRoot)
{
	AVLtree newRoot = oldRoot->right;
	oldRoot->right = newRoot->left;
	newRoot->left = oldRoot;
	oldRoot->height = Max(Height(oldRoot->left), Height(oldRoot->right)) + 1;
	newRoot->height = Max(Height(newRoot->left), oldRoot->height) + 1;

	return newRoot;
}



//��˫��ת������oldRoot�����ӽ����ҵ���ת������oldRoot��������ת
AVLtree DoubleRotateWithLeft(AVLtree oldRoot)
{
	oldRoot->left = SingleRotateWithRight(oldRoot->left);
	return SingleRotateWithLeft(oldRoot);
}


//��˫��ת������oldRoot���Һ��ӽ�������ת������oldRoot�����ҵ���ת
AVLtree DoubleRotateWithRight(AVLtree oldRoot)
{
	oldRoot->right = SingleRotateWithLeft(oldRoot->right);
	return SingleRotateWithRight(oldRoot);
}



//��AVL����������
AVLtree InsertToAVL(AVLtree t, int data)
{
	//��tΪNULL���򴴽��½�㣬�ں�����󷵻��½��
	if (t == NULL)
	{
		t = (AVLtree)malloc(sizeof(struct treeNode));
		t->frequency = 1;
		t->data = data;
		t->height = 0;
		t->left = t->right = NULL;
	}
	//����������С�ڵ�ǰ�������
	else if (data < t->data)
	{
		//�����ݲ��뵽t�����ӣ���ʱ���ܵ���t�����Ӹ߶ȱ��Һ��Ӹ�2����Ҫ�����ѡ����ת��ʽ
		t->left = InsertToAVL(t->left, data);
		if (Height(t->left) - Height(t->right) == 2)
		{
			if (data < t->left->data)
				t = SingleRotateWithLeft(t);
			else
				t = DoubleRotateWithLeft(t);
		}
		else
			t->height = Max(Height(t->right), Height(t->left)) + 1; //��ʹ���������߶ȲΪ2��Ҳ�п�����Ҫ���µ�ǰ���߶�
	}
	//���������ݴ��ڵ�ǰ�������
	else if (data > t->data)
	{
		//�����ݲ��뵽t������������ʱ���ܵ���t���Һ��Ӹ߶ȱ����Ӹ�2����Ҫ�����ѡ����ת��ʽ
		t->right = InsertToAVL(t->right, data);
		if (Height(t->right) - Height(t->left) == 2)
		{
			if (data > t->right->data)
				t = SingleRotateWithRight(t);
			else
				t = DoubleRotateWithRight(t);
		}
		else
			t->height = Max(Height(t->right), Height(t->left)) + 1; //��ʹ���������߶ȲΪ2��Ҳ�п�����Ҫ���µ�ǰ���߶�
	}
	//�������Ѵ��ڣ����������frequency
	else
		t->frequency++;

	
	return t;
}



//�������ݵ���ͨ���������
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

//������������
void PrintInOrder(AVLtree t)
{
	if (t == NULL)
		return;

	PrintInOrder(t->left);
	printf("%d  ", t->data);
	PrintInOrder(t->right);
}

//������������
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



//����ɾ�������е���С��㣬�豣֤����t��ΪNULL
AVLtree DeleteMin(AVLtree t,int *pMinData)
{
	//����ǰ���û���������Ϊ��С��㣬���ǽ������ݱ�����pMinData��Ȼ�����ͷŲ��������Һ���
	if (t->left == NULL)
	{
		(*pMinData) = t->data;
		AVLtree temp = t->right;
		free(t);
		return temp;
	}
	//����ǰ��㲻����С��㣬�����Ǽ���ǰ��������Ѱ�Ҳ�ɾ����С���
	//������ɾ���˽������ֻ���ܳ����������߶ȵ��ڡ����������������
	//���������߶ȵ���������2����ʱ���Ǹ�����������״̬�����Ե�ǰ�����к�����ת
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
		//��ɾ���������������߶ȵ�����������˵��ԭ����������������������������ȻҪ���µ�ǰ���߶�
		else
			t->height = Max(Height(t->left), Height(t->right)) + 1;
	}
	return t;
}



//ɾ��AVL���еĽ��
AVLtree DeleteNode(AVLtree t, int data)
{
	if (t == NULL)
		return t;
	//����������С�ڵ�ǰ��㣬��ǰ��������ɾ��Ŀ����
	//��ɾ���ɹ�����ֻ���ܳ����������߶ȵ��ڡ����������������ڵ����������߶�2����������Ǹ���������״̬�����Ե�ǰ������ת
	//��ɾ���������������߶ȵ�����������˵��ԭ����������������������������ȻҪ���µ�ǰ���߶ȣ���������ں���ĩβ����
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
	//���������ݴ��ڵ�ǰ��㣬��ǰ��������ɾ��Ŀ����
	//��ɾ���ɹ�����ֻ���ܳ����������߶ȵ��ڡ����������������ڵ����������߶�2����������Ǹ���������״̬�����Ե�ǰ������ת
	//��ɾ���������������߶ȵ�����������˵��ԭ����������������������������ȻҪ���µ�ǰ���߶ȣ���������ں���ĩβ����
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
	//����ǰ��㼴��Ҫɾ���Ľ�㣬�����������ӣ�������ɾ�����������е���С��㲢����ǰ��������޸�Ϊ����С�������
	//ɾ����Ĳ���ͬ��
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
	//����ǰ�����Ҫɾ����ֻ��һ�����ӻ�û�к��ӣ��򷵻���Ψһ���ӣ�Ҳ������NULL�����ͷŵ�ǰ���
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