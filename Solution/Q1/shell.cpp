#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
using namespace std;

//GLOBAL VARIABLES//

int pipe_count=0, fd, flag, len, lines, env_flag, status;
static char* args[512];
char  *cmd_exec[100];
char buffer[1024], cwd[1024], current_directory[1000];
pid_t pid;
extern char** environ;

//FUNCTION SIGNATURES//

void init_variables();
void environmment();
void set_environment_variables();
void change_directory();
void parent_directory();
void echo_calling(char *echo_val);
char* skipcomma(char* str);
static int split(char *cmd_exec, int, int, int);
void with_pipe_execute();
static int command(int, int, int, char *cmd_exec);
void prompt();
void sigintHandler(int sig_num);

//#################################################//

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}
void init_variables()
{
  fd =0;
  flag=0;
  len=0;
  lines=0;
  pipe_count=0;

  buffer[0]='\0';
  cwd[0] = '\0';
  pid=0;
  env_flag=0;
  
}
  

void environmment()
{
  int i =1, index=0;
  char env_val[1000], *value;
  while(args[1][i]!='\0')
              {
                   env_val[index]=args[1][i];
                   index++;
                    i++;
              }
  env_val[index]='\0';
  value=getenv(env_val);

  if(!value)
      printf("\n");
  else printf("%s\n", value);
}

void set_environment_variables()
{  
int n=1;
char *left_right[100];
if(args[1]==NULL)
      {
        char** env;
          for (env = environ; *env != 0; env++)
          {
            char* value = *env;
            printf("declare -x %s\n", value);    
          }  
         return; 
      }
left_right[0]=strtok(args[1],"=");
while ((left_right[n]=strtok(NULL,"="))!=NULL)
      n++;
left_right[n]=NULL;
setenv(left_right[0], left_right[1], 0);
}


 
void change_directory()
{
char *h="/home";   
if(args[1]==NULL)
        chdir(h);
else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
        chdir(h);
else if(chdir(args[1])<0)
    printf("bash: cd: %s: No such file or directory\n", args[1]);

}

void parent_directory()
{
if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
         printf("%s\n", cwd );
        }
else
       perror("getcwd() error");


}
void echo_calling(char *echo_val)
{
  int i=0, index=0;
  env_flag=0;
  char new_args[1024],env_val[1000], *str[10];
  str[0]=strtok(echo_val," ");
  str[1]=strtok(NULL, "");
  strcpy(env_val, args[1]);
  if(str[1]==NULL)
         {
          printf("\n");
          return;
         } 
  if (strchr(str[1], '$')) 
                  {
                  env_flag=1;
                  }
  
  memset(new_args, '\0', sizeof(new_args));
  i=0; 
  while(str[1][i]!='\0')
    {
      if(str[1][i]=='"')
      {
      index=0;     
      while(str[1][i]!='\0')
          {
          if(str[1][i]!='"')
                {
                new_args[index]=str[1][i];
                 flag=1;
                index++;
                }
          i++;
          }             
      }
      else if(str[1][i]==39)
      {
      index=0;
      while(str[1][i]!='\0')
          {
          if(str[1][i]!=39)
                {
                new_args[index]=str[1][i];
                 flag=1;
                index++;
                }
          i++;
          }               
      }
      else if(str[1][i]!='"')
        {
          new_args[index]=str[1][i];
          index++;
          i++;
        }
      else i++;    
    }


new_args[index]='\0';
if((strcmp(args[1], new_args)==0)&&(env_flag==0))
    printf("%s\n", new_args);
else {
     strcpy(args[1], new_args);
      if(env_flag==1)
                {
                environmment();
                }
      else if(env_flag==0)
                {
                  printf("%s\n", new_args ); 
                }
    }  
}
char* skipcomma(char* str)
{
  int i=0, j=0;
  char temp[1000];
  while(str[i++]!='\0')
            {
              if(str[i-1]!='"')
                    temp[j++]=str[i-1];
            }
        temp[j]='\0';
        str = strdup(temp);
  
  return str;
}
static int split(char *cmd_exec, int input, int first, int last)
{
    char *new_cmd_exec1;  
    new_cmd_exec1=strdup(cmd_exec);
   //else
      {
        int m=1;
        args[0]=strtok(cmd_exec," ");       
        while((args[m]=strtok(NULL," "))!=NULL)
              m++;
        args[m]=NULL;
        if (args[0] != NULL) 
            {

            if (strcmp(args[0], "exit") == 0) 
                    exit(0);
            if (strcmp(args[0], "echo") != 0) 
                    {
                      cmd_exec = skipcomma(new_cmd_exec1);
                      int m=1;
                      args[0]=strtok(cmd_exec," ");       
                      while((args[m]=strtok(NULL," "))!=NULL)
                                m++;
                      args[m]=NULL;

                    }
            if(strcmp("cd",args[0])==0)
                    {
                    change_directory();
                    return 1;
                    }
            else if(strcmp("pwd",args[0])==0)
                    {
                    parent_directory();
                    return 1;
                    }
           
            }
        }
    return command(input, first, last, new_cmd_exec1);
}


void with_pipe_execute()
{

int i, n=1, input, first;

input=0;
first= 1;

cmd_exec[0]=strtok(buffer,"|");

while ((cmd_exec[n]=strtok(NULL,"|"))!=NULL)
      n++;
cmd_exec[n]=NULL;
pipe_count=n-1;
for(i=0; i<n-1; i++)
    {
      input = split(cmd_exec[i], input, first, 0);
      first=0;
    }
input=split(cmd_exec[i], input, first, 1);
input=0;
return;

}
static int command(int input, int first, int last, char *cmd_exec)
{
  int mypipefd[2], ret, input_fd, output_fd;
  ret = pipe(mypipefd);
  if(ret == -1)
      {
        perror("pipe");
        return 1;
      }
  pid = fork();
 
  if (pid == 0) 
  {
    if (first==1 && last==0 && input==0) 
    {
      dup2( mypipefd[1], 1 );
    } 
    else if (first==0 && last==0 && input!=0) 
    {
      dup2(input, 0);
      dup2(mypipefd[1], 1);
    } 
    else 
    {
      dup2(input, 0);
    }
   if (strcmp(args[0], "export") == 0)
                  {
                  set_environment_variables();
                  return 1;
                  }
    if (strcmp(args[0], "echo") == 0)
              {
              echo_calling(cmd_exec);
    }
 
    else if(execvp(args[0], args)<0) printf("%s: command not found\n", args[0]);
              exit(0);
  }
  else 
  {
     waitpid(pid, 0, 0);  
   }
 
  if (last == 1)
    close(mypipefd[0]);
  if (input != 0) 
    close(input);
  close(mypipefd[1]);
  return mypipefd[0];

}
void prompt()
{
  char shell[1000];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
          strcpy(shell, "MyShell:");
          strcat(shell, cwd);
          strcat(shell, "$ ");

          printf("%s", shell);
        }
   else
       perror("getcwd() error");

}

int main()
{   
    int status;
    char ch[]={"\n"};
    getcwd(current_directory, sizeof(current_directory));
    signal(SIGINT, sigintHandler);
    while (1)
    {
      init_variables();
      prompt();
      fgets(buffer, 1024, stdin);
      if(strcmp(buffer, ch)==0)
            {
              continue;
            }
    
      len = strlen(buffer);
      buffer[len-1]='\0';

      if(strcmp(buffer, "exit") == 0) 
            {
              flag = 1;
              break;
            }
   
      with_pipe_execute();
      waitpid(pid,&status,0);
         
    }  
    if(flag==1)
      {
      printf("ByeBye\n");
      exit(0);       
      return 0;
      }
return 0;
}


