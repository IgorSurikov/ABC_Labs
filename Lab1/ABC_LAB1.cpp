#include <iostream>
using namespace std;


void vector_calculation(_int8 *A, _int8 *B, _int8 *C, _int16 *D) {
	__int16 correctAnswer[8] = { 0 };
	__int16 F[8] = { 0 };
	__int8 F1[8] = { 0 };
	/*__int8 A[8];
	__int8 B[8];
	__int8 C[8];
	__int16 D[8];
	memcpy(A, A1, sizeof(A));
	memcpy(B, B1, sizeof(B));
	memcpy(C, C1, sizeof(C));
	memcpy(D, D1, sizeof(D));*/


	for (int i = 0; i < 8; i++)
	{
		correctAnswer[i] = (C[i] + B[i]) * A[i] * D[i];
	}

	__asm {
		mov ESI, B
		movq mm0, [ESI]
		mov ESI, C
		movq mm1, [ESI]
		paddsb mm0, mm1;
		movq qword ptr[F1], mm0

		mov ESI, A
		movq mm1, [ESI]
		mov ESI, D
		movq mm2, [ESI]

		punpcklbw mm1, mm1
		psrlw mm1, 8
		pmullw mm1, mm2
		movq qword ptr[F], mm1

		mov ESI, A
		movq mm3, [ESI]
		mov ESI, D
		movq mm2, [ESI + 8]
		punpckhbw mm3, mm3
		psrlw mm3, 8
		pmullw mm3, mm2
		movq qword ptr[F + 8], mm3

		movq mm4, mm0

		punpcklbw mm0, mm0
		psrlw mm0, 8
		pmullw mm0, mm1
		movq qword ptr[F], mm0

		punpckhbw mm4, mm4
		psrlw mm4, 8
		pmullw mm4, mm3
		movq qword ptr[F + 8], mm4
	}

	for (int i = 0; i < 8; i++)
	{
		cout << (int)F[i] << " ";
	}

	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << correctAnswer[i] << " ";
	}
	cout << endl;
}

int main()
{
	//F[i] = A[i] * (C[i]) + B[i])* D[i]

	__int8 A1[8]{ 2, 2, 1, 8, 4, 5, 3, 1 };
	__int8 B1[8]{ 1, 7, 1, 8, 1, 3, 1, -1 };
	__int8 C1[8]{ 8, -7, 3, -5, 2, 3, 2, 1 };
	__int16 D1[8]{ 3, 5, 1, 2, 3, -4, 9, -1 };


	__int8 A[8]{ 2, 2, 1, 10, 8, 5, 6, 1 };
	__int8 B[8]{ 1, 7, 1, 8, 4, 3, 1, -1 };
	__int8 C[8]{ 8, 7, 1, -5, 4, 3, 2, 1 };
	__int16 D[8]{ 3, 4, 1, 6, 7, -8, 9, -2 };

	vector_calculation(A, B, C, D);
	vector_calculation(A1, B1, C1, D1);



}




