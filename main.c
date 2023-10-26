// author: @Ruizhe
// created on: 2023.9.16
#include "execute.h"
#include "headers.h"
#include "signals.h"

int main(void) {
    signal(SIGINT, ctrlC);
    while (1) {
        signal(SIGINT, ctrlC);
        wait(NULL);
        char str[MAX_LENGTH];  // for the input line
        char* args[MAX_LENGTH];
        printf("mumsh $ ");
        fflush(stdout);
        char* Input = fgets(str, sizeof(str), stdin);
        if (Input == NULL) {
            if (feof(stdin)) {
                printf("exit\n");
                break;
            } else {
                signal(SIGINT, ctrlC_c);
                continue;
            }
        }
        str[strcspn(str, "\n")] = '\0';
        mum_exit(str);

        // before tokenize, add spaces when needed to form a new string (for
        // redirection)
        char space = ' ';
        char* str_new = (char*)malloc(MAX_LENGTH);
        str_new[0] = '\0';
        for (int i = 0; i < (int)strlen(str); i++) {
            char new_char[2];
            new_char[0] = str[i];
            new_char[1] = '\0';
            if (str[i] == '<') {
                if (i - 1 >= 0 && str[i - 1] != ' ' && str[i - 1] != '<') {
                    strncat(str_new, &space, 1);
                    strncat(str_new, new_char, 1);
                    if (i + 1 < (int)strlen(str) && str[i + 1] != '>') {
                        strncat(str_new, &space, 1);
                    }
                } else if (i + 1 < (int)strlen(str) && str[i + 1] != ' ' &&
                           str[i + 1] != '<') {
                    char new_char[2];
                    new_char[0] = str[i];
                    new_char[1] = '\0';
                    strncat(str_new, new_char, 1);
                    strncat(str_new, &space, 1);
                } else {
                    if (i + 1 < (int)strlen(str) && str[i + 1] == '<') {
                        char new_char[2];
                        new_char[0] = str[i];
                        new_char[1] = '\0';
                        strncat(str_new, &space, 1);
                        strncat(str_new, new_char, 1);

                    } else {
                        new_char[0] = str[i];
                        new_char[1] = '\0';
                        strncat(str_new, new_char, 1);
                        strncat(str_new, &space, 1);
                    }
                }
            } else if (str[i] == '>') {
                if (i - 1 >= 0 && str[i - 1] != ' ' && str[i - 1] != '>') {
                    strncat(str_new, &space, 1);
                    strncat(str_new, new_char, 1);
                    if (i + 1 < (int)strlen(str) && str[i + 1] != '>') {
                        strncat(str_new, &space, 1);
                    }
                } else if (i + 1 < (int)strlen(str) && str[i + 1] != ' ' &&
                           str[i + 1] != '>') {
                    char new_char[2];
                    new_char[0] = str[i];
                    new_char[1] = '\0';
                    strncat(str_new, new_char, 1);
                    strncat(str_new, &space, 1);

                } else {
                    new_char[0] = str[i];
                    new_char[1] = '\0';
                    strncat(str_new, new_char, 1);
                }
            } else {
                new_char[0] = str[i];
                new_char[1] = '\0';
                strncat(str_new, new_char, 1);
            }
        }

        // tokenize, split by empty spaces. store in an array
        const char split[] = " ";
        char* token = strtok(str_new, split);
        int args_cnt = 0;

        while (token != NULL) {
            args[args_cnt] = token;
            token = strtok(NULL, split);
            args_cnt++;
        }
        args[args_cnt] = NULL;
        char source[MAX_LENGTH];
        char destination[MAX_LENGTH];
        if (args_cnt != 0) {
            commands(args, source, destination);
        }
        free(str_new);
    }
}
