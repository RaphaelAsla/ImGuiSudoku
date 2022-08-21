# Sudoku generator and solver, easy to use.
First of all you need to setup imgui-sfml <code>https://github.com/eliasdaler/imgui-sfml</code>
then
```bash
mkdir build && cd build
CXX=clang++ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja
./sudoku
```
<br>
<strong>Note that "difficulty" is relative, more empty cells doesn't necessarily mean that its a harder puzzle</strong>
