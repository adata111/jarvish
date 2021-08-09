# J.A.R.V.I.SH. 
## *Just A Rather Very Incomplex SHell*

A very basic version of shell implemented in C.

<img src="/screenshots/jarvish1.png"/>

## Run the shell

1. Run the command `make`.
2. Run `./Jarvish` to get a banner with "J.A.R.V.I.SH." written followed by a prompt of the form `<username@system_name:path>` highlighted in cyan. The directory from where the shell is executed is the home(~) directory for the shell
3. Run any command in the shell. It can entail as many number of tabs and spaces, the shell accounts for those.
4. In order to exit, run `quit` or `exit` or `ctrl+C`.

<img src="/screenshots/jarvish2.png"/>

## Commands

### Built-in commands

These commands have been defined by me and are contained within the shell itself. Jarvish shows an error message if any invalid command is entered.

1. `echo [arguments]`   
    - Displays whatever is specified in [arguments].
    - Implemented in [echo.c](echo.c)

2. `cd [directory]`    
    - Changes directory to the directory specified, throws an error if the directory does not exist or if there are incorrect number of arguments.
    - Variations such as `cd, cd . , cd .., cd ~` also work.
    - `cd -` prints the previous working directory and switches to the previous working directory.
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

6. `setenv var [value]`
    - Sets value of the environment variable `var` to [value]. If value is omitted, the variable's value is set to an empty string.
    - Implemented in [command.c](command.c)

7. `unsetenv var`
    - Destroys the environment variable `var`, if it exists.
    - Implemented in [command.c](command.c)   

8. `jobs`
    - Prints a list of all existing background processes spawned by the shell in order of their creation times. 
    - Prints job number, process ID and their state, which can either be ​"running​" or ​"stopped"​.
    - Implemented in [jobs.c](jobs.c)

9. `kjob <job number> <signal number>`
    - Sends the signal corresponding to 'signal number' to the job number specified.
    - Throws error if `job number` is invalid or `signal number` is invalid.
    - Implemented in [kjob.c](kjob.c)

10. `fg <jobNumber>`
    - Brings a running or a stopped background job corresponding to `job number` to foreground.
    - Throws error if `job number` is invalid
    - Implemented in [fg.c](fg.c)

11. `bg <jobNumber>`
    - Changes a stopped background job to a running background job.
    - Throws error if `job number` is invalid
    - Implemented in [bg.c](bg.c)

12. `overkill`
    - ​Kills all background process at once.
    - Implemented in [overkill.c](overkill.c)

13. `quit` or `exit` or `ctrl+D`
    - Exits the shell, closing (sending SIGKILL to) all background processes and updates [history.txt](history.txt) file.

All other valid commands are treated as system commands like emacs, vim etc. This is implemented in [executeSys.c](executeSys.c)]

### Background Processes

- Command followed by '&' symbol is run in the background. Eg. `emacs &`.
- Upon termination of a background process, the shell prints in red its PID, name and if it exits abnormally, the exit status.
- When a background process is run, it prints number of background processes running currently and the pid of the last background process.
- Implemented in [executeSys.c](executeSys.c) 

### Additional Commands

1. `history [num]`
    - Lists the last [num] commands. If no arguments are specified, it displays the last 10 commands.
    - Retains upto 20 commands even upon shell exit - uses [history.txt](history.txt) to store these commands.
    - Implemented in [history.c](history.c) 

2. `nightswatch -n [seconds] [interrupt/newborn]`
    - `interrupt` argument prints the number of times CPU has been interrupted by keyboard.
    - `newborn` argument prints pid of the most recently created process.
    - Executes every [seconds] number of seconds as specified by user.
    - To exit press `q` and hit `enter`.
    - Error handling done for incorrect arguments.

### Additional Features

1. Input-Output Redirection & Piping
    - Handles `<`, `>`, `>>`, and `|` operators appropriately, wherever they are in the command

2. `​ctrl+Z`
    - Pushes any currently running foreground job into the
background, and changes its state from running to stopped

3. `ctrl+C`
    - Interrupt any currently running foreground job, by sending it SIGINT​ signal
    - If there is no foreground job, then the signal does not have any effect.

4. Exit Codes
    - Exit codes indicate whether a process exited normally, or encountered an error.
    - ​`:')​` is displayed alongside the next prompt if the previous command exited successfully and ​`:'(​`  is displayed if it encountered an error
    - For a pipeline or a semicolon separated list of commands, the exit status corresponds to that of the last command in the pipeline/list.

5. Command chaining with Logical AND and OR
    - The logical ​AND​ and ​OR​ operators can be used to chain commands, such that the exit code of the entire chain is the logical ​AND​ or ​OR​ (respectively) of the individual exit codes
    - `@` is used to denote `AND` and `$` for `OR`

#### To be noted: 
- Length of input should be less than 2000 characters in total.
- No more than 20 chained commands should be there.
- No more than 100 semi-colon separated commands should be there.