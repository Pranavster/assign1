#include "storage_mgr.h" //has references to macros 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initStorageManager(void) {
    // Initialization code, if needed
    /*SM_FileHandle *fHandle;     //declare fHandle
    fHandle->fileName = " ";
    fHandle->curPagePos = 0;
    fHandle->totalNumPages = 0;//get the total number of pages
    fHandle->mgmtInfo = NULL;
    printf("\npingpong\n");*/
}

RC createPageFile(char *fileName)
{
    char *ptr=malloc(PAGE_SIZE);//manually allocate size for a char pointer
    memset(ptr, '\0', PAGE_SIZE);//the file will be empty
    FILE *file = fopen(fileName,"w+");//open file with writing permissions, or return that it is not found
    if (file == NULL) 
        return RC_WRITE_FAILED;
    char *emptyPage = (char *)calloc(PAGE_SIZE, sizeof(char));//create and fill the empty page with the contents of the page file
    if(emptyPage == NULL)
        return RC_FILE_NOT_FOUND;
    if( fwrite(emptyPage, sizeof(char), PAGE_SIZE, file)<PAGE_SIZE)
    {
        
    }
    free(emptyPage);//free memory of emptyPage
    //fHandle->totalNumPages+=1;//notify the total number of pages has increased
    fclose(file);
    free(ptr);
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
    printf("\nFile size:%d\n",totalFileSize);
    fHandle->totalNumPages = totalFileSize / PAGE_SIZE;//get the total number of pages
    fHandle->mgmtInfo = file;
    return RC_OK;
}
RC closePageFile(SM_FileHandle *fHandle)
{
    if(fHandle->mgmtInfo == NULL)//if the file is non existent or closing it did not work, return RC_FILE_HANDLE_NOT_INIT or RC_FILE_NOT_CLOSED, respectively
    {
        printf("Null management Info");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if(fclose((FILE *)(fHandle->mgmtInfo))!=0)//closes the file
    {
        printf("Null management Info");
        return RC_FILE_NOT_CLOSED;
    }
    fHandle->mgmtInfo = NULL;
    return RC_OK;

}
RC destroyPageFile(char *fileName)
{
    SM_FileHandle fHandle;
    RC result = openPageFile(fileName, &fHandle);//check if the file can be opened
    if(result != RC_OK)
    {
        return result;
    }
    if(closePageFile(&fHandle) != RC_OK)//close the page and return RC_FILE_NOT_CLOSED if there was a failure
    {
        return RC_FILE_NOT_CLOSED;
    }
    if(remove(fileName)!=0)//remove the page and return RC_FILE_NOT_FOUND if there was a failure
    {
        printf("\nc\n");
        return RC_FILE_NOT_FOUND;
    }
    return RC_OK;//file successfully deleted
}

RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage )
{
    //The method reads the block at position pageNum from a file and stores its content in the memory pointed to by the memPage page handle.
    //If the file has less than pageNum pages, the method should return RC READ NON EXISTING PAGE
    if(fHandle->totalNumPages<pageNum)
        return RC_READ_NON_EXISTING_PAGE;
    
}

extern int getBlockPos(SM_FileHandle *fHandle )//return the position of the page
{
    return fHandle->curPagePos;
}
int main() 
{
    SM_FileHandle fHandle1,fHandle2;

    RC crpf1_result = createPageFile("gerald.txt");//Testing creation of file
    RC crpf2_result = createPageFile("sharandeep.txt");//Testing creation of file
    RC opf1_result = openPageFile("gerald.txt",&fHandle1);
    RC opf2_result = openPageFile("sharandeep.txt",&fHandle2);
    if(opf1_result==RC_OK)
    {
        printf("File successfully opened");
        printf("Filename %s\n",fHandle1.fileName);
        printf("Current page position %d\n",fHandle1.curPagePos);
        printf("Total amount of pages %d\n",fHandle1.totalNumPages);
    }
    else
    {
        printf("File unsuccessfully opened, result is %d\n",opf1_result);
    }
    if(opf2_result==RC_OK)
    {
        printf("File successfully opened");
        printf("Filename %s\n",fHandle2.fileName);
        printf("Current page position %d\n",fHandle2.curPagePos);
        printf("Total amount of pages %d\n",fHandle2.totalNumPages);
    }
    else
    {
        printf("File unsuccessfully opened, result is %d\n",opf2_result);
    }
    printf("%d",getBlockPos(&fHandle2));
}
