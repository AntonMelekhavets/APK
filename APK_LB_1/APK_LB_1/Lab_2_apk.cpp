#include <iostream>
#include <ctime>
#include <conio.h>
using namespace std;

#define SIZE 8 // ������ ������
#define ITER_COUNT 1000000 // ���������� ����������

void fill_matrix(short matrix1[SIZE][SIZE], short matrix2[SIZE][SIZE], short result_matrix[SIZE][SIZE]);
void show_matrix(short result_matrix[SIZE][SIZE]);
void null_matrix(short result_matrix[SIZE][SIZE]);

int main()
{
	char again;
	clock_t result_time; 
	short matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE];
	int iter_asm = SIZE*SIZE;  // ���������� ���������� ��� ������� (asm)
	int iter_MMX = iter_asm/4; // ���������� ���������� ��� MMX (�� 4 �������� �����)
	do {
		system("cls");
		fill_matrix(matrix1, matrix2, result_matrix); // ���������� ������
		result_time = clock();							// C++ 
		for(int temp = 0; temp<ITER_COUNT; temp++)
		{
			for(int i=0; i<SIZE; i++)
			{
				for(int j=0; j<SIZE; j++)
					result_matrix[i][j] = matrix1[i][j] | matrix2[i][j];
			}
		}
		result_time -= clock();
		cout<<"C++: "<<-result_time<<" (ms)"<<endl;
		show_matrix(result_matrix);
		null_matrix(result_matrix);
		result_time = clock();							//Asm
		for(int i = 0; i<ITER_COUNT; i++)
		{
			_asm 
			{
				pusha;									// ���������� ���� �������� ����������
				xor esi, esi;	 
				mov ecx, iter_asm;	// ��������� � ������� ecx ���������� ��������, ��� �������� ������������� ����������
			calculation_asm:
				xor dx,dx;
				mov ax, matrix1[esi];
				mov bx, matrix2[esi];
				or ax, bx // ���������� �������� ��������� ��� ��� ���� ��������
				mov result_matrix[esi], ax;
				add esi, 2; //����������� � ���������� �������� ������� (short = 2 �����)
			loop calculation_asm;
				popa; //���������� �������� ���� ��������� �� �����
			}
		}
		result_time -= clock();
		cout<<"Asm: "<<-result_time<<" (ms)"<<endl;
		show_matrix(result_matrix);
		null_matrix(result_matrix);
		result_time = clock(); //MMX
		for(int i = 0; i<ITER_COUNT; i++)
		{
			_asm 
			{
				pusha; // ���������� �������� ���� ���������
				xor esi, esi;
				mov ecx, iter_MMX; //���������� � ecx ���������� ��������� ������� ��� ���������� 
			calculation_mmx:
				movq MM1, matrix1[esi];
				movq MM2, matrix2[esi];
				por MM1, MM2;   //���������� �������� ��������� ���
				movq result_matrix[esi], MM1;
				add esi, 8;   // ������� � ��������� ������ ��������� ��� ���������� (4 ��������, ������ �� 2 �����)
			loop calculation_mmx;
				emms; //����������� ���������� �� ������ MMX
				popa;
			}
		}
		result_time -= clock();
		cout<<"MMX "<<-result_time<<" (ms)"<<endl;
		show_matrix(result_matrix);
		do 
		{
			cout << "Do you want to repeat? ('y' - yes, 'n' - no): " << endl;
			again = getch();
		} while (again != 'y' && again != 'n');
	} while (again == 'y');
	return 0;
}

void fill_matrix(short matrix1[SIZE][SIZE],short matrix2[SIZE][SIZE],short result_matrix[SIZE][SIZE])
{
	srand(time(0));
	for(int i=0;i<SIZE;i++)
	{
		for(int j=0;j<SIZE;j++) 
		{
			matrix1[i][j] = (short)rand();
			matrix2[i][j] = (short)rand();
			result_matrix[i][j] = 0;
		}
	}
}

void show_matrix(short result_matrix[SIZE][SIZE])
{

	for(int i=0;i<SIZE;i++) 
	{ 
		for(int j=0;j<SIZE;j++)
		{
			if(result_matrix[i][j]<10000)
				cout<<result_matrix[i][j]<<"   ";
			else
				cout<<result_matrix[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void null_matrix(short result_matrix[SIZE][SIZE])
{
	for(int i=0;i<SIZE;i++) 
	{
		for(int j=0;j<SIZE;j++)
			result_matrix[i][j]=0;
	}
}