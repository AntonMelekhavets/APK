#include <iostream>
#include <ctime>
#include <conio.h>
using namespace std;

#define SIZE 8 // размер матриц
#define ITER_COUNT 1000000 // количество вычислений

void fill_matrix(short matrix1[SIZE][SIZE], short matrix2[SIZE][SIZE], short result_matrix[SIZE][SIZE]);
void show_matrix(short result_matrix[SIZE][SIZE]);
void null_matrix(short result_matrix[SIZE][SIZE]);

int main()
{
	char again;
	clock_t result_time; 
	short matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE];
	int iter_asm = SIZE*SIZE;  // количество вычислений дл€ матрицы (asm)
	int iter_MMX = iter_asm/4; // количество вычислений дл€ MMX (по 4 элемента сразу)
	do {
		system("cls");
		fill_matrix(matrix1, matrix2, result_matrix); // заполнение матриц
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
				pusha;									// сохранение всех реистров процессора
				xor esi, esi;	 
				mov ecx, iter_asm;	// занесение в регистр ecx количества векторов, над которыми проивзвод€тс€ вычислени€
			calculation_asm:
				xor dx,dx;
				mov ax, matrix1[esi];
				mov bx, matrix2[esi];
				or ax, bx // выполнение операции побитовое »Ћ» дл€ двух векторов
				mov result_matrix[esi], ax;
				add esi, 2; //перемещение к следующему элементу матрицы (short = 2 байта)
			loop calculation_asm;
				popa; //извлечение значений всех регистров из стека
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
				pusha; // сохранение значений всех регистров
				xor esi, esi;
				mov ecx, iter_MMX; //добавление в ecx количества элементов матрицы дл€ вычислени€ 
			calculation_mmx:
				movq MM1, matrix1[esi];
				movq MM2, matrix2[esi];
				por MM1, MM2;   //выполнение операции побитовое »Ћ»
				movq result_matrix[esi], MM1;
				add esi, 8;   // переход с следующей группе элементов дл€ вычислени€ (4 элемента, каждый по 2 байта)
			loop calculation_mmx;
				emms; //возвращение управлени€ от команд MMX
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