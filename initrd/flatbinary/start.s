#extern main

.section .text
    # push argv
    # push argc
    call main
    # main has returned, eax is return value
    ret
