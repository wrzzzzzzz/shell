#ifndef EXECUTE_H
#define EXECUTE_H
int redirection_type_and_reset_argu(char** args, char* source, char* destination);
void mum_exit(char* input);
void change_dir(char* args[]);
void pipe_dealing(char** args);
void file_redirection(int method, char* source, char* destination,char** args);
void commands(char* args[],char* source, char* destination);
#endif
