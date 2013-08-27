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
	int* arraySize = malloc(sizeof(int));
	int* fileVals = checkDirs(dirA, arraySize);
	int i;
	FILE *fp;
	fp = fopen("layout.txt", "w");
	for (i = 0; i < *arraySize; i++)
		fprintf(fp,"%d\n",fileVals[i]);
	free(arraySize);
	free(fileVals);
	return 0;
}

//This method is the meat of the program, taking in two paths and searching their directories.
//This is the first time I can actually feel really thankful for recursion.
int* checkDirs(char* apath, int* arraySize){
	DIR *a;
        struct dirent *adent; 
	struct stat *dirA;
	dirA = malloc(sizeof(stat));
	char **Anames;
	char currentFile[128];//get the job done for small files and I wasn't given a size limit.
	Anames = malloc(sizeof(char*));
	strcat(apath,"/");
	char tempA[256];
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
		strcpy(tempA,apath);
		strcat(tempA,Anames[i]);
        }
	free(dirA);
	free(Anames);
	closedir(a);
	return finalVals;
}

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
