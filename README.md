
# Toxic's Stack

## Description
I guess all of you know what stack is. And that's my variation that I'm going to use in my future projects. Stack uses hash (DJB2) and canary protection. Also it encodes pointer, so it isn't so easy to find real stack in memory (but not very complicated too).

## Installation
Just download files from include/ and source/ (you do not need my "main.cpp") and include "stack.h" to your project.

## Usage
All functions and macros below return value of "CodeError" data type.

```
List of the most important functions (no caps) and macros (caps):

    CREATE_STACK(size_t* stk_enc_ptr)                    //  creates and initializes the stack (returns encoded pointer)
    StackDtor   (size_t* stk_enc_ptr)                    //  destructs the stack
    StackPush   (size_t stk_enc_ptr, StackElem_t value)  //  puts value to stack
    StackPop    (size_t stk_enc_ptr, StackElem_t* var)   //  pulls value from stack
```

## Contributing and feedback
You can always find me on Telegram 👉 [Toxic](t.me/TToxFac)

I would really like to see your pull requests and discuss your ideas 😜

![kitty](https://media1.tenor.com/m/bHHunoDZd7sAAAAd/что-ну-что.gif)
