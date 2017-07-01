
#pragma once

#include <stdio.h>

int rows;
int cols;
unsigned char *buffer = NULL;

void ReadHeader(FILE *fp)
{
	unsigned char bytes[4]; //һ�ζ� 4 ���ֽ�

	fseek(fp, 18, SEEK_SET); // ����ǰ18���ֽڣ���ȡWidth, Height

	//read Width
	fread(bytes, 1, 4, fp);
	cols = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);

	//read Height
	fread(bytes, 1, 4, fp);
	rows = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}


//д��һ�����鵽 ͼ�񻺳�����
void WriteRow(unsigned char *rowBuffer, int idx)
{
	int offset = idx * cols * 3; //�� idx ��, ÿ��3������

	for (int i = 0; i < cols * 3; i++)
	{
		buffer[offset + i] = rowBuffer[i];
	}
}


/* ���ô˺����� fileName = �ļ��� */
unsigned char *OpenImage(const char *fileName)
{
	FILE *fp = fopen(fileName, "rb+"); //�����Ʒ�ʽ��ȡ

	ReadHeader(fp); //��ȡͼƬ�� Width, Height
	
	buffer = new unsigned char[rows * cols * 3];  // 24λ���ɫ��3���ֽ�

	/* ���ļ�ͷ��ʼ��ƫ��54���ֽڣ��ļ�ͷ��Ϣ���ȣ� */
	fseek(fp, 54, SEEK_SET);

	int offset = (cols * 24 + 31) / 32 * 4; // ����ÿ�д洢�ֽ���
	unsigned char *rowBuffer = new unsigned char[offset]; //�л�����

	for (int i = 0; i < rows; i++)
	{
		fread(rowBuffer, 1, offset, fp); //��ȡһ��
		WriteRow(rowBuffer, rows - 1 - i); //д�뵽������
	}
	
	delete []rowBuffer; //�ͷ��л�����
	fclose(fp);         //�ر��ļ�
	return buffer;
}


/* �ر�ͼ�� */
void CloseImage()
{
	delete[] buffer; // ɾ���ļ�������
}