// 这是使用应用程序向导生成的 VC++ 
// 应用程序项目的主项目文件。

#include "stdafx.h"

#using <mscorlib.dll>

using namespace System;

#include <stdlib.h>
#include <stdio.h>


int _tmain()
{
    // TODO: 请用您自己的代码替换下面的示例代码。
	char buf[40960];
	char *targetbuf;
	int num,num2;
    Console::WriteLine(S"Hello World");

	FILE *fp,*d2server;
	fp = fopen("1.exe","rb");
	if (fp==NULL)
	{
		Console::WriteLine(S"Can't open 1.exe to read");
		return 0;
	}

	num = fread(buf,1,40960,fp);
	fclose(fp);
	
	printf("1.exe has %d bytes\n",num);

	d2server = fopen("d2server.dll","rb");
	if (d2server==NULL)
	{
		Console::WriteLine(S"Can't open d2server.dll to read");
		return 0;
	}

	targetbuf = (char *)malloc(1024*1024);
	num2 = fread(targetbuf,1,1024*1024,d2server);
	fclose(d2server);
	printf("d2server.dll has %d bytes\n",num2);

	d2server = fopen("d2server.dll","wb");
	if (d2server==NULL)
	{
		free(targetbuf);
		Console::WriteLine(S"Can't open d2server.dll to write");
		return 0;
	}

	if (num>512 && num2 > 0xFA00)
	{
		fseek(d2server,0,SEEK_SET);
		fwrite(targetbuf,1,0xFA00,d2server);
		fwrite(&buf[512],1,num-512,d2server);
		memset(targetbuf,0,1024*100);
		fwrite(targetbuf,1,16384-num+512,d2server);
	}
	else
	{
		Console::WriteLine(S"Error, patch code less than 512 bytes!");
	}
	free(targetbuf);
	fclose(d2server);
    Console::WriteLine(S"Done");

	return 0;
}
