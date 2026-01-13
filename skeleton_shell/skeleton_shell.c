#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    char input[256];  // buffer for user input
    char *args[10];   // array of pointers to characters
    char bjack[15];
    pid_t pid;
    int status;

    srand(time(NULL)); // seed random number generator

    while (1) {
        // Print prompt
        printf("myshell> ");
        fflush(stdout);

        // Get user input with fgets
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // Ctrl+D (EOF)
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        // Built-in commands
        if (strcmp(input, "exit") == 0 || strcmp(input, "close") == 0) { 
            break;
        }
        if (strcmp(input, "help") == 0) {
            printf("Simple shell using fork() and exec().\n");
            printf("Built-ins: help, exit, close, or blackjack.\n");
            continue;
        }

        // Blackjack game
        if (strcmp(input, "blackjack") == 0) {
            int player_total = (rand() % 10 + 1) + (rand() % 10 + 1);
            int dealer_total = (rand() % 10 + 1) + (rand() % 10 + 1);

            printf("Your starting hand: %d\n", player_total);

            while (1) {
                printf("Press (h) to hit or (s) to stand: ");
                if (fgets(bjack, sizeof(bjack), stdin) == NULL) break;
                bjack[strcspn(bjack, "\n")] = 0;

                if (strcmp(bjack, "h") == 0) {
                    int card = rand() % 10 + 1;
                    player_total += card;
                    printf("You drew %d. Your total: %d\n", card, player_total);
                    if (player_total > 21) {
                        printf("Bust! Dealer wins.\n");
                        break;
                    }
                } else if (strcmp(bjack, "s") == 0) {
                    printf("Dealer's total: %d\n", dealer_total);
                    while (dealer_total < 17) {
                        int card = rand() % 10 + 1;
                        dealer_total += card;
                        printf("Dealer draws %d. Dealer total: %d\n", card, dealer_total);
                    }
                    if (dealer_total > 21 || player_total > dealer_total) {
                        printf("You win!\n");
                    } else if (player_total < dealer_total) {
                        printf("Dealer wins!\n");
                    } else {
                        printf("It's a tie!\n");
                    }
                    break;
                } else {
                    printf("Invalid choice. Type 'h' or 's'.\n");
                }
            }
            continue;
        }

        // Split input into words (split on spaces)
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < 9) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // NULL-terminate the array

        if (args[0] == NULL) continue; // empty input

        // Fork and exec
        pid = fork();
        if (pid == 0) {
            execvp(args[0], args); // search in /usr/bin and PATH
            perror("execvp");      // only runs if exec fails
            exit(1);
        } else if (pid > 0) {
            waitpid(pid, &status, 0); // parent waits
        } else {
            perror("fork");
        }
    }

    return 0;
}
