#!/bin/bash
great(){
    echo "Hello World, Greetings to $1 from $2"
}

# $0 : The name of the shell or script. In Bash, it is not replaced by the function name inside a
#   function; use ${FUNCNAME[0]} for that.
# $1 , $2 , $3 , etc.: The first, second, third (and so on) argument passed to the
#   script/function.
# $# : The number of arguments passed.
# $@ : All the arguments. When quoted as "$@" , each argument remains a separate word.
# $* : All the arguments. When quoted as "$*" , they are joined into a single word.

# Commands
# cat : stands for Concatenate and it reads a file and outputs its content. It can read any
#   number of files, and hence the name concatenate.
# wc : is short for Word count. It reads a list of files and generates one or more of the following
#   statistics: newline count, word count, and byte count.
# grep : stands for Global regular expression print. It searches for lines with a given string or
#   looks for a pattern in a given input stream.
# head : shows the first line(s) of a file.
# tail : shows the last line(s) of a file.
# file : reads the files specified and performs a series of tests in an attempt to classify them by
#   type.

# Redirection, Pipes and Filters
# We can add operators between commands in order to chain them together.
# The pipe operator | : forwards the output of one command to another. E.g., 
#       cat/etc/passwd | grep "my_username" checks system information about "my_username".
# The redirect operator > : sends the standard output of one command to a file. E.g., 
#       ls > files-in-this-folder.txt saves a file with the list of files.
# The error redirect operator 2> : sends the standard error of one command to a file. E.g., 
#       ls > output.txt 2> errors.txt saves a file with the list of files and a file with errors.
# The append operator >> : appends the output of one command to a file.
# In Bash, the operator &> : sends both standard output and standard error to a file.
# The && : operator executes the second command only if the exit status of the first command is
#   0. E.g., sudo apt update && sudo apt upgrade .
# The || : operator executes the second command only if the exit status of the first command is
#   not 0.
# The ; : operator separates commands and executes them sequentially regardless of their exit
#   status.
# $? is a variable containing the exit status of the last command.

# Advanced Commands
# tr : stands for translate. It supports a range of transformations including uppercase to
#   lowercase, squeezing repeating characters, deleting specific characters, and basic find and
#   replace. For instance:
#       1. echo "Welcome to Advanced Programming!" | tr '[:lower:]' '[:upper:]' converts
#          all lowercase characters to uppercase.
#       2. echo -e "A;B;c\n1,2;1,4;1,8" | tr "," "." | tr ";" "," replaces commas with
#          dots and semi-colons with commas.
#       3. echo "My ID is 73535" | tr -d '[:digit:]' deletes all the digits from the string.

# sed : stands for stream editor, and it can perform many operations on files, such as
#   searching, finding and replacing, inserting, or deleting. We give just a hint of its true power:
#       1. echo "UNIX is great OS. UNIX is open source." | sed "s/UNIX/Linux/" replaces
#          the first occurrence of "UNIX" with "Linux".
#       2. echo "UNIX is great OS. UNIX is open source." | sed "s/UNIX/Linux/2" replaces
#          the second occurrence of "UNIX" with "Linux".
#       3. echo "UNIX is great OS. UNIX is open source." | sed "s/UNIX/Linux/g" replaces
#          all occurrences of "UNIX" with "Linux".
#       4. echo -e "ABC\nabc" | sed "/abc/d" deletes lines matching "abc".
#       5. echo -e "1\n2\n3\n4\n5\n6\n7\n8" | sed "3,6d" deletes lines from 3 to 6.

# cut : is a command for cutting out the sections from each line of files and writing the result to
#   standard output.
#       1. cut -b 1-3,7- state.txt cuts bytes ( -b ) from 1 to 3 and from 7 to the end of each
#          line.
#       2. echo -e "A,B,C\n1.22,1.2,3\n5,6,7\n9.99999,0,0" | cut -d "," -f 1 gets the first
#          column of a CSV ( -d specifies the column delimiter; -f n selects the n-th field from
#          each line).

# find : is used to find files in specified directories that meet certain conditions. For example,
# find . -type d -name "*lib*" finds all directories (not files), starting from the current one
# ( . ), whose names contain "lib".

# locate : is less powerful than find but much faster, since it relies on a database that is
#   updated on a daily basis or manually using the command updatedb .
# For example, locate -i foo finds all files or directories whose names contain foo , ignoring case.


# Double quotes may be used to identify a string where the variables are interpreted. Single quotes
# identify a string where variables are not interpreted. Check the output of the following commands
a=yes
echo "$a"
echo '$a'

# The output of a command can be converted into a string and assigned to a variable for later reuse:
list=`ls -l` # Or, equivalently:
list=$(ls -l)

# ./my_command & : Run a command in background: 
# Ctrl-Z : suspends the current subprocess.
# jobs : lists all subprocesses running in the background in the terminal.
# bg %n : reactivates the n-th subprocess and sends it to the background.
# fg %n : brings the n-th subprocess back to the foreground.
# Ctrl-C : terminates the subprocess in the foreground (when not trapped).
# kill pid : sends termination signal to the subprocess with id pid . You can get a list of the
#   most computationally expensive processes with top and a complete list with "ps aux"
#   (usually "ps aux" is filtered through a pipe with "grep" )
# When the terminal is closed, background jobs may receive the "SIGHUP" signal. Whether they
#   terminate depends on the shell, its configuration, and how the processes handle that signal. Tools
#   such as "nohup" can be used to make a command ignore "SIGHUP" .
# man : command prints the documentation manual for command.
# info command : is also an info facility that sometimes provides more information