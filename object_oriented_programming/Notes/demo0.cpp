/*
**Commonly used shell commands**

echo  : prints whatever you type at the shell prompt.
date  : displays the current time and date.
clear : clears the terminal.

pwd   : stands for Print working directory and prints the path of the current working directory.
        This is also the default place where shell commands look for data files.
ls    : stands for List and lists the contents of a directory. If run without a path argument, ls
        lists the contents of the current directory.
cd    : stands for Change directory and changes the active directory to the path specified.

cp    : stands for Copy and it copies one or more files or directories, keeping the originals
        unaltered. We need to specify what we want to copy, i.e., the source and where we want to
        copy them, i.e., the destination.
mv    : stands for Move and it moves one or more files or directories from one place to another.
        We need to specify what we want to move, i.e., the source and where we want to move them,
        i.e., the destination.
touch : command is used to create new, empty files. It is also used to change the timestamps
    on existing files and directories.
mkdir : stands for Make directory and is used to make a new directory or a folder.
rm    : stands for Remove and it removes files or directories. By default, it does not remove
        directories, unless you provide the flag rm -r ( -r means recursively).

!!Warning!! Files removed via rm are lost forever, please be careful!

chmod +x script_file : To run your brand new script you may need to change the access permissions of the file. To make
a file executable run

**Not all commands are equal**

When executing an external command such as ls , the shell normally creates a subprocess. A
subprocess inherits the exported environment variables from the parent process, executes the
command, and returns control to the calling process.

A subprocess cannot change the state of the calling process.

The command "source script_file" executes the commands contained in script_file in the
current shell, as if they were typed directly in the terminal. It is commonly used for scripts that
change environment variables or define aliases or functions. Typing ". script_file" does the
same.
If the environment should not be altered, use "./script_file" , instead.

**Built-in commands**

Some commands, like "cd" are executed directly by the shell, without creating a subprocess.
Indeed it would be impossible to have "cd" as a regular command!
The reason is: a subprocess cannot change the state of the calling process, whereas "cd" needs
to change the current working directory of the shell itself. Bash also updates the PWD shell
variable, which contains the path of the current working directory.

type command_name : This command tells you whether a command is built-in or an external command. 
                    It also tells you the path of the command if it is an external command.
which command_name   :  This command tells you the path of the command if it is an external command. 
                        It does not tell you whether a command is built-in or not.

*/