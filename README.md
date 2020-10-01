# My Linux Shell in C

A very basic version of shell.

## Run the shell

1. Run the command `make`.
2. Run `./myshell` to get a prompt of the form `<username@system_name:path>` highlighted in cyan.
3. Run any command in the shell. It can entail as many number of tabs and spaces, the shell accounts for those.
4. In order to exit, run `quit` or `exit` or `ctrl+C`.

## Commands

### Built-in commands

These commands have been defined by me and are contained within the shell itself.

1. `echo [arguments]`
    
    - Displays whatever is specified in [arguments].
    - Implemented in [echo.c](echo.c)

2. `cd [directory]`
    
    - Changes directory to the directory specified, throws an error if the directory does not exist or if there are incorrect number of arguments.
    - Variations such as `cd, cd . , cd .., cd ~` also work.
    - Implemented in [cd.c](cd.c)

3. `ls [-l -a] [directory]`
    
    - Lists all the files and directories in the specified directory in alphabetical order.
    - Variations such as `ls, ls . , ls .., ls ~` also work.
    - Also handles multiple directories as arguments. eg. `ls -l dir1 dir2 dir3`.
    - Mimics behaviour of bash by just printing filename if `ls` is tried on anything other than a directory. 
    - Implemented in [ls.c](ls.c)

4. `pwd`
    
    - Displays the name of the working directory.
    - Implemented in [command.c](command.c)

5. `pinfo [PID]`

    - Prints numerous details about the process such as its status, memory, and executable path.
    - Just `pinfo` with no arguments gives details of the shell.
    - Implemented in [pinfo.c](pinfo.c)


### Foreground and Background Processes

- All other commands are treated as system commands like emacs, vim etc.
- To run a process in the background, follow the command with a '&' symbol. Eg. `emacs &`.
- Upon termination of a background process, the shell prints its PID, name and exit status.
- When a background process is run, it prints number of background processes running currently and the pid of the last background process.
- Implemented in [executeSys.c](executeSys.c) 

### Additional Commands

1. `quit` or `exit` or `ctrl+C`

    - Exits the shell, closing all background processes and updates [history.txt](history.txt) file.

2. `history [num]`

    - Lists the last [num] commands. If no arguments are specified, it displays the last 10 commands.
    - Retains upto 20 commands even upon shell exit - uses [history.txt](history.txt) to store these commands.
    - Implemented in [history.c](history.c) 

3. `nightswatch -n [seconds] [interrupt/newborn]`

    - `interrupt` argument prints the number of times CPU has been interrupted by keyboard.
    - `newborn` argument prints pid of the most recently created process.
    - Executes every [seconds] number of seconds as specified by user.
    - To exit press `q` and hit `enter`.
    - Error handling done for incorrect arguments.
 
P.S. Pls make sure length of input is less than 2000 characters in total.
No more than 20 chained commands.