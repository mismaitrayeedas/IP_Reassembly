#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <netinet/in.h>
#include<arpa/inet.h>
#define size 100

/* Structures to store the Datagram and their respective information */
struct datagram{
        unsigned long ip;
	int id;
        struct datagram *link;
        struct fragment *child;
};

struct fragment{
        int fragment_offset;
        int more_fragment;
        int length;
        struct fragment *clink;
};

/*Global structure varibale to Store the List of Datagram*/
struct datagram *start;

/* Functions Declaration */
void display();
void insert_datagram(int,unsigned long,int,int,int);
void insert_fragment(int,int,int,struct datagram *);
void parser(FILE *);

/*int main(){

        FILE *fp;
        fp = fopen("data.txt", "r");
        convertion(fp);
        display();
        fclose(fp);
        return(0);
}*/

int main(int argc,char *argv[])
{
   
    FILE *fp;

   // printf("File name = %s\n", argv[1]);
    fp=fopen(argv[1],"r");
    
    if(fp==NULL) 
    { 
        perror("File cannot be opened");
      
       
        exit(1);
    }
    else 
    {
      	parser(fp);
      
        fclose(fp);
    }
   return(0);    
} 

void insert_datagram(int id,unsigned long ip,int fragment_offset,int more_fragment,int length)
{
    	struct datagram *head=NULL,*temp=NULL;
    
    	head=(struct datagram*)malloc(sizeof(struct datagram));
    	head->id=id;
    	head->ip=ip;
	head->link=NULL;
   	head->child=NULL;
    	temp=start;
	
    	if(start==NULL)
    	{
                start=head;
		insert_fragment(fragment_offset,more_fragment,length, head);
    	}
    	else{
                if(temp->link == NULL)
		{
                        if(temp->ip== head->ip && temp->id==head->id)
			{
				
                              		insert_fragment(fragment_offset,more_fragment,length,temp);
				
			}
                        else
			{
                                temp->link=head;
				
				insert_fragment(fragment_offset,more_fragment,length,head);	
			}
                }
                else{
                        while(temp->link != NULL)
                        {
                                if(temp->ip== head->ip && temp->id==head->id)
                                        break;
                                temp=temp->link;
                        }
                        if(temp->ip== head->ip && temp->id==head->id)
                               insert_fragment(fragment_offset,more_fragment,length, temp);
                        else
			{
                                temp->link=head;
				temp=temp->link;
				insert_fragment(fragment_offset,more_fragment,length, temp);
                	}   
		}
        }
}

void insert_fragment(int fragment_offset,int more_fragment,int length, struct datagram *parent)
{
        struct fragment *head=NULL,*temp=NULL;
	struct datagram *prev=NULL;
	int i;
    	
    	head=(struct fragment*)malloc(sizeof(struct fragment));
        head->fragment_offset=fragment_offset;
    	head->more_fragment=more_fragment;
    	head->length=length;
    	head->clink=NULL;
    	temp = parent->child;
	//cprev=temp;	
	prev=parent;
    	
    	if( temp ==NULL)
                parent->child = head;
		//cprev=head;
    	else
	{
		if(temp->clink==NULL)
		{
					
			 if(temp->fragment_offset== head->fragment_offset && temp->more_fragment==head->more_fragment && temp->length==head->length)
				{
                                      //  printf("\n%s\t\t%d\t\t%d\t%d\t%d-- Duplicate Fragment\n",inet_ntoa(*(struct in_addr *)&prev->ip,prev->id,head->fragment_offset,head->more_fragment,head->length);
                                      i=1;
                                   
				}	
			else
				temp->clink=head;				
		}
		else
		{
			i=0;               	
			
			while(temp->clink!=NULL)
			{
			 	 if(temp->fragment_offset== head->fragment_offset && temp->more_fragment==head->more_fragment && temp->length==head->length)
				{
                                       printf("\n%s\t\t%d\t-- Duplicate Fragment\n",inet_ntoa(*(struct in_addr *)&prev->ip),prev->id);
				
					i=1;
					break;
					
				}
				
                                temp=temp->clink;
				
                        }
			if(i==0){			
				temp->clink=head;
			}
		}
	}
}





void parser(FILE *file1)
{
        int id,fragment_offset,more_fragment,length;
        unsigned long converted_ip;
        id=0;
        fragment_offset=0;
        more_fragment=0;
        length=0;
        char ip[20],buffer[size];

        while(fgets(buffer,size,file1)!= NULL){
            sscanf(buffer,"%s %d %d %d %d",ip,&id,&fragment_offset,&more_fragment,&length);
            converted_ip=inet_addr(ip);
            if(converted_ip==0)
            {
            	printf("\n invalid address");
            }
            else
            {
            	insert_datagram(id,converted_ip,fragment_offset,more_fragment,length);
           }
        }
        display();

}

void display()
{
    struct datagram *temp=NULL;
    int current_length ,total_length,check,i=0;
   
    temp=start;
   
    while(temp!=NULL)
    {
	struct fragment *child=NULL;
	current_length=0;
	total_length=0;
	check=0;
	child=temp->child;
	
	while(child !=NULL)
	{
		if(child->more_fragment==0)
		{
			total_length=child->fragment_offset+child->length-20;
		}
		if((child->fragment_offset==0) && (child->more_fragment==0))
		{
			printf("\n%s\t\t%d\t\tIncomplete \t\t\tinconsistent data \n",inet_ntoa(*(struct in_addr *)&temp->ip),temp->id);
    	 		child = NULL;
    	 		check = check+1;
		}
		else
         	{  
          		current_length = (child->length + current_length-20);
          		//prev = child;
          		child = child->clink;
		}
	}
	 
         	i=i+1;
       	if(check == 0)
        {

    	   if(current_length == total_length)
    	    {
 	        printf("\n%s\t\t%d\t\t\t complet fragment\t\t\t No error\n", inet_ntoa(*(struct in_addr *)&temp->ip),temp->id);
    	    }
    	    else if(current_length > total_length)
    	        printf("\n%s\t\t%d\t \t\tReassembly incompleted\t\t\t Length Mismatch\n",inet_ntoa(*(struct in_addr *)&temp->ip),temp->id);
    	    else if(current_length<total_length)
    	         printf("\n%s\t\t%d\t\t\tReassembly incompleted\t\t\t fragment missing\n",inet_ntoa(*(struct in_addr *)&temp->ip),temp->id);
         }
        temp = temp->link;
    }
}



