#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd[2];  // fd[0] okuma için, fd[1] yazma için
    pipe(fd);
    
    pid_t pid = fork();
    
    if (pid == 0)  // Child process
    {
        close(fd[0]);  // Okuma ucunu kapat
        char *msg = "Merhaba!";
        write(fd[1], msg, 8);
        close(fd[1]);
        exit(0);
    }
    else  // Parent process
    {
        close(fd[1]);  // Yazma ucunu kapat
        char buf[8];
        read(fd[0], buf, 8);
        buf[8] = '\0';
        printf("Mesaj alındı: %s\n", buf);
        close(fd[0]);
    }
    return 0;
}