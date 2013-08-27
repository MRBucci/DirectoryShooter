//The Equalizer, by Andrew Scherch, for CS 283 Lab 3
//This program essentially functions by recursively running
//the checkDirs method which searches and "equalizes" directory
//B with directory A, as input by the user. Whatever is in A but not
//in B is copied over, while whatever is in B but not A is deleted.
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int* checkDirs(char* apath, int* arraySize);

int multiHash(char* aWord);

int wordSort(char* fileNames);

int main(int argc, char *argv[])
{
	char dirA[] = ".\\";
	strcat(dirA,argv[1]);
	int* arraySize = malloc(sizeof(int));
	int* fileVals = checkDirs(dirA, arraySize);
	int i;
	for (i = 0; i < *arraySize; i++)
		printf("%d\n",fileVals[i]);
	free(arraySize);
	free(fileVals);
}

//This method is the meat of the program, taking in two paths and searching their directories.
//This is the first time I can actually feel really thankful for recursion.
int* checkDirs(char* apath, int* arraySize){
	DIR *a;
        struct dirent *adent; 
	struct stat *dirA;
	dirA = malloc(sizeof(stat));
	char **Anames;
	char buffer[1024]; //This isn't a very large buffer, but it should
	char currentFile[128];//get the job done for small files and I wasn't given a size limit.
	Anames = malloc(sizeof(char*));
	strcat(apath,"\\");
	char tempA[256];
	int aItems = 0;
        char* aString;
        if (!(a = opendir(apath))){				//check if both directories can open first.
                error("Failed to open directory a!");
		exit(1);
	}
	while(0 != (adent = readdir(a))){ //Read through A's files and store their names and stats.
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
	int i,dirCount,fileCount,nbytes,found,readFile,writeFile;
	dirCount = 0;
	*arraySize = aItems;
	int* finalVals = malloc(((int)aItems) * sizeof(int));
	fileCount = 0;
	printf("aItems is %d", aItems);
	//First, we search through A to find things that aren't in B to copy over, including directories.
	for (i = 0; i < aItems; i++){
		finalVals[i] = wordSort(Anames[i]);				//found will be used with multiple checks of the directories.
		strcpy(tempA,apath);
		strcat(tempA,Anames[i]);
		/*if (S_ISDIR(dirA[i].st_mode) != 0){			//First, check if the current file is a directory.
			returnFiles[0] = realloc(returnFiles[0],(dirCount+1) * sizeof(char*));
			returnFiles[0][dirCount] = malloc((strlen(Anames[i])+1)*sizeof(char));
			strcpy(returnFiles[0][dirCount],Anames[i]);
			printf("%s",returnFiles[0][dirCount]);
			dirCount = dirCount + 1;	
			printf("Dircount %d %d \n",dirCount,sizeof(returnFiles[0]));
			}
		else{
			returnFiles[1] = realloc(returnFiles[1],(fileCount+1) * sizeof(char*));
			returnFiles[1][fileCount] = malloc((strlen(Anames[i])+1)*sizeof(char));
                        strcpy(returnFiles[1][fileCount],Anames[i]);
                        fileCount = fileCount + 1;
			printf("Filecount %d %d \n",fileCount,sizeof(returnFiles[1]));
		}*/
        }
	free(dirA);
	free(Anames);
	closedir(a);
	return finalVals;
}


int multiHash(char* aWord)
{
        int i;
	int value = 0;
	for (i = 0; i < strlen(aWord); i++){
		value = value + aWord[i];
	}
	return value % 26;
}

int wordSort(char* fileNames){
	int i;
	char* per; 
	per = strstr(fileNames,".");	
	printf("per is %s\n", per);
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
