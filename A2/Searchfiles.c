#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int bs(char a[], int length, char key);

char city[1000][100];
int cityIndex[1000];

int zip[1000];
int zipIndex[1000];

int binary_search_zip(int size, char target[]);
int binary_search_city(int arr[], int l, int r, int x);


int main(int argc, char *argv[]) {

    char buf[100];
    int ip, z, zp, citylocation=0, ziplocation=0;
    FILE * cityfile = fopen("CityAndPosition.txt", "r");
    FILE * zipfile = fopen("ZipAndPosition.txt", "r");


    if(cityIndex == NULL){
        printf("Cannot open the file");
        exit(0);
    }
    while(fscanf(cityfile,"%s %d",buf,&ip)==2){
        strcpy(city[citylocation], buf);
        cityIndex[citylocation] = ip;
        citylocation++;
    }
    while(fscanf(zipfile,"%d %d",&z,&zp)==2){
        zip[ziplocation] = z;
        zipIndex[ziplocation] = zp;
        ziplocation++;
    }

    for(int l = 0; l < citylocation; l++){
        printf("%s %d\n", city[l], cityIndex[l]);
    }
    for(int e = 0; e < ziplocation; e++){
        printf("%d %d\n", zip[e], zipIndex[e]);
    }


    char input[100];
    printf("Enter the zip or city that you want to search:\n");
    scanf("%s", input);

    if(!isdigit(input[0])){
        if (binary_search_zip(citylocation,input) == -1){
            printf("cannot found the result\n");
        }
    }
    else{
        if (binary_search_city(zip, 0, (ziplocation-1), atoi(input)) == -1){
            printf("cannot found the result\n");
        }
    }
    

    ziplocation=0;
    citylocation=0;

    if(zipIndex == NULL){
        printf("Cannot open the file");
        exit(0);
    }

     return 0;

}

int binary_search_zip(int size, char target[]){
    int bottom= 0;
    int mid;
    int top = size - 1;
    int found = -1;
    //printf("%s", target);
    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(city[mid], target) == 0){
            FILE *cityandzip = fopen("CityAndZip.txt", "r");

            fseek(cityandzip,cityIndex[mid],SEEK_CUR);
            int zipcode;
            fscanf(cityandzip,"%*s %d", &zipcode);
            printf("%s zipcode is %d.\n", target, zipcode);
            fclose(cityandzip);
            found = 1;
            return found;
            break;
        }
        if (strcmp(city[mid], target) > 0){
            top= mid - 1;
        }
        if (strcmp(city[mid], target) < 0){
            bottom= mid + 1;
        }
    }
    return found;
}

int binary_search_city(int arr[], int l, int r, int x) {

    int found = -1;
    
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == x){
            FILE *cityandzip = fopen("CityAndZip.txt", "r");

            fseek(cityandzip,zipIndex[mid],SEEK_CUR);
            char cityname[100];
            fscanf(cityandzip,"%s %*d", cityname);
            printf("%d the city is %s.\n", x, cityname);
            found = 1;
            return found; 
        }
  
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid] > x) 
            return binary_search_city(arr, l, mid - 1, x); 
  
        // Else the element can only be present 
        // in right subarray 
        return binary_search_city(arr, mid + 1, r, x); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return found; 
}