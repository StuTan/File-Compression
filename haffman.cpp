 /**
 * main.cpp 
 */
#include <iostream>
#include <cstdio>    
#include "huffman.h"
  
using namespace  std;


int main(int argc, char *argv[]) 

{
	printf("\nexplanation:\n\t c refer to 压缩\n");
	printf("\t d refer to 解压\n");
	printf("\t please input in order :haffman c/d filename newname\n ");
    if(argc != 4)
    {
        printf("error:\n\t 输入不符合标准\n");
        exit(1);
    }

    if(0 == strcmp("c", argv[1]))
    {
        Huffman h(argv[2], argv[3]);
        h.compress();
        cout<<"\t压缩成功"<<endl; 
    }
    else if(0 == strcmp("d", argv[1]))
    {
        Huffman h(argv[2], argv[3]);
        h.decompress();
        cout<<"\t解压成功"<<endl; 
    }
    else
    {
        printf("error:\n\t unkonwn command\n");
    }
    return 0;
} 
