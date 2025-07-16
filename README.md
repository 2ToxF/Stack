# Toxic's Stack

## Description
I guess all of you know what stack is. And that's my implementation that I'm going to use in my future projects. Stack uses hash (DJB2) and canary protection. Also it encodes pointer, so it isn't so easy to find real stack in memory (but not very complicated too).


## Installation
Clone repo:
```
git clone https://github.com/2ToxF/Stack.git
```

Build project:
```
cmake -S . -B build
cmake --build build
```

Now you have static library file in "lib/" directory. If you use this project as a module of another, then "lib/" will be created in the top level of the source tree (CMAKE_SOURCE_DIR).

Also there are 2 build types. By default "Release"-type is set. If you want to specify it, then use:
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
```
```
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
```
instead of `cmake -S . -B build`


## Documentation
First, generate documentation:
```
doxygen Docs_config
```
Then open `documentation/html/index.html`


## Usage
All functions and macros below return value of "CodeError" data type.

```
List of the most important functions and macros:

    CREATE_STACK(size_t* stk_enc_ptr)                    //  creates and initializes the stack (returns encoded pointer)
    StackDtor   (size_t* stk_enc_ptr)                    //  destructs the stack
    StackPush   (size_t stk_enc_ptr, StackElem_t value)  //  puts value to stack
    StackPop    (size_t stk_enc_ptr, StackElem_t* var)   //  pulls value from stack
```


## Contributing and feedback
You can always find me on Telegram 👉 [Toxic](t.me/TToxFac)

I would really like to see your pull requests and discuss your ideas 😜

![kitty](https://media1.tenor.com/m/bHHunoDZd7sAAAAd/что-ну-что.gif)
