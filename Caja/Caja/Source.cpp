#include <complex>
#include <iostream>
#include <valarray>
#include<time.h>
#include<thread>
#include "../mjerenje.h"
#include "../loading.h"
using namespace std;
double PI = 3.141592653589793238460;
const int num_threads = 4;
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
int redak, stupac;
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray &x)
{
	const int N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[slice(0, N / 2, 2)];
	CArray  odd = x[slice(1, N / 2, 2)];

	// conquer

	fft(even);
	fft(odd);

	// combine
	for (int k = 0; k < N / 2; ++k)

	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}
void fftP(CArray &x, int poc, int kraj)
{
	const int N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[slice(0, N / 2, 2)];
	CArray  odd = x[slice(1, N / 2, 2)];

	// conquer

	fft(even);
	fft(odd);

	// combine
	for (int k = poc; k < kraj; ++k)

	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}
int main()
{
	Complex *test = NULL;
	srand(time(NULL));
	int dim;
	printf("Koliko brojeva zelite ?\n");
	scanf("%d",&dim);
	test=(Complex*)malloc(dim*sizeof(Complex));
	for (int i = 0; i < dim; i++)
	{
		test[i]=rand()%10;
	}

	double wall1 = get_wall_time();//Poèetak mjerenja
	CArray data(test, dim);

	// forward fft
	fft(data);

	std::cout << "fft" << std::endl;
	for (int i = 0; i < dim; ++i)
	{
		std::cout << data[i] << std::endl;
	}
	double wall2 = get_wall_time();//Završetak mjerenja

	printf("\nVrijeme izvodenja sekvencijalno %lf s\n", wall2 - wall1);
	printf("-------------------------------------------------------------------------------------\n");

	printf("paralelno\n");
	wall1 = get_wall_time();//Poèetak mjerenja
	CArray data2(test, dim);
	std::thread t[num_threads];

	for (int i = 0;i<num_threads;i++)
	{
		int poc, kraj, korak;
		if (i<num_threads - 1)
		{

			korak = redak / num_threads;
			poc = i * korak;
			kraj = poc + korak;
			t[i] = std::thread( fftP, data2,poc,kraj);
		}
		else // za neparne dimenzije
		{
			korak = redak / num_threads;
			poc = i * korak;
			kraj = poc + korak + redak % num_threads;

			t[i] = std::thread(fftP , data2,poc,kraj);
		}


	}
	for (int i = 0;i<num_threads;i++)
		t[i].join();
	wall2 = get_wall_time();//Završetak mjerenja

	printf("\nVrijeme izvodenja paralelno %lf s\n", wall2 - wall1);
	system("pause");
	return 0;
}