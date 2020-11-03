#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

typedef struct Fullname
{
	char First[10],Last[10];
}FullName;

typedef struct Employee
{
	FullName Name;
	int Emp_Id;
	float Salary;
	char Post[15];
}Employee;

//Check Duplicate data
int Duplicate(char *File_Name,int Id)
{
	int fd;
	Employee Emp_Obj;
	fd=open(File_Name,O_RDONLY); 
	if(fd==-1) 	printf("\nFile Opening Error");
	else
	{	
     		while(read(fd,&Emp_Obj,sizeof(Employee))!=0)
     		{  
     			if(Id==Emp_Obj.Emp_Id)
	       		{ 
	  	     		close(fd); 
				return 1;
			}
	       	}
    		close(fd);
	}
	return 0;
} 
//Create New File
int Create(char *File_Name)
{
	//FILE *Ptr;
	int fd;
	char Answer;
	printf("\nEnter File Name: ");
	scanf("%s",File_Name);
	fd=open(File_Name,O_RDONLY);
	if(fd!=-1)
	{
		printf("\nFile already exist!!");
		printf("\nDo u want to remove this File (y/n):\n");
		scanf(" %c",&Answer);
	}
	if(Answer=='y' || Answer=='Y' || fd==-1)
	{
		fd=open(File_Name, O_RDONLY | O_CREAT, S_IRWXU);
		if(fd==-1)
		{
			printf("File creation Error...");
			return 1;
		}
		else 
			printf("\nFile Created Successfully: %d",fd);
	}	
	close(fd);
	return 0;
} 	
//Insert or Append data in existing File
void Insert(char *File_Name)
{
	int Num,i,j,Id,Flag,Ch, fd;
	float Sal;
	Employee Emp_Obj;
	fd=open(File_Name,O_WRONLY | O_APPEND);
	 if(fd!=-1)
	 {
		printf("\nEnter Employee data:");
       		do
       		{
       			printf("\nFirst Name: "); 
			scanf("%s",Emp_Obj.Name.First);
       			j=0;
       			Flag=0;
       			while(Emp_Obj.Name.First[j]!='\0')
       			{
				if((Emp_Obj.Name.First[j]>='a' &&Emp_Obj.Name.First[j]<='z')||(Emp_Obj.Name.First[j]>='A'&&Emp_Obj.Name.First[j]<='Z'))
 					j++;
				else 
				{
       					Flag=1;
       					break;
       				}
       			}
      			if(Flag==1)
       				printf("\nWrong Input!! Re-enter First name:\n");
       		}while(Flag!=0);
      		printf("\nLast Name: "); 
		scanf("%s",Emp_Obj.Name.Last);
       		while(1)  //Check for Existance/Nagative  value Employee Id
       		{
  			printf("\nEmployee Id:");
			scanf("%d",&Id);
			if(Id > 0 && !Duplicate(File_Name,Id) )
  			{
  				Emp_Obj.Emp_Id=Id;
	  			break;
	  		} 	 		printf("\nEmployee Id already present/Negative Id re-enter:\n"); 		}        		while(1) //Check for Negative Salary
      		{ 	 		printf("\nSalary: ");
	 		scanf("%f",&Sal);
 			if(Sal>0) 
	 		{
	 			Emp_Obj.Salary=Sal;
				break;
	 		}
			printf("\nNegative Salary!! Reenter Salary ");
       		}
       		printf("\nPost: ");
		scanf("%s",Emp_Obj.Post);
        		write(fd,&Emp_Obj,sizeof(Employee));
		close(fd);
	}
	else
       		printf("File Opening Error...");
} 
//Search & print details of given Employee Id
int Retrive(char *File_Name)
{ 	int Id,Flag=0,i=0, fd;
	Employee Emp_Obj;
	fd=open(File_Name,O_RDONLY);
	if(fd==-1)	
		printf("\nFile Opening Error!!");
	else
	{
       		printf("\nEnter Employee Id which is to be Retrived/Modified/Delete:\n");
       		scanf("%d",&Id);
       		while(read(fd,&Emp_Obj,sizeof(Employee))!=0)
       		{
       			if(Id==Emp_Obj.Emp_Id)
       			{
       				printf("\n------------------------------------------------------------------------");
       				printf("\n\tName\t\tEmployee Id\t   Salary\t  Post");
       				printf("\n------------------------------------------------------------------------");
       				printf("\n%s",Emp_Obj.Name.First);
       				printf(" %s",Emp_Obj.Name.Last);
       				printf("\t%d",Emp_Obj.Emp_Id);
       				printf("\t\t%5.2f",Emp_Obj.Salary);
       				printf("\t%s",Emp_Obj.Post);
       				Flag=1;
       				break;
       			}        			i++;
       		}
       		if(Flag==0) 			i=-1;
    		close(fd);
   	}
	return i;
} 
//Modify salary & post of given employee(Using Temp File)
void Modify1(char *File_Name)
{
	//FILE *Ptr,*Temp;
	Employee Emp_Obj;
	int i=0,Pos, fd, fd1;
	fd=open(File_Name,O_RDONLY);
	fd1=open("Temp",O_WRONLY | O_CREAT, S_IRWXU);
	if(fd==-1)
		printf("\nFile Opening Error");
	else
	{
		Pos=Retrive(File_Name);
	       	if(Pos!=-1)
	       	{
		       	while(read(fd,&Emp_Obj,sizeof(Employee)))
		       	{
		       		if(Pos==i)
		       		{
					printf("\nEnter Modified Employee data:"); 					printf("Salary: ");
					scanf("%f",&Emp_Obj.Salary);
	       				printf("Post: ");
					scanf("%s",Emp_Obj.Post);
	       			}
	       			write(fd1,&Emp_Obj,sizeof(Employee));
	       			i++;
	       		}
	       		close(fd);
	       		close(fd1);
	       		remove(File_Name);
	       		rename("Temp",File_Name);
		}
		else
			printf("Record Not present");
	}
}

//Remove Given Employee data(Using Temp File)
void Delete(char *File_Name)
{
	//FILE *Ptr,*Temp;
	Employee Emp_Obj;
	int Pos,i=0, fd, fd1;
	fd=open(File_Name,O_RDONLY);
	fd1=open("Temp",O_WRONLY | O_CREAT,S_IRWXU);
	if(fd==-1)
		printf("\nFile Opening Error");
	else
	{
		Pos=Retrive(File_Name);
	       	if(Pos!=-1)
	       	{
	       		while(read(fd,&Emp_Obj,sizeof(Employee)))
	       		{
	       			if(Pos!=i)
		 		write(fd1,&Emp_Obj,sizeof(Employee));
	       			i++;
	       		}
	       		close(fd);
	       		close(fd1);
	       		remove(File_Name);
	       		rename("Temp",File_Name);
	       	}
	       	else printf("Record Not present");
	} 
} 
//print contents of the given File
void Display(char *File_Name)
{
	//FILE *Ptr;
	int fd;
	Employee Emp_Obj;
	fd=open(File_Name,O_RDONLY);
	if(fd==-1)
		printf("\nFile Opening Error");
	else
	{
		printf("\n------------------------------------------------------------------------");
	       	printf("\n\tName\t\t\t   Id\t  Salary\t   Post");
	       	printf("\n------------------------------------------------------------------------");
	       	while(read(fd,&Emp_Obj,sizeof(Employee)))
	       	{
	       		printf("\n%10s",Emp_Obj.Name.First);
	       		printf("%10s |",Emp_Obj.Name.Last);
	       		printf("\t%9d |",Emp_Obj.Emp_Id);
	       		printf("\t%9f |",Emp_Obj.Salary);
	       		printf("%9s",Emp_Obj.Post);
	       	}
	    	close(fd);
	}
} 	
int main()
{
	char File_Name[10],Ans;
	int Choice, flag;
	printf("\n\t\t\tFile Creation");
	flag=Create(File_Name);
	if(flag==0)
	{
		do
		{
			printf("\nChoices:\n1: Insert\n2: Display\n3: Retrive\n4: Modify\n5: Delete\n6: Exit\nEnter Your Choice: ");
			scanf("%d",&Choice);
			switch(Choice)
			{
				case 1: printf("\n\t\t\tInsert Operation");
				Insert(File_Name);
				Display(File_Name);
				break;
				
				case 2: printf("\n\t\t\tDisplay Operation");
				Display(File_Name);
				break; 	
				case 3: printf("\n\t\t\tSearch Operation");
				if(Retrive(File_Name)==-1)
				printf("\nRecord Not Present");
				break;
	
				case 4: printf("\n\t\t\tModify Operation");
				Modify1(File_Name);
				Display(File_Name);
				break;
				
				case 5: printf("\n\t\t\tDelete Operation");
				Delete(File_Name);
				Display(File_Name);
				break;
			
				case 6: exit(0);
	
				default: printf("\nInvalid Choice\n");
			}
			printf("\n\nDo u want to Continue :");
			scanf(" %c",&Ans);
		}while(Ans=='y' || Ans=='Y');
	}
	return 0;
}
