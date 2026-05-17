A pretty standard shell written in C++

Compile using:
```c++
g++ -o shell -std=c++17 main.cpp shell.cpp
```

Run using:
```c++
./shell
```

Some things I'll fix pretty soon:

-> Input & output redirection cannot stack into more than 1 command at a time
-> Piping cannot stack into more than 1 command at a time

(Eg: ls > a.txt works, but ls > a.txt > b.txt doesn't)