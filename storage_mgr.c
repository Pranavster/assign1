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
    if(fHandle->mgmtInfo == NULL)//if the file is non existent or closing it did not work, return RC_FILE_HANDLE_NOT_INIT or RC_FILE_NOT_FOUND, respectively
    {
        printf("Null management Info");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if(fclose((FILE *)(fHandle->mgmtInfo))!=0)//closes the file
    {
        printf("Null management Info");
        return RC_FILE_NOT_FOUND;
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
    if(closePageFile(&fHandle) != RC_OK)//close the page and return RC_FILE_NOT_FOUND if there was a failure
    {
        return RC_FILE_NOT_FOUND;
    }
    if(remove(fileName)!=0)//remove the page and return RC_FILE_NOT_FOUND if there was a failure
    {
        printf("\nc\n");
        return RC_FILE_NOT_FOUND;
    }
    return RC_OK;//file successfully deleted
}

RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) 
{
    if (pageNum >= fHandle->totalNumPages || pageNum < 0) 
    {
        return RC_READ_NON_EXISTING_PAGE; // Return error if the requested page doesn't exist
    }

    FILE *file = (FILE *)fHandle->mgmtInfo; // Use the mgmtInfo as a file pointer

    // Calculate the offset (position in the file) to the start of the page
    int offset = pageNum * PAGE_SIZE;

    // Move the file pointer to the correct position (beginning of the requested page)
    if (fseek(file, offset, SEEK_SET) != 0) 
    {
        return RC_FILE_SEEK_ERROR; // Return an error if the seek fails
    }

    // Read the page into the memory buffer
    size_t bytesRead = fread(memPage, sizeof(char), PAGE_SIZE, file);
    if (bytesRead < PAGE_SIZE) 
    {
        return RC_READ_NON_EXISTING_PAGE; // Return error if the full page wasn't read
    }

    fHandle->curPagePos = pageNum; // Update the current page position
    return RC_OK; // Return success
}

RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) 
{
    // Check if the file handle is initialized
    if (fHandle == NULL || fHandle->mgmtInfo == NULL) 
    {
        return RC_FILE_HANDLE_NOT_INIT; // Return error if the file handle is not initialized
    }

    // Attempt to read the first block (pageNum = 0)
    return readBlock(0, fHandle, memPage); // Use the existing readBlock function to read the first page
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

extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle->totalNumPages == 0)   //Verify that the file handle has 0 pages in total
    {
       return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}
int isPageNumberValid(int pageNum, SM_FileHandle *fHandle)   //given page number is valid within the file handle's total number of pages
{
    return((pageNum >= fHandle->totalNumPages || pageNum < 0) ? 0 : 1);   //Return 0 if invalid, 1 if valid
}
extern RC WriteBlock(int PageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (!isPageNumberValid(PageNum, fHandle))   //verify the page number
    {
       return RC_WRITE_FAILED;   //if the page is incorrect return the error code
    }
    FILE *fgroup8 = fopen(fHandle->fileName, "r+");   //use the read/write mode to open the file
    if(fgroup8 == NULL)   //verify that the file was successfully opened
    {
       return RC_FILE_NOT_FOUND;
    }

    char *bufferPtr = memPage;   //a pointer to the start of the memory page
    for (int index = 0; index < PAGE_SIZE; index++)   //loop through each byte in the page
    {
       if (fputc(*bufferPtr, fgroup8) == EOF)
       {
          fclose(fgroup8);  //close the file if writing fails
          return RC_WRITE_FAILED;
       }
       bufferPtr++;
    }
    fHandle->curPagePos = ftell(fgroup8);
    fclose(fgroup8);

    return RC_OK;
}


extern RC WriteCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int currentPageNumber = fHandle->curPagePos / PAGE_SIZE;   //Determine the current page's index by using the file position as a guide

    char *bufferPtr = memPage;   //Set bufferPtr to point to the start of memPage
    FILE *fgroup8 = fopen(fHandle->fileName, "r+");   //use the read/write mode to open the file

    for (int index = 0; index < PAGE_SIZE; index++)
    {
       if (fputc(*bufferPtr, fgroup8) == EOF)
       {
          return RC_WRITE_FAILED;   //Return an error code if the write operation fails
       }
       bufferPtr++;
    }
    fHandle->curPagePos += PAGE_SIZE;   //Move the file position indication by one page's worth
    fHandle->totalNumPages++;
    return RC_OK;
}


extern RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    SM_PageHandle emptyBlock = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));   //Set memory to zero and allocate space for a data page
    char *blockPtr = emptyBlock;   //Assign blockPtr to the start of the recently allocated memory block
    FILE *fgroup8 = fopen(fHandle->fileName, "r+");   //use the read/write mode to open the file
    for (int index = 0; index < PAGE_SIZE; index++)
    {
       if (fputc(0, fgroup8) == EOF)   //In order to detect write failures, write a zero byte to the file
       {
          free(emptyBlock);
          return RC_WRITE_FAILED;   //In order to signal the write failure, return an error code
       }
       blockPtr++;
    }
    fHandle->totalNumPages++;   // Increase the file handle's total amount of pages
    free(emptyBlock);

    return RC_OK;
}

extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    if (numberOfPages > fHandle->totalNumPages)
    {
        FILE *fgroup8 = fopen(fHandle->fileName, "r+");
        if (fgroup8 == NULL)
        {
            return RC_FILE_NOT_FOUND; 
        }
        while (fHandle->totalNumPages < numberOfPages)
        {
            RC result = appendEmptyBlock(fHandle);
            if (result != RC_OK)
            {
                fclose(fgroup8); 
                return result; 
            }
        }

        fclose(fgroup8);
    }

    return RC_OK; 
}
