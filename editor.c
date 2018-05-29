#include "editor.h"
#include "document.h"
#include "format.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//a 3 sdkjn
//a 30  aksjdh

char *get_filename(int argc, char *argv[]) {
  // TODO implement get_filename
  // take a look at editor_main.c to see what this is used for
	if(argc!=2)
	{
		print_usage_error();
		exit(1);
	}
  return argv[1];
}

void handle_display_command(Document *document, const char *command) {

	if(command==NULL)
		return;
	if(command[0]!='p')
		return;
	if(strlen(command)==1 && command[0]=='p')
	{
		for(int i = 0; i <(int)document->vector->size;i++)
		{
			print_line(document, i);
		}
		return;
	}
	if(document==NULL){
		print_document_empty_error();
		return;
	}
	else
	{
		if(command[1]!=' ')
		{
			invalid_command(command);
			return;
		}
		if(strlen(command)>1)
		{
			char linenum[100];
			int i=2;
			int j=0;

			int temp;

			while(i<(int)strlen(command)){
				temp = command[i];
				if(temp>57 || temp <48){
					invalid_command(command);
					return;
				}
				linenum[j] = command[i];
				i++;
				j++;
			}

			int finalline = atoi(linenum);
			if(finalline<1 || finalline>(int) document->vector->size)
			{
				invalid_line();
				return;
			}

			int minrange = finalline-6;
			int maxrange = finalline+5;
			if(minrange<0)
				minrange = 0;
			if(maxrange> (int)document->vector->size)
				maxrange = (int)document->vector->size;
			
			for(int i = minrange;i<maxrange;i++)
			{
				print_line(document, i);
			}
		}
		else
		{
			for(int i=0;i<(int)document->vector->size;i++)
			{
				print_line(document, i);
			}
		}
	}

  // TODO implement handle_display_command
}

void handle_write_command(Document *document, const char *command) {

	if(command == NULL || command[0]!='w')
		return;

	if(command[0]=='w')
	{
		if(command[1]!=' ')
		{
			invalid_command(command);
			return;
		}
		else
		{

			char *operation = malloc(sizeof(char));
			int linenum;
			char * mystring = malloc(strlen(command));
			sscanf(command, "%c %d %s", operation, &linenum, mystring);
			char * temporary = strstr(command, mystring);
			free(operation);

			free(mystring);
			mystring = temporary;
			

			char *begin = strstr(mystring, "$");
			if(begin==NULL)
			{
				Document_set_line(document, linenum - 1, mystring);
				return;
			}
			else
			{
				char *temp = strndup(mystring, begin-mystring);
				Document_set_line(document, linenum -1, temp);
				free(temp);
				
				while(1)
				{
					char * end = strstr(begin+1, "$");
					if(end==NULL)
					{
						temp = strdup(begin+1);
						Document_insert_line(document, linenum++, temp);
						free(temp);
						break;
					}
					else
					{
						temp = strndup(begin+1, end-begin-1);
						Document_insert_line(document, linenum++, temp);
						free(temp);
						begin = end;
					}
				}

			}
			

	 	}
	}
}

void handle_append_command(Document *document, const char *command) {

	if(command == NULL || command[0]!='a')
		return;

	if(command[0]=='a')
	{
		if(command[1]!=' ')
		{
			invalid_command(command);
			return;
		}
		else
		{
			char *operation = malloc(sizeof(char));
			int linenum;
			char * mystring = malloc(strlen(command));
			sscanf(command, "%c %d %s", operation, &linenum, mystring);
			free(mystring);		
			const char *parser = command+2;
			while(isdigit(*parser))
			{
				parser++;
			}
			parser = parser+1;
			mystring = strdup(parser);
			//printf("My String is:%s\n", mystring);	
		
			free(operation);

			char *begin = strstr(mystring, "$");

			if(linenum<=(int)document->vector->size)
			{
				if(begin==NULL)
				{	
					char *concattemp = malloc(strlen(Document_get_line(document, linenum-1)) + strlen(mystring) + 2);
					strcpy(concattemp, Document_get_line(document, linenum-1));
					strcat(concattemp, mystring);
					Document_set_line(document, linenum-1, concattemp);
					free(concattemp);
					free(mystring);
					return;
				}
				else
				{
					char *temp = strndup(mystring, begin-mystring);
					Document_set_line(document, linenum -1, temp);
					free(temp);
					
					while(1)
					{
						char * end = strstr(begin+1, "$");
						if(end==NULL)
						{
							temp = strdup(begin+1);
							Document_insert_line(document, linenum++, temp);
							free(temp);
							break;
						}
						else
						{
							temp = strndup(begin+1, end-begin-1);
							Document_insert_line(document, linenum++, temp);
							free(temp);
							begin = end;
						}
					}

				}
			}
			else
			{
				if(begin==NULL)
				{
					printf("Reach here 1\n");
					char *concattemp = strdup(mystring);
					Document_insert_line(document, linenum-1, concattemp);
					free(concattemp);
					free(mystring);
					return;
				}
				else
				{
					char *temp = strndup(mystring, begin-mystring);
					Document_insert_line(document, linenum -1, temp);
					free(temp);
					
					while(1)
					{
						char * end = strstr(begin+1, "$");
						if(end==NULL)
						{
							temp = strdup(begin+1);
							Document_insert_line(document, linenum++, temp);
							free(temp);
							break;
						}
						else
						{
							temp = strndup(begin+1, end-begin-1);
							Document_insert_line(document, linenum++, temp);
							free(temp);
							begin = end;
						}
					}
				}
			}
			free(mystring);
	 	}
	}
}

void handle_delete_command(Document *document, const char *command) {

	if(command==NULL || command[0]!='d')
			return;

	char *operation = malloc(sizeof(char));
	int linenum;	

	sscanf(command, "%c %d", operation, &linenum);
	free(operation);
	if(linenum==0)
	{
		invalid_command(command);
		return;
	}
	if(linenum>(int)document->vector->size)
	{
		invalid_line();
		return;
	}	
	Document_delete_line(document, linenum-1);
}

void handle_search_command(Document *document, const char *command) {

	if(command ==NULL|| command[0]!='/')
		return;

	char *stringtofind = strdup(command+1);

	for(int i=0;i<(int)document->vector->size;i++)
	{
		char *temp;
		temp = strstr(document->vector->array[i], stringtofind);
		if(temp==NULL)
			continue;
		else
			print_search_line(i+1, document->vector->array[i], temp, stringtofind);
	}
  // TODO implement handle_search_command
}

void handle_save_command(Document *document, const char *filename) {
		Document_write_to_file(document, filename);
  // TODO implement handle_save_command
}
