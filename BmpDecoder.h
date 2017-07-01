
#pragma once

#include <stdio.h>

int rows;
int cols;
unsigned char *buffer = NULL;

void ReadHeader(FILE *fp)
{
	unsigned char bytes[4]; //一次读 4 个字节

	fseek(fp, 18, SEEK_SET); // 跳过前18个字节，读取Width, Height

	//read Width
	fread(bytes, 1, 4, fp);
	cols = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);

	//read Height
	fread(bytes, 1, 4, fp);
	rows = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}


//写入一行数组到 图像缓冲区中
void WriteRow(unsigned char *rowBuffer, int idx)
{
	int offset = idx * cols * 3; //第 idx 行, 每列3个像素

	for (int i = 0; i < cols * 3; i++)
	{
		buffer[offset + i] = rowBuffer[i];
	}
}


/* 调用此函数， fileName = 文件名 */
unsigned char *OpenImage(const char *fileName)
{
	FILE *fp = fopen(fileName, "rb+"); //二进制方式读取

	ReadHeader(fp); //读取图片的 Width, Height
	
	buffer = new unsigned char[rows * cols * 3];  // 24位真彩色，3个字节

	/* 从文件头开始，偏移54个字节（文件头信息长度） */
	fseek(fp, 54, SEEK_SET);

	int offset = (cols * 24 + 31) / 32 * 4; // 计算每行存储字节数
	unsigned char *rowBuffer = new unsigned char[offset]; //行缓冲区

	for (int i = 0; i < rows; i++)
	{
		fread(rowBuffer, 1, offset, fp); //读取一行
		WriteRow(rowBuffer, rows - 1 - i); //写入到缓冲区
	}
	
	delete []rowBuffer; //释放行缓冲区
	fclose(fp);         //关闭文件
	return buffer;
}


/* 关闭图像 */
void CloseImage()
{
	delete[] buffer; // 删除文件缓冲区
}