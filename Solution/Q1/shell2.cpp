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

int pipe_count=0, fd, flag, len, lines, env_flag, flag_pipe, flag_without_pipe, op_redir, ip_redir, bang_flag, status;
static char* args[512];
char *history, *cmd_exec[100], *input_redirection_file,*output_redirection_file ;
char buffer[1024], cwd[1024], history_data[1000][1000], current_directory[1000],ret_file[3000],his_var[2000] ;
pid_t pid;
extern char** environ;

//FUNCTION SIGNATURES//

void init_variables();
void process ();
void filewrite();
void bang_execute();
void environmment();
void set_environment_variables();
void change_directory();
void parent_directory();
void echo_calling(char *echo_val);
void history_execute_with_constants();
static char* skipwhite(char* s);
void tokenise_commands(char *com_exec);
void tokenise_redirect_input_output(char *cmd_exec);
void tokenise_redirect_input(char *cmd_exec);
void tokenise_redirect_output(char *cmd_exec);
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
  flag_pipe=0;
  flag_without_pipe=0;
  op_redir=0;
  ip_redir=0;
  buffer[0]='\0';
  cwd[0] = '\0';
  pid=0;
  env_flag=0;
  bang_flag=0;
}
  
void process ()
{
  int fd;
  history=(char *)malloc(100*sizeof(char));
  strcpy(history,current_directory);
  strcat(history, "/");
  strcat(history, "history.txt"); 
  fd=open(history, O_RDONLY|O_CREAT,S_IRUSR|S_IWUSR);
  int bytes_read=0, i=0, x=0, index=0;
  char buffer[1], temp[1000];
    do 
    {
        bytes_read = read (fd, buffer, sizeof (buffer));
        for (i=0; i<bytes_read; ++i) 
                  {
                    temp[index]=buffer[i];
                    index++;
                    if(buffer[i]=='\n')
                        {
                          temp[index-1]='\0';
                          lines++; 
                          index=0;
                          strcpy(history_data[x], temp);
                          x++;
                          temp[0]='\0';
                        }
                  }
    }
    while (bytes_read == sizeof (buffer)); 
    close (fd); 
}
void filewrite()
{
  
  int fd_out,ret_write,len=0;
  char input_data[2000];
  lines++;
  char no[10];
  sprintf (no, "%d", lines ); 
  strcpy(input_data, " ");
  strcat(input_data, no);
  strcat(input_data, " ");
  strcat(input_data, buffer);

  len = strlen(input_data);
  fd_out=open(history,O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR);
  len=strlen(buffer);
  ret_write=write(fd_out,input_data,len);
  if(ret_write<0) 
        {
          printf("Error in writing file\n");
          return;
        }
  close(fd_out);

}
void bang_execute()
{
  char bang_val[1000];
  char *tokenise_bang[100], *num_ch[10];
  int i, n=1, num, index=0;
  i=1;
  if(buffer[i]=='!')
        {
           strcpy(bang_val, history_data[lines-1]);
        }
  else if(buffer[i]=='-')
    {
        n=1;
        num_ch[0]=strtok(buffer,"-");
        while ((num_ch[n]=strtok(NULL,"-"))!=NULL)
              n++;
        num_ch[n]=NULL;
        num = atoi(num_ch[1]);

        index = lines-num;
        strcpy(bang_val, history_data[index]);
              
    }
  else 
    {
      num_ch[0]=strtok(buffer,"!");
      num = atoi(num_ch[0]);
      strcpy(bang_val, history_data[num-1]);
    }
  tokenise_bang[0]=strtok(bang_val," ");
  while ((tokenise_bang[n]=strtok(NULL,""))!=NULL)
              n++;
  tokenise_bang[n]=NULL;
  strcpy(bang_val, tokenise_bang[1]);
  printf("%s\n", bang_val );
  strcpy(buffer, bang_val);

    
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
void history_execute_with_constants()
{
  int num, i, start_index;
  if(bang_flag==1)
        {
         for(i=0; i<lines; i++)
            printf("%s\n", history_data[i] ); 
        }
  else if(args[1]==NULL)
      {
        for(i=0; i<lines-1; i++)
            printf("%s\n", history_data[i] );
        printf(" %d %s\n", lines, his_var );
      }
  else
      {
        if(args[1]!=NULL)
        num = atoi(args[1]);
        if(num>lines) 
        {
          for(i=0; i<lines-1; i++)
            printf("%s\n", history_data[i] );
          printf(" %d %s\n", lines, his_var );
        }
        start_index=lines-num;
        for(i=start_index; i<lines-1; i++)
            printf("%s\n", history_data[i] );
          printf(" %d %s\n", lines, his_var );
      } 

}
static char* skipwhite(char* s)
{
  while (isspace(*s)) ++s;
  return s;
}
void tokenise_commands(char *com_exec)
{
int m=1;
args[0]=strtok(com_exec," ");       
while((args[m]=strtok(NULL," "))!=NULL)
        m++;
}
void tokenise_redirect_input_output(char *cmd_exec)
{
  char *io_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  io_token[0]=strtok(new_cmd_exec1,"<");       
  while((io_token[m]=strtok(NULL,">"))!=NULL)
        m++;
  io_token[1]=skipwhite(io_token[1]);
  io_token[2]=skipwhite(io_token[2]);
  input_redirection_file=strdup(io_token[1]);
  output_redirection_file=strdup(io_token[2]);
  tokenise_commands(io_token[0]);
  
}
void tokenise_redirect_input(char *cmd_exec)
{
  char *i_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  i_token[0]=strtok(new_cmd_exec1,"<");       
  while((i_token[m]=strtok(NULL,"<"))!=NULL)
        m++;
  i_token[1]=skipwhite(i_token[1]);
  input_redirection_file=strdup(i_token[1]);
  tokenise_commands(i_token[0]);
}
void tokenise_redirect_output(char *cmd_exec)
{
  char *o_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  o_token[0]=strtok(new_cmd_exec1,">");       
  while((o_token[m]=strtok(NULL,">"))!=NULL)
          m++;
  o_token[1]=skipwhite(o_token[1]);
  output_redirection_file=strdup(o_token[1]); 
  tokenise_commands(o_token[0]);   
  
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
    if (strchr(cmd_exec, '<') && strchr(cmd_exec, '>')) 
            {
              ip_redir=1;
              op_redir=1;
              tokenise_redirect_input_output(cmd_exec);
            }
   else if (strchr(cmd_exec, '<')) 
        {
          ip_redir=1;
          tokenise_redirect_input(cmd_exec);
        }
   else if (strchr(cmd_exec, '>')) 
        {
          op_redir=1;
          tokenise_redirect_output(cmd_exec);
        }
    if(op_redir == 1)
                {                    
                        output_fd= creat(output_redirection_file, 0644);
                        if (output_fd < 0)
                          {
                          fprintf(stderr, "Failed to open %s for writing\n", output_redirection_file);
                          return(EXIT_FAILURE);
                          }
                        dup2(output_fd, 1);
                        close(output_fd);
                        op_redir=0;
                }
    if(ip_redir  == 1)
                  {
                         input_fd=open(input_redirection_file,O_RDONLY, 0);
                         if (input_fd < 0)
                          {
                          fprintf(stderr, "Failed to open %s for reading\n", input_redirection_file);
                          return(EXIT_FAILURE);
                          }
                        dup2(input_fd, 0);
                        close(input_fd);
                        ip_redir=0;
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
    else if (strcmp(args[0], "history") == 0)
             {
              history_execute_with_constants();
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
          strcpy(shell, "My_shell:");
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
      if(buffer[0]!='!')
            {
                process();
                filewrite(); 
            }         
      len = strlen(buffer);
      buffer[len-1]='\0';
      strcpy(his_var, buffer);
      if(strcmp(buffer, "exit") == 0) 
            {
              flag = 1;
              break;
            }
      if(buffer[0]=='!')  
              {
                process();
                bang_flag=1;
                bang_execute();
              }
      with_pipe_execute();
      waitpid(pid,&status,0);
         
    }  
    if(flag==1)
      {
      printf("Bye...\n");
      exit(0);       
      return 0;
      }
return 0;
}


