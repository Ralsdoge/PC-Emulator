#pragma once //Include once

enum Instructions{
    LDA = 1, //Load the Accumulator(ACC)
    STA = 2, //Store the Accumulator(ACC)
    ADD = 3, //ADD
    JMP = 4, //Jump to address
    HLT = 255 //Halt
};