//The Equalizer, by Andrew Scherch, for CS 283 Lab 3
//This program essentially functions by recursively running
//the checkDirs method which searches and "equalizes" directory
//B with directory A, as input by the user. Whatever is in A but not
//in B is copied over, while whatever is in B but not A is deleted.
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <cstring>

char*** checkDirs(char* apath);

int multiHash(char* aWord);

int **wordSort(char** fileNames);

int main(int argc, char *argv[])
{
	char dirA[] = ".\\";
	char*** files;
	int** fileVals;
	strcat(dirA,argv[1]);
	files = checkDirs(dirA);
	fileVals = wordSort(files[1]);
	printf("%s %d \n", files[1][0],*fileVals[0]);
	int i,j;
	//for(i = 0; i < sizeof(files); i++){
	//	for(j = 0; j < sizeof(files[i]); j++){
	//		free(files[i][j]);
	//	}
	//	free(files[i]);
	//}
	for(i = 0; i < sizeof(fileVals); i++)
		free(fileVals[i]);
}

//This method is the meat of the program, taking in two paths and searching their directories.
//This is the first time I can actually feel really thankful for recursion.
char*** checkDirs(char* apath){
	DIR *a;
        struct dirent *adent; 
	struct stat *dirA;
	dirA = malloc(sizeof(stat));
	char **Anames;
	char ***returnFiles = malloc(2 * sizeof(char**));
	char buffer[1024]; //This isn't a very large buffer, but it should
	char currentFile[128];//get the job done for small files and I wasn't given a size limit.
	Anames = malloc(sizeof(char*));
	strcat(apath,"\\");
	char tempA[256];
	int aItems = 0;
        char* aString;
        if (!(a = opendir(apath))){				//check if both directories can open first.
                //error("Failed to open directory a!");
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
               		         //perror("");
               		         break;
         	        }
		}
        }
	int i,dirCount,fileCount,nbytes,found,readFile,writeFile;
	returnFiles[0] = malloc(sizeof(aItems) * sizeof(char*));
	returnFiles[1] = malloc(sizeof(aItems) * sizeof(char*));
	dirCount = 0;
	fileCount = 0;
	//First, we search through A to find things that aren't in B to copy over, including directories.
	for (i = 0; i < aItems; i++){
		found = 0;				//found will be used with multiple checks of the directories.
		strcpy(tempA,apath);
		strcat(tempA,Anames[i]);			//Return temps to their original paths, adding the current file to A.
		if (S_ISDIR(dirA[i].st_mode) != 0){			//First, check if the current file is a directory.
			returnFiles[0][dirCount] = malloc(sizeof(Anames[i])*sizeof(char));
			strcpy(returnFiles[0][dirCount],Anames[i]);
			dirCount = dirCount + 1;	
			}
		else{
			returnFiles[1][fileCount] = malloc(sizeof(Anames[i])*sizeof(char));
                        strcpy(returnFiles[1][fileCount],Anames[i]);
                        fileCount = fileCount + 1;
		}
        }
	free(dirA);
	free(Anames);
	closedir(a);
	return returnFiles;
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

int **wordSort(char** fileNames){
	int i;
	char* per; 
	int **wordValues = malloc(sizeof(fileNames) * sizeof(int*));
	for (i = 0; i < sizeof(fileNames); i++){
		wordValues[i] = (int*)malloc(sizeof(int));
                
			per = strstr(fileNames[i],".");	
			if (per != NULL){
			if (strcmp(per, ".bat"))
					*wordValues[i] = 30;
			else if(strcmp(per, ".exe"))
					*wordValues[i] = 40;
			else if(strcmp(per, ".py"))
					*wordValues[i] = 50;
			else if(strcmp(per, ".cpp"))
					*wordValues[i] = 60;
			else if(strcmp(per, ".c"))
					*wordValues[i] = 70;
			}
			else
					*wordValues[i] = multiHash(fileNames[i]);
					
		}
	return wordValues;
}
