#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <cstring>

int randomGenerator(int argc, char *argv[]) {
    
    

    const char* args[argc + 1];

    args[0] = "./rgen";
    
    // Copy the command line arguments received by ece650-a3
    for (int i = 1; i < argc; ++i) {
        args[i] = argv[i];
    }

    args[argc] = nullptr;

    
    
    execv(args[0], const_cast<char* const*>(args));
    
    std::cerr
        << "pid: " << getpid()
        << " Error: failed to execute the C++ program" << std::endl;
    return 1;
    
}

int pythonRunner() {
    
    execlp("python3", "python3", "ece650-a1.py", NULL);

    std::cerr
        << "pid: " << getpid()
        << " Error: failed to execute the Python program" << std::endl;
    return 1;
}


int shortestPath() {
    
    
    const char* args[3];
    args[0] = "./ece650-a2";  
    args[1] = nullptr;
    
    execv(args[0], const_cast<char* const*>(args));
    
    std::cerr
        << "pid: " << getpid()
        << " Error: failed to execute the C++ program" << std::endl;
    return 1;
    
}

int main(int argc, char *argv[]) {

   
    
    int pipe1[2];
    pipe(pipe1);


    pid_t pid;

    pid = fork();
   
    if (pid == 0) {
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);

        //std::cerr << "First child \n";
        
        return randomGenerator(argc, argv);
    } else if (pid > 0) {
        // std::cout << "I am a parent process, and created child: "
        //           << pid << std::endl;
    }
    else
    {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }

 

    int pipe2[2];
    pipe(pipe2);
    
    pid_t pid2;
    pid2 = fork();
    if (pid2 == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);

        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[0]);
        close(pipe2[1]);

        //std::cout << "Second child \n";
        
        pythonRunner();
        

        // This part will be reached only if exec fails
        std::cerr << "Second child process: Failed to execute the Python program" << std::endl;
        return 1;
    } else if (pid2 > 0) {
        // std::cout << "I am a parent process, and created second child: "
        //           << pid2 << std::endl;
    }
    else
    {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }

  

    pid_t pid3;
    pid3 = fork();
    if (pid3 == 0) {

        close(pipe1[0]);
        close(pipe1[1]);

        dup2(pipe2[0], STDIN_FILENO);
        
        close(pipe2[0]);
        close(pipe2[1]);


        //std::cout << "Third child \n";


        
        shortestPath();
        

        // This part will be reached only if exec fails
        std::cerr << "Third child process: Failed to execute the C++ program" << std::endl;
        return 1;
    } else if (pid2 > 0) {
        // std::cout << "I am a parent process, and created second child: "
        //           << pid2 << std::endl;
    }
    else
    {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }


    close(pipe1[0]);
    close(pipe1[1]);


    dup2(pipe2[1], STDOUT_FILENO);

    


    while (!std::cin.eof()) {
       
        std::string input;
        std::getline(std::cin, input);

        std::cout <<input<<"\n";
        //std::cout.flush();
    
    }

    close(pipe2[1]);



    //Kill command
    kill(pid, SIGTERM);  // Terminate the first child process
    kill(pid2, SIGTERM); // Terminate the second child process
    kill(pid3, SIGTERM); // Terminate the third child process

    
    int status1, status2, status3;

    waitpid(pid, &status1, 0);
    waitpid(pid2, &status2, 0);
    waitpid(pid3, &status3, 0);
    
    return 0;
}