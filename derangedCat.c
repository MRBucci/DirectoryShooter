//Put this program in a Linux directory, and it will take all of the files
//and convert them into readable data for the windows program to create a level with.
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int* checkDirs(char* apath, int* arraySize);

int wordSort(char* fileNames);

int main(int argc, char *argv[])
{
	char dirA[] = "./";
	strcat(dirA,".");
	int* arraySize = malloc(sizeof(int));//The arraySize variable is initialized solely to circumvent
	int* fileVals = checkDirs(dirA, arraySize);//the issue that the program can't normally determine
	int i;                                //the size of an array at runtime if it's dynamically allocated.
	FILE *fp;
	fp = fopen("layout.txt", "w");  //The integer array is fed into this file.
	for (i = 0; i < *arraySize; i++)//arraySize's only use is here.
		fprintf(fp,"%d\n",fileVals[i]);
	free(arraySize);
	fclose(fp);
	free(fileVals);
	return 0;
}


int* checkDirs(char* apath, int* arraySize){
	DIR *a;
        struct dirent *adent; 
	struct stat *dirA;
	dirA = malloc(sizeof(stat));
	char **Anames;
	char currentFile[128];//128 is more than enough characters for a file title.
	Anames = malloc(sizeof(char*));
	strcat(apath,"/");
	char tempA[256];  //used in the file scanning process.
	int aItems = 0;
        char* aString;
        if (!(a = opendir(apath))){				//check if both directories can open first.
                error("Failed to open directory a!");
		exit(1);
	}
	while(0 != (adent = readdir(a))){ //Read through A's files and store their names.
		strcpy(currentFile,adent->d_name);
                if (((strcmp(currentFile,".")) != 0) &&
                         ((strcmp(currentFile,"..")) != 0)){
			strcpy(tempA,apath);
			strcat(tempA,adent->d_name);
			aItems++;					//These items variables are used later to loop through the files.
			Anames = realloc(Anames, aItems * sizeof(char*));//reallocing to keep array size to a minimum.
			Anames[aItems-1] = malloc((strlen(currentFile)+1) * sizeof(char));
			strcpy(Anames[aItems-1],currentFile);
			dirA = realloc(dirA,aItems * sizeof(struct stat));
			if(stat(tempA,&dirA[aItems-1]) < 0)
               		{
               		         perror("");
               		         break;
         	        }
		}
        }
	int i;
	*arraySize = aItems;
	int* finalVals = malloc(((int)aItems) * sizeof(int));
	//Search through A!
	for (i = 0; i < aItems; i++){
		finalVals[i] = wordSort(Anames[i]);		//Set the final int based on the file extension!
        }
	free(dirA);
	free(Anames);
	closedir(a);
	return finalVals;
}

//This method takes in a string, uses strstr to try to find a file extension,
//and returns an integer based on the existence of such an extension.
int wordSort(char* fileNames){
	int i;
	char* per; 
	per = strstr(fileNames,".");	
	if (per != NULL){
		if ((strcmp(per, ".doc") == 0) || (strcmp(per, ".txt") == 0))
			return 1;
		else if((strcmp(per, ".png") == 0) || (strcmp(per, ".jpg") == 0) || (strcmp(per, ".jpeg") == 0))
			return 2;
		else if((strcmp(per, ".mp3") == 0) || (strcmp(per, ".wav") == 0))
			return 3;
		}
		return 4;
}
