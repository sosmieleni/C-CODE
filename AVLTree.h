#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;
typedef struct _AvlNode
{
	ElementType Element;
	struct _AvlNode *left;
	struct _AvlNode *right;
	int	Height;
}AvlNode;

typedef AvlNode *Position;
typedef AvlNode *AvlTree;

AvlTree MakeEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
AvlTree Insert(ElementType X, AvlTree *T);
AvlTree Delete(ElementType X, AvlTree *T);
static int Height(Position P);
Position SingleRotateWithLeft(Position K2);
Position SingleRotateWithRight(Position K2);
Position DoubleRotateWithLeft(Position K3);
Position DoubleRotateWithRight(Position K3);
void PreTraver(AvlTree T);

AvlTree MakeEmpty(AvlTree T) 
{
	if (T != NULL) {
		MakeEmpty(T->left);
		MakeEmpty(T->right);
		free(T);
	}
	else
		return NULL;
}

Position Find(ElementType X, AvlTree T)
{
	if (T == NULL)
		return NULL;
	if (X < T->Element)
		return Find(X, T->left);
	else if (X > T->Element)
		return Find(X, T->right);
	else
		return T;
}

Position FindMin(AvlTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->left == NULL)
		return T;
	else
		return FindMin(T->left);
}

Position FindMax(AvlTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->right == NULL)
		return T;
	else
		return FindMax(T->right);
}

static int Height(Position P)
{
	if (P == NULL)
		return -1;
	else
		return P->Height;
}

int Max(int a, int b)
{
	int max = a > b ? a : b;
	return max;
}

AvlTree Insert(ElementType X, AvlTree *T)
{
	if (*T == NULL)
	{
		*T = (AvlTree)malloc(sizeof(AvlNode));
		(*T)->Element = X;
		(*T)->left = (*T)->right = NULL;
	}
	else if (X < (*T)->Element)
	{
		(*T)->left = Insert(X, &(*T)->left);
		if (Height((*T)->left) - Height((*T)->right) == 2)
			if (X < (*T)->left->Element)
				*T = SingleRotateWithLeft(*T);
			else
				*T = DoubleRotateWithLeft(*T);
	}
	else if (X > (*T)->Element)
	{
		(*T)->right = Insert(X, &(*T)->right);
		if (Height((*T)->right) - Height((*T)->left) == 2)
			if (X > (*T)->right->Element)
				*T = SingleRotateWithRight(*T);
			else
				*T = DoubleRotateWithRight(*T);
	}
	(*T)->Height = Max(Height((*T)->left), Height((*T)->right)) + 1;
	return *T;
}

AvlTree Delete(ElementType X, AvlTree *T)
{
	AvlTree Tempcell;
	if (T == NULL)
		return NULL;
	else if (X < (*T)->Element)
	{
		(*T)->left = Delete(X, &(*T)->left);
		if (Height((*T)->right) - Height((*T)->left) == 2)
		{
			Tempcell = (*T)->right;
			if (Height(Tempcell->left) > Height(Tempcell->right))
				*T = DoubleRotateWithRight(*T);
			else
				*T = SingleRotateWithRight(*T);
		}
	}
	else if (X > (*T)->Element)
	{
		(*T)->right = Delete(X, &(*T)->right);
		if (Height((*T)->left) - Height((*T)->right) == 2)
		{
			Tempcell = (*T)->left;
			if (Height(Tempcell->right) > Height(Tempcell->left))
				*T = DoubleRotateWithLeft(*T);
			else
				*T = SingleRotateWithLeft(*T);
		}
	}
	else if ((*T)->left && (*T)->right)
	{
		Tempcell = FindMin((*T)->right);
		(*T)->Element = Tempcell->Element;
		(*T)->right = Delete((*T)->Element, &(*T)->right);
	}
	else
	{
		Tempcell = *T;
		if ((*T)->left == NULL)
			*T = (*T)->right;
		else if ((*T)->right == NULL)
			*T = (*T)->left;
		free(Tempcell);
	}
	if (*T != NULL)
		(*T)->Height = Max(Height((*T)->left), Height((*T)->right)) + 1;
	return *T;
}

Position SingleRotateWithLeft(Position K2)
{
	Position K1;
	K1 = K2->left;
	K2->left = K1->right;
	K1->right = K2;
	K2->Height = Max(Height(K2->left), Height(K2->right)) + 1;
	K1->Height = Max(Height(K1->left), K2->Height) + 1;
	return K1;
}

Position SingleRotateWithRight(Position K2)
{
	Position K1;
	K1 = K2->right;
	K2->right = K1->left;
	K1->left = K2;
	K2->Height = Max(Height(K2->left), Height(K2->right)) + 1;
	K1->Height = Max(Height(K1->right), K2->Height) + 1;
	return K1;
}

Position DoubleRotateWithLeft(Position K3)
{
	K3->left = SingleRotateWithRight(K3->left);
	return SingleRotateWithLeft(K3);
}

Position DoubleRotateWithRight(Position K3)
{
	K3->right = SingleRotateWithLeft(K3->right);
	return SingleRotateWithRight(K3);
}

void PreTraver(AvlTree T)
{
	if (T != NULL) 
	{
		printf("%d ", T->Element);
		PreTraver(T->left);
		PreTraver(T->right);
	}
}
