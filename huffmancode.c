#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BIT_MAX 16
#define CODETYPE_MAX 80
#define MAX 2000
typedef struct hnode{
	char litter;
	int weight;
	struct hnode * left;
	struct hnode * right;
	} Hnode;
typedef  Hnode * Huffman;

typedef struct{
	char litter;
	char bit[BIT_MAX];
}HuffCode;
typedef  HuffCode * Code;

Huffman BuildHuffman( Huffman Heap, int * size );
void ShowCode( Huffman Tree, Code arr );
void BuildHeap( Huffman arr, int * size );
Huffman HeapDelet( Huffman Heap, int * size );
void percedown( Huffman arr, int i, int * size );
void HeapInsert( Huffman Heap, Hnode node, int * size );
int Findlitter( char lit, Code arr );
int Findbit( char *bit, Code arr );
Huffman CreatHnode();
void swap( Huffman a, Huffman b )
{
	Hnode tmp = *a;
	*a = *b;
	*b = tmp;
}

int main(void)
{
	char name[20];
	char path[20];
	char codebit[BIT_MAX];
	int i, j, codenum, n;
	char txt[MAX];
	FILE *fp, *pf;
	Huffman Tree;
	HuffCode codearr[CODETYPE_MAX];
	Hnode HuffHeap[CODETYPE_MAX];
	for( i = 0; i < CODETYPE_MAX; i++ ){					//初始化结构体
		HuffHeap[ i ].weight = 1;
		HuffHeap[ i ].left = HuffHeap[ i ].right = NULL;
		HuffHeap[ i ].litter = '\0';
	}
	printf("请输入待压缩的文件路径和文件名,如C:\\\\和TEXT.txt \n");
	printf("路径 : ");
	scanf("%s", path);
	printf("文件名 : ");
	scanf("%s", name);
	strcat( path, name );
	fp = fopen( path, "r" );
	for( i = 0; (txt[ i ] = fgetc( fp )) != EOF && i < MAX; i++ )   //打开文件并存储到数组
		;
	fclose( fp );
	for( HuffHeap[0].litter = codearr[0].litter = txt[i--],	//存储字符类型
		codenum = 1; i > 0; i-- )							
	{
		for( j = 0; j < codenum; j++ )
			if(	txt[ i ] == HuffHeap[ j ].litter ){
				HuffHeap[ j ].weight++;
				break;
			}
			else if( j == codenum - 1 ){
				HuffHeap[codenum].litter = txt[ i ];
				codearr[codenum++].litter = txt[ i ];
				break;
			}
	}
	n = codenum;
	Tree = BuildHuffman( HuffHeap, &codenum );
	printf("编码成功:\n\n");
	ShowCode( Tree, codearr );
	for( i = 0; i < n; i++ )
		printf("\t\t%c\tcode:\t%s\n", codearr[ i ].litter, codearr[ i ].bit); //打印编码
	putchar('\n');
	printf("1.压缩到当前文件夹 2.压缩到其他文件夹\n");
	scanf("%d", &i);
	switch( i ){
	case 1:	
		path[strlen(path) - strlen(name)] = '\0';
		printf("请输入文件名 : ");
		scanf("%s", name);
		strcat( path, name);
		fp = fopen( path, "w" );
		break;
	case 2:
		printf("请输入路径 : ");
		scanf("%s", path);
		printf("请输入文件名 : ");
		scanf("%s", name);
		strcat( path, name );
		fp = fopen( path, "w" );
		break;
	}
	for( i = 0; txt[ i ] != EOF; i++ ){
		j = Findlitter( txt[ i ], codearr );
		fputs( codearr[ j ].bit, fp );
	}
	fclose( fp );
	fp = fopen( path, "r" );
	for( j = 0; fgetc( fp ) != EOF; j++ )	//压缩后文件位长
		;
	printf("压缩成功,压缩率是 : %d / (%d * 8.0) = %f%%\n", j, i, j/(i*8.0)*100 );
	fclose( fp );
	fp = fopen( path, "r");
	printf("\n解压压缩文件");
	printf("\n1.解压到当前文件夹 2.解压到其他文件夹\n");
	scanf("%d", &i);
	switch( i ){
	case 1:	
		path[strlen(path) - strlen(name)] = '\0';
		printf("请输入文件名 : ");
		scanf("%s", name);
		strcat( path, name);
		break;
	case 2:
		printf("请输入路径 : ");
		scanf("%s", path);
		printf("请输入文件名 : ");
		scanf("%s", name);
		strcat( path, name );
		break;
	}
	pf = fopen( path, "w" );
	for( i = 0; (codebit[i] = fgetc( fp )) != EOF; i++ ){
		codebit[i + 1] = '\0';
		if( (n = Findbit( codebit, codearr )) != -1 )
		{
			fputc( codearr[n].litter, pf );
			i = -1;
		}
	}
	fclose( fp );
	fclose( pf );
	return 0;
}

Huffman BuildHuffman( Huffman HuffHeap, int * size )
{

	int i;
	Huffman ptr0, header;
	BuildHeap( HuffHeap, size );
	for( i = *size - 1; i > 0; i-- ){
		ptr0 = CreatHnode();
		ptr0->left = HeapDelet( HuffHeap, size );
		ptr0->right = HeapDelet( HuffHeap, size );
		ptr0->weight = ptr0->left->weight + ptr0->right->weight;
		HeapInsert( HuffHeap, *ptr0, size );
	}
	header = HeapDelet( HuffHeap, size );
	return header;
}

void BuildHeap( Huffman arr, int * size )
{
	int i;
	for( i = *size / 2; i >= 0; i-- )
		percedown( arr, i, size );
}

Huffman HeapDelet( Huffman Heap, int * size )
{
	Huffman min = CreatHnode();
	*min = Heap[ 0 ];
	swap( &Heap[ 0 ], &Heap[*size - 1] );
	(*size)-- ;
	percedown( Heap, 0, size );
	return min;
}

void percedown( Huffman arr, int i, int * size )
{
	Hnode tmp;
	int child;
	for( tmp = arr[ i ]; i * 2 + 1 < *size; i = child ){
		child = i * 2 + 1;
		if( arr[child].weight > arr[child + 1].weight && child != *size - 1 )
			child++;
		if( arr[child].weight < tmp.weight )
			arr[ i ] = arr[child];
		else
			break;
	}
	arr[ i ] = tmp;
}

Huffman CreatHnode()
{
	Huffman node = (Huffman)malloc( sizeof(Hnode) );
	node->litter = '\0';
	node->weight = 0;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void HeapInsert( Huffman Heap, Hnode node, int * size )
{
	int i, parent;
	Hnode tmp = node;
	(*size)++;
	Heap[*size - 1] = node;
	for( i = *size - 1; i > 0; i = (i - 1) / 2 ){
		parent = (i - 1) / 2;
		if(	Heap[parent].weight > tmp.weight )
			Heap[ i ] = Heap[parent];
		else
			break;
	}
	Heap[ i ] = tmp;
}

void ShowCode( Huffman Tree, Code arr )
{
	int p;
	static int n = 0;
    static char bit0[BIT_MAX];
	if( Tree->left != NULL && Tree->right )
	{
		bit0[n++] = '0';
		ShowCode( Tree->left, arr );
		bit0[n++] = '1';
		ShowCode( Tree->right, arr );
		}
	if( Tree->litter != '\0' ){
		p = Findlitter( Tree->litter, arr );
		bit0[ n ] = '\0';
		strcpy(arr[ p ].bit, bit0);
	}
	 n--;
}

int Findlitter( char lit, Code arr )
{
	int i;
	for( i = 0; i < CODETYPE_MAX; i++ )
		if( arr[ i ].litter == lit )
			break;
	return i;
}

int Findbit( char *bit, Code arr )
{
	int i, j;
	for( i = 0; i < CODETYPE_MAX; i++ ){
		for( j = 0; arr[ i ].bit[ j ] != '\0' ; j++ )
			if( bit[ j ] != arr[ i ].bit[ j ] )
				break;
		if( arr[ i ].bit[ j ] == '\0' )
			return i;
	}
	return -1;
}
