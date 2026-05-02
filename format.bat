@echo off
echo format the source code using clang-format...
for /r "Engine" %%f in (*.cpp *.h *.hpp) do (
    echo Processing %%~nxf
    "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\bin\clang-format.exe" -i "%%f"
)
for /r "RogaliqueGame" %%f in (*.cpp *.h *.hpp) do (
    echo Processing %%~nxf
    "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\bin\clang-format.exe" -i "%%f"
)
echo All done!
pause