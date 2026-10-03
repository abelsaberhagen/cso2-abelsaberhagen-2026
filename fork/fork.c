#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int process_count;

int get_array_size(char *arr[]) {
    int count = 0;

    while (arr[count] != NULL) {
        count++;
    }
    return count;
}

void print_string_array(char **strArr) {

    for (int i = 0; i < sizeof(strArr); i++) {
        if (strArr[i] == NULL) {
            break;
        }
        printf("%s\n", strArr[i]);
    }
}


char **string_split(const char *input, const char *sep, int *num_words){

    *num_words = 0;
    int inputLen = strlen(input);
    char **sepStr = calloc(1000, sizeof(char *));
    char *word = calloc(1000, sizeof(char));

    printf("Input: %s\n", input);
    printf("Sep: %s\n", sep);
    printf("num_words: %d\n", *num_words);
    


    for (int i = 0; i <= inputLen; i++) {
        if (input[i] != '\0'){
            char c = input[i];
            
            for (int j = 0; j < strlen(sep); j++) {
                if (c == sep[j]) {
                   
                    c = '\0';
                    
                    if (sepStr[*num_words] == NULL) {
                        sepStr[*num_words] = malloc(inputLen * sizeof(char));
                    }
                    if (word[0] != '\0'){
                        strcpy(sepStr[*num_words], word);
                        printf("Word: %s\n", sepStr[*num_words]);
                        (*num_words)++;
                        free(word);
                        word = NULL;
                        word = calloc(1000, sizeof(char));
                    }
                }   
                 
            }
            char arr[2] = {c,'\0'};
            strcat(word, arr);
        } else {
            if (word != NULL && word[0] != '\0') {
                sepStr[*num_words] = calloc(1000, sizeof(char));
                strcpy(sepStr[*num_words], word);
                printf("Word: %s\n", sepStr[*num_words]);
                (*num_words)++;
                break;
            }
        }  
    }
    free(word);
    return sepStr;
}


void writeoutput(const char *command, const char *out_path, const char *err_path) {
    
    pid_t pid = fork();
    
    if (pid == 0) {
        
        printf("This is before open\n"); fflush(stdout);
        int fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int fd_er = open(err_path, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
        if (fd < 0) { printf("Error in open\n"); }
        printf("This is after open\n"); fflush(stdout);
        dup2(fd, 1);
        dup2(fd_er, 2);
        close(fd);
        close(fd_er);
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        perror("execl failed");
        _exit(127);

    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}


void parallelwriteoutput(int count, const char **argv_base, const char *out_path) {
    char *numbers[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    
    process_count = count;


    int size = 0;
    while (argv_base[size] != NULL) {
        size++;
    }

    char **input_text = calloc(1000, sizeof(char));
    printf("Size of: %d\n", size);
    for (int i = 0; i < size; i++) {
        input_text[i] = strdup(argv_base[i]);
    }
    


    pid_t pid = fork();
    if (pid == 0) {
        process_count--;
        printf("%d\n", process_count);
        if (process_count > 0) {
            parallelwriteoutput(process_count, argv_base, out_path);
        }

        input_text[size] = numbers[process_count];
        input_text[size + 1] = NULL;
        char *const *exec_args = (char *const *)input_text;

        int fd = open(out_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) { printf("Error in open\n"); } fflush(stdout);
        dup2(fd, 1);
        close(fd);
        execv(argv_base[0], exec_args);
        perror("execv failed");
        _exit(127);
    } else {
        int status;
        waitpid(pid, &status, 0);

        for (int i = 0; i < size; i++) {
            free(input_text[i]);
        }
        free(input_text);
    }

}

