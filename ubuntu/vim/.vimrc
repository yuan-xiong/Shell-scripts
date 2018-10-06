" automatically add Author information in vim file.

" 当新建 .h .c .hpp .cpp .mk .sh等文件时自动调用SetTitle 函数

"autocmd BufNewFile *.[ch],*.hpp,*.cpp,Makefile,*.mk,*.sh exec ":call SetTitle()"
"autocmd BufNewFile *.[ch],*.hpp,*.cpp,Makefile,*.mk,*.sh* exec ":call SetTitle()"


" 加入注释
func SetComment()
    call setline(1,"/*================================================================")
    call append(line("."),   "*   Copyright (C) ".strftime("%Y")." XY. All rights reserved.")
    call append(line(".")+1, "*   ")
    call append(line(".")+2, "*   文件名称：".expand("%:t"))
    call append(line(".")+3, "*   创 建 者：Xiong Yuan")
    call append(line(".")+4, "*   创建日期：".strftime("%Y年%m月%d日"))
    call append(line(".")+5, "*   描    述：")
    call append(line(".")+6, "*")
    call append(line(".")+7, "================================================================*/")
    call append(line(".")+8, "")
    call append(line(".")+9, "")
endfunc

" 加入shell,Makefile注释
func SetComment_sh()
    call setline(3, "#================================================================")
    call setline(4, "#   Copyright (C) ".strftime("%Y")." XY. All rights reserved.")
    call setline(5, "#   ")
    call setline(6, "#   文件名称：".expand("%:t"))
    call setline(7, "#   创 建 者：Xiong Yuan")
    call setline(8, "#   创建日期：".strftime("%Y年%m月%d日"))
    call setline(9, "#   描    述：")
    call setline(10, "#")
    call setline(11, "#================================================================")
    call setline(12, "")
    call setline(13, "")
endfunc

" 定义函数SetTitle，自动插入文件头
func SetTitle()

    if &filetype == 'make'
        call setline(1,"")
        call setline(2,"")
        call SetComment_sh()

    else
         call SetComment()
         if expand("%:e") == 'hpp'
          call append(line(".")+10, "#ifndef _".toupper(expand("%:t:r"))."_H")
          call append(line(".")+11, "#define _".toupper(expand("%:t:r"))."_H")
          call append(line(".")+12, "#ifdef __cplusplus")
          call append(line(".")+13, "extern \"C\"")
          call append(line(".")+14, "{")
          call append(line(".")+15, "#endif")
          call append(line(".")+16, "")
          call append(line(".")+17, "#ifdef __cplusplus")
          call append(line(".")+18, "}")
          call append(line(".")+19, "#endif")
          call append(line(".")+20, "#endif //".toupper(expand("%:t:r"))."_H")

         elseif expand("%:e") == 'h'
        call append(line(".")+10, "#pragma once")

         elseif &filetype == 'c'
        call append(line(".")+10,"#include \"".expand("%:t:r").".h\"")

         elseif &filetype == 'cpp'
        call append(line(".")+10, "#include \"".expand("%:t:r").".h\"")

         endif

    endif
endfunc



set number
set cursorline
set showcmd
set showmatch
set ruler
set hlsearch
set completeopt=longest,menu
set wildmenu

" ctags
set tags=/home/xiongyuan/workspace/git/autonomousdriving_flex-tinyyolo_with_avm/tags
" cscope

" Tlist
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1
" open taglist
"let Tlist_Auto_Open	= 1
" close taglist
let Tlist_Auto_Open	= 0

let Tlist_Ctags_Cmd='/usr/bin/ctags'
