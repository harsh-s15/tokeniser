<h1>Introduction</h1>
A tokeniser module for visualising how the first steps of compilation of any given C++ program results in a token stream.

<h3><u>Lexical Analyser: token generator</u></h3>

Manual :

1. Download this repository/zip file onto your system and navigate to this directory in your terminal.
2. Make sure you have the g++ compiler installed.
3. Compile the program <code>main.cpp</code><br>by typing <br><code>g++ main.cpp -o FSM</code><br>in your terminal.
4. Run the executable with the input file passed as an argument, that is, 
<br><code>./FSM input.in</code>

Results:

- After the entire scanning of input.in, the symbols get stored in a new file <code>symbol_table_1.out</code>
in the same directory.
- tokens with ID and attribute (if present) get stored in the specified format in another newly created file <code>pa_1.out</code>
