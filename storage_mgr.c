#include "storage_mgr.h" //has references to macros 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initStorageManager(void) {
    // Initialization code, if needed
}

RC createPageFile(char *fileName)
{
    SM_FileHandle *fHandle;     //declare fHandle
    char *ptr=malloc(PAGE_SIZE);//manually allocate size for a char pointer
    memset(ptr, '\0', PAGE_SIZE);//the file will be empty
    FILE *file = fopen(fileName,"w+");//open file with writing permissions, or return that it is not found
    if (file == NULL) 
        return RC_FILE_NOT_FOUND;
    char *emptyPage = (char *)calloc(PAGE_SIZE, sizeof(char));//create and fill the empty page with the contents of the page file
    fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);
    free(emptyPage);//free memory of emptyPage
    fHandle->totalNumPages+=1;//notify the total number of pages has increased
    fclose(file);
    return RC_OK;//this means the function worked
}
RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
    FILE *file = fopen(fileName,"r+");//open the file with reading permissions, or the file was not found
    if (file == NULL) 
        return RC_FILE_NOT_FOUND;
    fHandle->fileName = fileName;//assign attributes of the fHandle struct
    fHandle->curPagePos = 0;
    fseek(file, 0, SEEK_END);//move the file pointer to the end of the file
    long int totalFileSize = ftell(file);//get the total size of the file
    fHandle->totalNumPages = totalFileSize / PAGE_SIZE;//get the total number of pages
    fHandle->mgmtInfo = file;
    return RC_OK;
}
RC closePageFile(SM_FileHandle *fHandle)
{
    if(fHandle->mgmtInfo == NULL)//if the file is non existent or closing it did not work, return RC_FILE_HANDLE_NOT_INIT or RC_FILE_NOT_CLOSED, respectively
    {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if(fclose((FILE *)(fHandle->mgmtInfo))!=0)
    {
        return RC_FILE_NOT_CLOSED;
    }
    fHandle->mgmtInfo = NULL;//set the closePageFile to 0
    return RC_OK;

}
RC destroyPageFile(char *fileName)
{
    SM_FileHandle *fHandle;//declare fHandle and remove 1 page if possible
    if(fHandle->totalNumPages>0)
    {
        fHandle->totalNumPages-=1;
    }
    else//edge case for when this method is called if there are no pages
    {
        printf("Can't remove a page that does not exist");
        exit(1);
    }
    if (remove(fileName) != 0) //if the file is not successfully removed, return appropriate macro, otherwise, free the file and return RC_OK
    {
        return RC_FILE_NOT_FOUND;
    }
    free(fileName);
    return RC_OK;
}

int main() 
{
    SM_FileHandle fHandle;
    RC crpf_result = createPageFile("example.txt");/*Testing creation of file*/
    if(crpf_result==RC_OK)
    {
        printf("File successfully created");
    }
    else
    {
        printf("File unsuccessfully created, result is ",crpf_result);
    }
    
    RC opf_result = openPageFile("example.txt", &fHandle);/*Testing opening of file*/
    if(opf_result==RC_OK)
    {
        printf("File successfully opened");
        printf("Filename %s\n",fHandle.fileName);
        printf("Current page position %d\n",fHandle.curPagePos);
        printf("Total amount of pages %d\n",fHandle.totalNumPages);
    }
    else
    {
        printf("File unsuccessfully created, result is ",opf_result);
    }
    RC cpf_result = closePageFile(&fHandle);/*Testing closing of file*/
    if(cpf_result==RC_OK)
    {
        printf("File successfully closed");
    }
    else
    {
        printf("File unsuccessfully created, result is ",cpf_result);
    }
    RC dpf_result = destroyPageFile("example.txt");/*Testing destruction of file*/
    if(dpf_result==RC_OK)
    {
        printf("File successfully closed");
    }
    else
    {
        printf("File unsuccessfully created, result is ",dpf_result);
    }
}