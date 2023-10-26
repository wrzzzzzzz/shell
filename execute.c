#include "execute.h"
#include "headers.h"
#include "signals.h"

// decide whether it's <, >, >>, < & >, or < & >>
// rule of return values indicating redirection type:
//<: 1;
//>: 2;
//  >>: 3;
//  < & >: 4;
//  < & >>: 5;
//  |: 6;
// default: return 0
// all the errors: return -9
int redirection_type_and_reset_argu(char** args,
                                    char* source,
                                    char* destination) {
    int size = 0;
    while (args[size] != NULL) {
        size++;
    }
    for (int i = 0; i < size; i++) {
        if (strcmp(args[i], "<") == 0) {
            if (i + 1 < size && args[i + 1] != NULL) {
                strcpy(source, args[i + 1]);
                args[i] = NULL;
                args[i + 1] = NULL;
                for (int j = i; j < size - 2; j++) {
                    args[j] = args[j + 2];
                }
                args[size - 2] = NULL;
                args[size - 1] = NULL;
            }
            for (int j = i; j < size; j++) {  //
                if (args[j] != NULL && strcmp(args[j], ">") == 0) {
                    if (j + 1 < size && args[j + 1] != NULL) {
                        strcpy(destination, args[j + 1]);
                        args[j] = NULL;
                        args[j + 1] = NULL;
                        for (int m = j; m < size - 2; m++) {
                            args[m] = args[m + 2];
                        }
                    }
                    return 4;
                } else if (args[j] != NULL && strcmp(args[j], ">>") == 0) {
                    if (j + 1 < size && args[j + 1] != NULL) {
                        strcpy(destination, args[j + 1]);
                        args[j] = NULL;
                        args[j + 1] = NULL;
                        for (int m = j; m < size - 2; m++) {
                            args[m] = args[m + 2];
                        }
                    }
                    return 5;
                } else if (args[j] != NULL && strcmp(args[j], "<") == 0) {
                    if (j + 1 < size &&
                        strcmp(args[j + 1], "<") !=
                            0) {  // exactly a file behind, dup input
                        printf("error: duplicated input redirection\n");
                        return -9;
                    }
                    // things regarding syntex error
                    exit(0);
                }
            }
            return 1;
        } else if (strcmp(args[i], ">") == 0) {
            if (i == 0) {
                printf("error: missing program\n");
                return -9;
            }
            if (i + 1 < size && args[i + 1] != NULL) {
                if (strcmp(args[i + 1], ">") == 0 ||
                    strcmp(args[i + 1], "<") == 0 ||
                    strcmp(args[i + 1], "|") == 0) {
                    printf("syntax error near unexpected token `%s'\n",
                           args[i + 1]);
                    return -9;
                }
                strcpy(destination, args[i + 1]);

                args[i] = NULL;
                args[i + 1] = NULL;
                for (int j = i; j < size - 2; j++) {
                    args[j] = args[j + 2];
                }

                args[size - 2] = NULL;
                args[size - 1] = NULL;
            }

            for (int j = i; j < size; j++) {  //
                if (args[j] != NULL && strcmp(args[j], "<") == 0) {
                    if (j + 1 < size && args[j + 1] != NULL) {
                        strcpy(source, args[j + 1]);
                        args[j] = NULL;
                        args[j + 1] = NULL;
                        for (int m = j; m < size - 2; m++) {
                            args[m] = args[m + 2];
                        }
                    }
                    return 5;
                } else if (args[j] != NULL && strcmp(args[j], ">") == 0) {
                    printf("error: duplicated output redirection\n");
                    return -9;
                } else if (args[j] != NULL && strcmp(args[j], ">>") == 0) {
                    printf("error: duplicated output redirection\n");
                    return -9;
                } else if (args[j] != NULL && strcmp(args[j], "|") == 0) {
                    printf("error: duplicated output redirection\n");
                    return -9;
                }
            }
            return 2;
        } else if (strcmp(args[i], ">>") == 0) {
            if (i + 1 < size && args[i + 1] != NULL) {
                strcpy(destination, args[i + 1]);
                args[i] = NULL;
                args[i + 1] = NULL;
                for (int j = i; j < size - 2; j++) {
                    args[j] = args[j + 2];
                }
                args[size - 2] = NULL;
                args[size - 1] = NULL;
            }
            for (int j = i; j < size; j++) {  //
                if (args[j] != NULL && strcmp(args[j], "<") == 0) {
                    if (j + 1 < size && args[j + 1] != NULL) {
                        strcpy(source, args[j + 1]);
                        args[j] = NULL;
                        args[j + 1] = NULL;
                        for (int m = j; m < size - 2; m++) {
                            args[m] = args[m + 2];
                        }
                    }
                    return 4;
                } else if (args[j] != NULL && strcmp(args[j], ">") == 0) {
                    if (j + 1 < size && args[j + 1] != NULL &&
                        strcmp(args[j + 1], "<") != 0) {
                        printf("error: duplicated output redirection\n");
                        return -9;
                    }

                } else if (args[j] != NULL && strcmp(args[j], ">>") == 0) {
                    printf("error: duplicated output redirection\n");
                    return -9;
                } else if (args[j] != NULL && strcmp(args[j], "|") == 0) {
                    printf("error: duplicated output redirection\n");
                    return -9;
                }
            }
            return 3;
        } else if (strcmp(args[i], "|") == 0) {
            if (i + 1 < size && strcmp(args[i + 1], "|") == 0) {
                printf("error: missing program\n");
                return -9;
            }
            for (int j = i; j < size; j++) {
                if (args[j] != NULL && strcmp(args[j], "<") == 0) {
                    printf("error: duplicated input redirection\n");
                    return -9;
                }
            }
            return 6;
        }
    }
    return 0;
}

void mum_exit(char* input) {  // function for exiting the shell totally
    if (strcmp(input, "exit") == 0) {
        printf("exit\n");
        exit(0);
    }
}

void change_dir(char* args[]) {
    if (args[0] != NULL && strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            if (chdir(args[1]) == 0) {
                // printf("success in changing\n");
            } else {
                printf("%s: No such file or directory\n", args[1]);
                exit(0);
            }
        }
    }
}

// reference: lab presentation
void pipe_dealing(char** args) {
    int size = 0;
    while (args[size] != NULL) {
        size++;
    }
    char* left_command[MAX_LENGTH];  // break the commands into left and right
                                     // part, splitted by |
    char* right_command[MAX_LENGTH];
    int flag = 0;
    int right_cnt = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(args[i], "|") == 0) {
            flag = 1;
            continue;
        }
        if (flag == 0) {
            left_command[i] = args[i];
        } else {
            right_command[right_cnt] = args[i];
            right_cnt++;
        }
    }

    // create and execute a pipe
    int fd[2];
    pipe(fd);
    pid_t ppid = fork();
    if (ppid == 0) {
        close(fd[0]);
        // printf("about to execute the left command\n");
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        if (execvp(left_command[0], left_command) != 0) {
            printf("%s: command not found\n", left_command[0]);
            exit(0);
        }
        exit(0);
    } else {
        close(fd[1]);
        // printf("about to execute the right command\n");
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        if (execvp(right_command[0], right_command) != 0) {
            // printf("the right command is: %s\n",right_command[0]);
            printf("%s: command not found\n", right_command[0]);
            exit(0);
        }
        exit(0);
    }
}

//reference: lab presentation
void file_redirection(int method,
                      char* source,
                      char* destination,
                      char** args) {
    if (method == 1) {  //<
        int fd = open(source, O_RDONLY, 0666);
        if (fd == -1) {
            printf("%s: No such file or directory\n", source);
            // return;
            exit(0);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    } else if (method == 2) {  //>

        int fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
            printf("%s: Permission denied\n", destination);
            exit(0);
        } else if (fd != -1) {
            int val = dup2(fd, STDOUT_FILENO);
            if (val == -1) {
                perror("this is a file error\n");
            }
            close(fd);
        } else {
            perror("error with the file\n");
        }
    } else if (method == 3) {  //>>
        int fd = open(destination, O_WRONLY | O_CREAT | O_APPEND, 0666);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    } else if (method == 4) {  // < & >
        int fd_s = open(source, O_RDONLY, 0666);
        int fd_d = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd_s, STDIN_FILENO);
        close(fd_s);
        dup2(fd_d, STDOUT_FILENO);
        close(fd_d);
    } else if (method == 5) {  // < & >>
        int fd_s = open(source, O_RDONLY, 0666);
        dup2(fd_s, STDIN_FILENO);
        close(fd_s);
        int fd_d = open(destination, O_WRONLY | O_CREAT | O_APPEND, 0666);
        dup2(fd_d, STDOUT_FILENO);
        close(fd_d);
    } else if (method == 6) {  // | codes for the pipe
        pipe_dealing(args);
    } else if (method == -9) {
        return;
    }
}

void commands(char* args[],
              char* source,
              char* destination) {  // for single commands
    if (args[0] != NULL && strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == 0) {
            // printf("success\n");
        } else {
            printf("%s: No such file or directory\n", args[1]);
            // exit(0);
        }
    } else {
        int method = redirection_type_and_reset_argu(args, source, destination);
        // file_redirection(method, source, destination);
        // printf("string now is: %s%s%s\n", args[0], args[1], args[2]);
        if (method == -9) {
            return;
        }
        pid_t cpid = fork();
        if (cpid == 0) {
            // printf("child process\n");
            // signal(SIGINT, ctrlC_cc);
            file_redirection(method, source, destination, args);
            if (method == 6) {
                return;
            }
            // printf("finished the redirection action\n");
            if (execvp(args[0], args) != 0) {
                printf("%s: command not found\n", args[0]);
                exit(0);
            }
            // fflush(stdout);
        } else if (cpid > 0) {
            signal(SIGINT, ctrlC_cc);
            // printf("I'm here!!. parent\n");
            wait(NULL);
        } else {
            perror("error in the process\n");
            exit(1);
        }
    }
}
