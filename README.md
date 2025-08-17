# braintercourse
A Brainfuck interpreter with real-time visual representation of memory, input, and output. 
Built with [Raylib](https://www.raylib.com/)


<div align="center">
  <img alt="Logo" src="https://github.com/del-Real/braintercourse/blob/main/demo.png" width="600" />
</div>


## Operation rules

| Command | Description |
|---------|-------------|
| `>` | Move data pointer one cell to the right |
| `<` | Move data pointer one cell to the left |
| `+` | Increment the value in the current cell |
| `-` | Decrement the value in the current cell |
| `[` | Jump forward past matching `]` if current cell is 0 |
| `]` | Jump back after matching `[` if current cell is not 0 |
| `.` | Output the current cell as ASCII character |
| `,` | Read one byte of input into the current cell |

## Usage

```bash
./braintercourse "<brainfuck program>"
```

Example (Hello World!):
```bash
./braintercourse ">>+<--[[<++>->-->+++>+<<<]-->++++]<<.<<-.<<..+++.>.<<-.>.+++.------.>>-.<+.>>."
```
