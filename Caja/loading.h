#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>


typedef struct {
	int x, y;
	float **red, **green, **blue;

} PPMImage;


typedef struct {
	int x, y;
	float **grey;

} PGMImage;

#define CREATOR "FSRE"
#define RGB_COMPONENT_COLOR 255

void alloc_matrix(float ***matrix, long  rowN, long  colN) {
	/*alocira prostor za matrice dimenzije rowN * colN */


	long i;

	*matrix = (float **)malloc(rowN * sizeof(float *));
	if (*matrix == NULL)
	{
		printf("alokacija matrice: nema dovoljno prostora\n");
		exit(1);
	}
	for (i = 0; i<rowN; i++)
	{
		(*matrix)[i] = (float *)malloc(colN * sizeof(float));
		if ((*matrix)[i] == NULL)
		{
			printf("alokacija matrice: nema dovoljno prostora\n");
			exit(1);
		}
	}
	return;
}
void disalloc_matrix(float **matrix, long  rowN, long  colN) {

	//oslobaðamo alociranu memoriju za matrice
	long i;
	for (i = 0; i<rowN; i++)
		free(matrix[i]);
	free(matrix);
	return;
}
int ucitajPPM(char *filename, PPMImage *slika)
{

	char   rows[2600];

	int brojac = 0;
	int *buffer, x = 1, y = 1;
	FILE   *fp;

	if ((fp = fopen(filename, "r")) == NULL) return 0;
	printf("%s", filename);
	printf("Ucitavamo sliku %s ...", filename);

	//oslobaðamo se prva 4 retka za metainformacijama o formatu i velicini slike
	fgets(rows, 2600, fp);
	fgets(rows, 2600, fp);
	fscanf_s(fp, "%d %d", &x, &y);
	printf("\n\tBroj redaka %d", x);
	printf("\n\tBroj stupaca %d\n", y);

    //	printf("%d %d",x,y);
	slika->x = x;
	slika->y = y;
	alloc_matrix(&(slika->red), y, x);
	alloc_matrix(&(slika->green), y, x);
	alloc_matrix(&(slika->blue), y, x);
	buffer = (int *)malloc(3 * x*y * sizeof(int));
	fgets(rows, 2600, fp);
	fgets(rows, 2600, fp);
	int gh;
	while (fscanf_s(fp, "%d", &gh) == 1)
	{
		buffer[brojac] = gh;
		brojac++;
	}

	fclose(fp);

	int g = 0, n = 0;
	//printf("%d %d %d",brojac,y,x);
	for (int k = 0; k<brojac; k++)
	{
		g = (k / 3) / x;
		n = (k / 3) % x;
		if (k % 3 == 0) {
			slika->red[g][n] = (float)buffer[k];
		}
		else if (k % 3 == 1) {
			slika->green[g][n] = (float)buffer[k];
		}
		else {
			slika->blue[g][n] = (float)buffer[k];
		}
	}


	return 1;
}
int ucitajPGM(char *filename, PGMImage *slika)
{

	char   rows[2600];

	int brojac = 0;
	int *buffer, x = 1, y = 1;
	FILE   *fp;

	if ((fp = fopen(filename, "rb")) == NULL) return 0;
	printf("%s", filename);
	printf("Ucitavamo sliku %s ...", filename);

	//oslobaðamo se prva 4 retka za metainformacijama o formatu i velicini slike
	fgets(rows, 2600, fp);
	fgets(rows, 2600, fp);
	fscanf_s(fp, "%d %d", &x, &y);
	printf("\n\tBroj redaka %d", x);
	printf("\n\tBroj stupaca %d\n", y);
	//printf("%d %d",x,y);
	slika->x = x;
	slika->y = y;
	alloc_matrix(&(slika->grey), y, x);
	buffer = (int *)malloc(x*y * sizeof(int));
	fgets(rows, 2600, fp);
	fgets(rows, 2600, fp);
	int gh;
	while (fscanf_s(fp, "%d", &gh) == 1)
	{
		buffer[brojac] = gh;
		brojac++;
	}

	fclose(fp);

	int g = 0, n = 0;
	//printf("%d %d %d",brojac,y,x);
	for (int k = 0; k<brojac; k++)
	{
		g = (k) / x;
		n = (k) % x;

		slika->grey[g][n] = (float)buffer[k];

	}
	return 1;
}
void zapisiPPM(const char *filename, PPMImage *img)
{
	FILE *fp;

	//otvara datoteku za izlaz
	fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "Greska u otvaranju datoteke '%s'\n", filename);
		exit(1);
	}

	//upisuje datoteku zaglavlju
	//format slike
	fprintf(fp, "P3\n");

	//komentira
	fprintf(fp, "# Created by %s\n", CREATOR);

	//
	fprintf(fp, "%d %d\n", img->x, img->y);

	// dubina rgb komponenta 
	fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

	// pixel podaci
	for (int i = 0; i<img->y; i++)
	{   fprintf(fp,"\n");
		for (int j = 0; j<img->x; j++)
			fprintf(fp, "%d %d %d ", (int)img->red[i][j], (int)img->green[i][j], (int)img->blue[i][j]);
	}
	fclose(fp);
}
void zapisiPGM(const char *filename, PGMImage *img)
{
	FILE *fp;

	//otvara datoteku za izlaz
	fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "Greska u otvaranju datoteke '%s'\n", filename);
		exit(1);
	}

	//upisuje datoteku zaglavlja
	//format slike
	fprintf(fp, "P2\n");

	//komentari
	fprintf(fp, "# Created by %s\n", CREATOR);

	//velicina slike
	fprintf(fp, "%d %d\n", img->x, img->y);

	// dubina rgb komponente
	fprintf(fp, "%d", RGB_COMPONENT_COLOR);

	// pixel podaci
	for (int i = 0; i<img->y; i++)
	{
		fprintf(fp, "\n");
		for (int j = 0; j<img->x; j++)
			fprintf(fp, "%d ", (int)img->grey[i][j]);
	}
	fclose(fp);
	printf("\nSlika %s spremljena!\n", filename);
}