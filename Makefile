all: a b Student-Management-System 

a: regular_request_generator.c
    gcc -o a regular_request_generator.c

b: random_request_generator.c
    gcc -o b random_request_generator.c

OS_Project: OS_Project.c
    gcc -o Student-Management-System  OS_Project.c

clean:
    rm -f a b Student-Management-System 