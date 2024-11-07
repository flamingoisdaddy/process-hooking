[-] this pgoram uses ptrade syscalls to allow the user to select a random process and read data from a specific memory addr [-]
----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------
[+]how it works[+]
----------------------------------------------------------------------------------------

[+] scans "/proc" (directory) to gather the list of active process id's
[+] prompts the user for the PID to hook based off of the 3 random listed PID's
[+] uses ptrace to attach to the process that was picked based off of user input
[+] reads memory addr for the selected proc memory map and then reads the data at that mem addr
[+] prints results
----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------
[+]setup[+]
----------------------------------------------------------------------------------------

[+] download the GCC compiler for C/C++ 
[+] make the file
[+] g++ -o hook hook.cpp (hook is changeable, hook.cpp is what you named the c++ file)
[+] ./hook 
----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------
[+]requirements[+]
----------------------------------------------------------------------------------------
[+] UNIX esc system
[+] c++11 or later
[+] root priv to manipulate other processes(you can execute from base user privileges)
----------------------------------------------------------------------------------------
