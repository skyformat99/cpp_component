# cpp_component
对 cpp 一些常用的功能进行封装

windows 编译:
vs2015:
  在项目最外层建立一个编译目录 -> build_vs2015_win64
  进入 build_vs2015_win64
  cmake .. -G "Visual Studio 14 Win64"
  
linux 编译:
  进入 script 目录
  ./start_compile_linux64_with_gcc.sh release rebuild
