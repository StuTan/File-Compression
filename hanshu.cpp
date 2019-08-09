/*hanshu.c*/
#include "huffman.h"
#include<bits/stdc++.h>

using namespace std;

void Huffman::create_node_array()
{
    int i, count;
    count = 0;
    for(i = 0; i < MAX_SIZE; ++i)
    {
        if(calculate[i] <= 0)
            continue;
        Noder node = new Huffman_node ();
        node->id = i;
        node->calculate = calculate[i];
        node->code = "";
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node_array[count++] = node;
    }
    // 插入假的停止符stop
    Noder node = new Huffman_node();
    node->id = stop;
    node->calculate = 1;
    node->code = "";
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL; 
    node_array[count++] = node; 
    size = count;
}

void Huffman::create_map_1(const Noder node, bool left)
{
    if(left)
        node->code = node->parent->code + "0";
    else
        node->code = node->parent->code + "1"; 
    if(node->left == NULL && node->right == NULL)
        map_1[node->id] = node->code;
    else
    {
        if(node->left != NULL)
            create_map_1(node->left, true);
        if(node->right != NULL)
            create_map_1(node->right, false);
    }
}

void Huffman::create_pq()
{
    int i;
    create_node_array();

    for(i = 0; i < size; ++i)
        pq.push(node_array[i]);
}

void Huffman::create_huffman_tree()
{
    root = NULL;

    while( !pq.empty() )
    {       
        Noder first = pq.top();
        pq.pop();
        if( pq.empty() )
        {
            root = first;
            break;
        }
        Noder second = pq.top();
        pq.pop();
        Noder new_node = new Huffman_node();
        new_node->calculate = first->calculate + second->calculate;

        if(first->calculate <= second->calculate)
        {
            new_node->left = first;
            new_node->right = second;
        }
        else
        {
            new_node->left = second;
            new_node->right = first;
        }
        first->parent = new_node;
        second->parent = new_node;

        pq.push(new_node);
        
    }
    
}

void Huffman::calculate_huffman_codes()
{
    if(root == NULL)
    {
        printf("建哈夫曼树失败\n");
        exit(1);
    }

    if(root->left != NULL)
        create_map_1(root->left, true);
    if(root->right != NULL)
        create_map_1(root->right, false);
}

void Huffman::do_compress()
{
    int length, i, j, byte_count;
    char in_char;
    unsigned char out_c, tmp_c;
    string code, out_string;
    map<int, string>::iterator map_1_it;
    // 第1行写入总结点数 
   // cout<<"size="<<size<<endl;
    out_file.write((char*)&size,sizeof(size));

    // 第2~(size+1)行写入字符和其相对应的个数 
    for(i=0;i<256;i++)
    {
    	if(calculate[i]!=0)
        {
			out_file.write((char*)&i,sizeof(i));
         	out_file.write((char*)&calculate[i],sizeof(calculate[i]));
		 } 
    }

    // 第size+2行写入huffman编码
    in_file.clear();
    in_file.seekg(ios::beg);
    code.clear();
    while( in_file.get(in_char))
    {
    	cout<<in_char;
        map_1_it = map_1.find((unsigned char)in_char);
        if(map_1_it != map_1.end() )
            code += map_1_it->second;
        else
        {
            printf("Can't find the huffman code of character %X\n", in_char);
            exit(1);
        }
        length = code.length();
        if(length > 10)
        {
            out_string.clear();
             for(i = 0; i + 7 < length; i += 8)
            {
                out_c = 0;
                for(j = 0; j < 8; j++)
                {
                    if('0' == code[i+j])
                        tmp_c = 0;
                    else
                        tmp_c = 1;
                    out_c += tmp_c<<(7-j);
                }
                out_string += out_c;
            }
            out_file << out_string;
           // cout<<out_string;
            code = code.substr(i, length-i);
        }
    }

    // 读完所有文件后，插入假的停止符stop 
    map_1_it = map_1.find(stop);
    if(map_1_it != map_1.end() )
        code += map_1_it->second;
    else
    {
        printf("Can't find the huffman code of  \n");
        exit(1);
    }
    //处理剩余的编码 
    length = code.length();
    out_c = 0;
    for(i = 0; i < length; i++)
    {
        if('0' == code[i])
            tmp_c = 0;
        else
            tmp_c = 1;
        out_c += tmp_c<<(7-(i%8));
        if(0 == (i+1) % 8 || i == length - 1)
        {
            out_file<<out_c;
            out_c = 0;
        }
    } 
}

void Huffman::rebuid_huffman_tree()
{
    in_file.read((char*)&size,sizeof(size));
    cout<<"size="<<size<<endl;
    int i;
    size--;
    while(size--)
    {
   		in_file.read((char*)&i,sizeof(i));
   		cout<<"i="<<i;
	    in_file.read((char*)&calculate[i],sizeof(calculate[i])); 	
	    cout<<" calculate[i]="<<calculate[i]<<endl;
	}
	create_pq();
    create_huffman_tree();
    calculate_huffman_codes();
}

void Huffman::decode_huffman()
{
    bool stop1=false;
    int i, id;
    char in_char;
    string out_string;
    unsigned char u_char, flag;
    Noder node;

    out_string.clear();
    node = root;  
    while(in_file.get(in_char))
    {
    	//aCount++;	 
        cout<<  in_char ;
        u_char = (unsigned char)in_char;
        flag = 0x80;
        for(i = 0; i < 8; ++i)
        {

            if(u_char &  flag)
                node = node->right;
            else
                node = node->left;

            if(node->left == NULL && node->right == NULL)
            {
                id = node->id;
                 if(id == stop)
                {
                    stop1 = true;
                    cout<<"是因为遇到了假的停止符才停下来的"<<endl; 
                    break;
                }
                else
                {
                    out_string += (char)node->id;
                    node = root;
                }
            }
            flag = flag >> 1;
        }
        
        if(!out_string.empty() )
        out_file << out_string;
        out_string.clear();
    } 
    cout<<"这是一个分界线, " ;
	//<< "aCount = " << dec << aCount <<endl; 
    if(!out_string.empty() )
        out_file << out_string;
}

Huffman::Huffman(string in_file_name, string out_file_name)
{
    in_file.open(in_file_name.c_str(), ios::in | ios::binary);
    if(!in_file)
    {
        printf("打开原文件错误, 路径是: %s\n", in_file_name.c_str());
        exit(1);
    }

    out_file.open(out_file_name.c_str(), ios::out|ios::binary);
    if(!out_file)
    {
        printf("打开目标文件错误，路径是: %s\n", out_file_name.c_str());
        exit(1);
    }
}
void DestroyBiTree(Noder &T)
{
	if (T == NULL) return ;
	DestroyBiTree(T->left);
	DestroyBiTree(T->right);
	delete T;
	T = NULL;
	return ;
}

Huffman::~Huffman()
{
    in_file.close();
    out_file.close();
	DestroyBiTree(root);
}

void Huffman::compress()
{
	char in_char; 
    while( in_file.get(in_char))
    { 
        calculate[(unsigned char)in_char]++; 
    }
    create_pq();
    create_huffman_tree();
    calculate_huffman_codes();
    do_compress();
}

void Huffman::decompress()
{
    rebuid_huffman_tree();
    decode_huffman(); 
} 
