
//Creation of a new file (fopen with attributes as “a” or “a+” or “w” or “w+”).

//example 1
// C program to Create a file 
#include <stdio.h> 
#include <stdlib.h> 

// Driver code 
int main() 
{ 
	// File Pointer declared 
	FILE* ptr; 

	// File opened 
	ptr = fopen("./Hello.txt", "w"); 

	// Failed Condition 
	if (ptr == NULL) { 
		printf("Error Occurred While creating a "
			"file !"); 
		exit(1); 
	} 

	// File closed 
	fclose(ptr); 

	// Data is finally Inserted 
	printf("File created\n\n"); 

	return 0; 
} 



//example 2
//Writing to a text file in C - fprintf() and fscanf()
// C program to write contents 
// to the file 
#include <stdio.h> 
#include <stdlib.h> 

// Driver code 
int main() 
{ 
	// File Pointer declared 
	FILE* ptr; 

	// File opened 
	ptr = fopen("./Hello.txt", "w+"); 

	// Failed Condition 
	if (ptr == NULL) { 
		printf("Error Occurred While writing to a text "
			"file !"); 
		exit(1); 
	} 

	// Data to be inserted 
	char str[] = "This is all the Data to be inserted in "
				"File by GFG."; 

	// Puts data inside the file 
	fputs(str, ptr); 
	
	// File closed 
	fclose(ptr); 

	// Data is finally Inserted 
	printf("Data Written Inside the file\n\n"); 

	return 0; 
}

//example 3
//Writing to a Binary File fwrite() 
// C program to write to a 
// binary file 
#include <stdio.h> 
#include <stdlib.h> 

// Struct declared 
struct Num { 
	int n1, n2; 
}; 

// Driver code 
int main() 
{ 
	// variables declared 
	int n; 
	struct Num obj; 
	
	// File Pointers declared 
	FILE* fptr; 
	
	// Failure Condition 
	if ((fptr = fopen("temp.bin", "wb")) == NULL) { 
		printf("Error! opening file"); 

		// if file pointer returns NULL program 
		// will exit 
		exit(1); 
	} 

		
	for (n = 1; n < 10; n++) { 
		obj.n1 = n; 
		obj.n2 = 12 + n; 
		
	// Data written 
		fwrite(&obj, sizeof(struct Num), 1, fptr); 
	} 

	// File closed 
	fclose(fptr); 

	printf("Data in written in Binary File\n\n"); 

	return 0; 
}

//example 4
//Reading text files in C
// C program to read contents 
// from the file 

/*
// fgets() reads up to n-1 characters from the input stream referenced by fp Stopsafteranewlinecharacterisread
If this function encounters a newline character '\n' or the end of the file EOF before they 
have read the maximum number of characters, then it returns only the characters read up to 
that point including the new line character. You can also use int fscanf(FILE *fp, const char 
*format, ...) function to read strings from a file, but it stops reading after encountering 
the first space character.
*/
#include <stdio.h> 
#include <stdlib.h> 

// Driver code 
int main() 
{ 
char str[80]; 
FILE* ptr; 

ptr = fopen("Hello.txt", "r"); 

if (ptr == NULL) 
{ 
	printf("Error While opening file"); 
		
	// if the pointer returns NULL 
	// program will exit 
	exit(1); 
} 
	
if(fgets(str, 80, ptr) != NULL) 
{ 
	puts(str); 
} 
fclose(ptr); 

return 0; 
}


//example 5
//fread()
// C Program to Read from a 
// binary file using fread() 
#include <stdio.h> 
#include <stdlib.h> 
struct Num 
{ 
	int n1, n2; 
}; 

// Driver code 
int main() 
{ 
int n; 
struct Num obj; 
FILE* fptr; 
if ((fptr = fopen("temp.bin", "rb")) == NULL) 
{ 
	printf("Error! opening file"); 
		
	// If file pointer will return NULL 
	// Program will exit. 
	exit(1); 
} 
	
// else it will return a pointer 
// to the file. 
for (n = 1; n < 10; ++n) 
{ 
	fread(&obj, sizeof(struct Num), 1, fptr); 
	printf("n1: %d\tn2: %d\n", obj.n1, obj.n2); 
} 
	
fclose(fptr); 
return 0; 
}

//example 6
//fseek()
// C program to implement 
// fseek 
#include <stdio.h> 
#include <stdlib.h> 

// Driver code 
int main() 
{ 
	// string declared 
	char str[80]; 
	
	// File Pointer declared 
	FILE* ptr; 

	// File Opened 
	ptr = fopen("Hello.txt", "w+"); 
	
	// Puts data in File 
	fputs("Welcome to GeeksforGeeks", ptr); 

	// fseek function used 
	fseek(ptr, 11, SEEK_SET); 
	
	// puts programming in place of at position defined 
	// After 11 elements 
	fputs("Programming ", ptr); 
	fclose(ptr); 

	// Reading the file 
	ptr = fopen("Hello.txt", "r+"); 
	if (fgets(str, 80, ptr) != NULL) { 
		puts(str); 
	} 

	// Close the opened file 
	fclose(ptr); 
	
	return 0; 
}
