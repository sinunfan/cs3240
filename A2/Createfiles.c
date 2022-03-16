#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Searchfiles.h"

void formatfile(FILE *Ofile);
void sortzip();
int cmpfunc(const void *a, const void *b);

int bs(char a[], int length, char key);

FILE *Nfile;

struct city
{
    char cityname[100];
    int zip;
    long position;
};

void sortcity();
int location = 0;

int main(int argc, char *argv[])
{

    if (strcmp(argv[2], "new") == 0)
    {
        Nfile = fopen(argv[1], "r");
        if (Nfile == NULL)
        {
            printf("Cannot open the file\n");
            exit(0);
        }
        formatfile(Nfile);
        //sort by the zip

        sortzip();
    }
    else if (strcmp(argv[2], "append") == 0 && argv[3]!=NULL)
    {
        char buf[100];
        int i;
        Nfile = fopen(argv[1], "r");
        FILE * appendfile = fopen(argv[3], "r");
        if (Nfile == NULL)
        {
            printf("Cannot open the file\n");
            exit(0);
        }
        if (appendfile == NULL)
        {
            printf("Cannot open the append file\n");
            exit(0);
        }
        FILE * tempfile = fopen("temp.txt", "a+");
        while (fscanf(Nfile, "%s %d", buf, &i) == 2)
        {
            fprintf(tempfile, "%s %d \n", buf, i);
        }
        while (fscanf(appendfile, "%s %d", buf, &i) == 2)
        {
            fprintf(tempfile, "%s %d \n", buf, i);
        }
        //FILE * readtemp = fopen("temp.txt", "r");
        rewind(tempfile);

        formatfile(tempfile);
        sortzip();
        
        fclose(tempfile);
        fclose(Nfile);
        //fclose(readtemp);

    }
    else if(argv[3] == NULL){
        printf("please put the file that you want to append to the result\n");
    }
    else if (strcmp(argv[2], "append")!=0 || strcmp(argv[2], "new") != 0)
    {
        printf("Wrong input argument or Wrong format (please check readme file)\n");
    }
    // printf("hi");



    // Ofile = fopen("cz1.txt", "r");

    // if (Ofile == NULL)
    // {
    //     printf("Cannot open the file");
    //     exit(0);
    // }
    // formatfile(Ofile);
    // //sort by the zip

    // sortzip();

    return 0;
}

void formatfile(FILE *Ofile)
{
    char buf[100];
    int i;
    struct city ct[1000];

    FILE *CityAndZip = fopen("CityAndZip.txt", "w");
    // fscanf(Ofile,"%s %d",buf, &i);
    // printf("%s\n", buf);

    while (fscanf(Ofile, "%s %d", buf, &i) == 2)
    {

        strcpy(ct[location].cityname, buf);
        ct[location].zip = i;
        ct[location].position = ftell(CityAndZip);

        //printf("%s %d %ld %d\n",ct[location].cityname, ct[location].zip, ftell(CityAndZip), location);
        fprintf(CityAndZip, "%s %d \n", ct[location].cityname, ct[location].zip);
        //fprintf(CityAndPosition, "%s %ld \n",ct[location].cityname, ct[location].position);
        //     printf("%s %d %ld %d\n",ct[location].cityname, ct[location].zip, ct[location].position, location);
        location++;
    }

    fclose(Ofile);
    fclose(CityAndZip);
}

// sort the zip code from min to max
void sortzip()
{
    FILE *CityAndZip1 = fopen("CityAndZip.txt", "r");
    char buf2[100];
    int i, k;
    struct city ct2[1000];

    FILE *ZipAndPosition = fopen("ZipAndPosition.txt", "w");

    for (k = 0; k < location; k++)
    {
        ct2[k].position = ftell(CityAndZip1);
        fscanf(CityAndZip1, "%s %d", buf2, &i);
        strcpy(ct2[k].cityname, buf2);
        ct2[k].zip = i;
        //printf("%s %d \n",buf2, ct2[k].zip);
    }

    //sort the struct array by zip from min to max
    qsort(ct2, location, sizeof(*ct2), cmpfunc);

    //use for loop to put the result to txt file called ZipAndPostion
    for (k = 0; k < location; k++)
    {
        fprintf(ZipAndPosition, "%d %ld \n", ct2[k].zip, ct2[k].position);
        printf("%s %d %ld %d\n", ct2[k].cityname, ct2[k].zip, ct2[k].position, k);
    }

    // sort by alphabetical order
    sortcity();

    fclose(CityAndZip1);
    fclose(ZipAndPosition);
}

//compera the all zip that been collect from txt file
int cmpfunc(const void *a, const void *b)
{
    struct city *z1 = (struct city *)a;
    struct city *z2 = (struct city *)b;
    return (z1->zip - z2->zip);
}

// sort city by alphabetical
void sortcity()
{
    FILE *CityAndZip1 = fopen("CityAndZip.txt", "r");
    char buf3[100];
    struct city ct3[1000];
    int i, j, k = 0;
    char temp[100];
    int tempzip;
    int tempposition;
    FILE *CityAndPosition = fopen("CityAndPosition.txt", "w");

    for (k = 0; k < location; k++)
    {
        ct3[k].position = ftell(CityAndZip1);
        fscanf(CityAndZip1, "%s %d", buf3, &i);
        strcpy(ct3[k].cityname, buf3);
        ct3[k].zip = i;
        //printf("%s %d \n",buf2, ct2[k].zip);
    }
    for (i = 0; i < location; i++)
    {
        for (j = i + 1; j < location; j++)
        {
            if (strcmp(ct3[i].cityname, ct3[j].cityname) > 0)
            {
                strcpy(temp, ct3[i].cityname);
                tempzip = ct3[i].zip;
                tempposition = ct3[i].position;
                strcpy(ct3[i].cityname, ct3[j].cityname);
                ct3[i].zip = ct3[j].zip;
                ct3[i].position = ct3[j].position;
                strcpy(ct3[j].cityname, temp);
                ct3[j].zip = tempzip;
                ct3[j].position = tempposition;
            }
        }
    }
    for (k = 0; k < location; k++)
    {
        //fprintf(CityAndZip, "%s %d \n",ct2[k].cityname, ct2[k].zip);
        fprintf(CityAndPosition, "%s %ld \n", ct3[k].cityname, ct3[k].position);
        printf("%s %d %ld %d\n", ct3[k].cityname, ct3[k].zip, ct3[k].position, k);
    }
    fclose(CityAndPosition);
}
