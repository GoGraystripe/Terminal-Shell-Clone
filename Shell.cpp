#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>  // for fork(), exec()
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for wait()
#include <string.h>   // For strdup() declaration

#include "Shell.h"


        

void Shell::run(){
    std::vector<std::string> previousCommands;
    while(true){                                        /* run indefinitely */
        std::cout << "Please input." << std::endl;      /* prompt for input */

        std::string input;                              /* declare an input string */
        std::getline(std::cin, input);                  /* set the input string equal to the entire line the user inputs */

        if (input == "exit"){
                std::cout << "Program terminated." << std::endl;     /* exit the forever-while loop if the user writes "exit" */
                break;
        }

        if(input == "!!"){
            if(!previousCommands.empty()){
                std::cout << "Repeating previous command: " << previousCommands.back() << std::endl;
                input = previousCommands.back();
            }
            else{
                perror("Error: no previous commands issued");
                continue;
            }
        }
        else{
            previousCommands.push_back(input);
        }

        std::vector<std::string> tokenized_input = splitInput(input); /* initialize a vector of strings called tokenized input */
                                                                      /* insert every word of the input string in, ignore spaces */
        if(!tokenized_input.empty()){                   /* if the tokenized_input vector isn't empty */

            if(tokenized_input[0] == "cd"){
                if(tokenized_input.size() > 1){
                    const char* path = tokenized_input[1].c_str();

                    if(chdir(path) != 0){
                        perror("Invalid file path provided");
                    }
                }
                else{
                    perror("No file path provided");
                }

                continue;
            }

            std::vector<char*> commands;                /* declare a vector of char*-s (pointers to char-s) */
            
            for(const auto& token : tokenized_input){      /* enhanced for-loop through tokenized_input */
                commands.push_back(strdup(token.c_str())); /* "auto" automatically detects the type of the token */
            }                                              /* "&" makes a reference to that token; adds the */
                                                           /* token.c_str() converts token to a C-style string*/
                                                           /* strdup() creates a copy of token.c_str() and char* that points there*/
            commands.push_back(nullptr);                   /* add a null-pointer to the end to tell the shell to terminate*/

            pid_t pid = fork();                            /* create a new pid_t object to read the command statement*/

            if(pid == 0){                                  /* if pid == 0, we are in the child process*/
                
                execvp(commands[0], commands.data()); /*execute the command at index 0 || use the rest of the user input as context */

                perror("Error executing command"); /* if execvp returns, an error has occurred*/
                exit(EXIT_FAILURE);

            }

            else if(pid > 0){ /* if pid > 0, we are in the parent process */
                int status;

                waitpid(pid, &status, 0); /* wait for the child process to finish */
            }

            else{ /* if pid < 0, fork() failed */
                perror("Error creating process");
            }

            freeCommands(commands);

        }

        
    }

}

std::vector<std::string> Shell::splitInput(const std::string& input){
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;

    while(stream >> token){
        tokens.push_back(token);
    }

    return tokens;
}

void Shell::freeCommands(std::vector<char*>& commands){
    for(char* command : commands){
        free(command);
    }
}



