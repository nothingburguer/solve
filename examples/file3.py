memory = {}
labels = {}
program = []
pc = 0


def parse(lines):
    global program, labels

    for line in lines:
        line = line.strip()

        if not line or line.startswith("#"):
            continue

        if line.endswith(":"):
            labels[line[:-1]] = len(program)
            continue

        program.append(line.split())


def get(x):
    if x.lstrip("-").isdigit():
        return int(x)
    return memory.get(x, 0)


def run():
    global pc

    while pc < len(program):
        inst = program[pc]

        op = inst[0]

        if op == "data":
            memory[inst[1]] = int(inst[2])
            pc += 1

        elif op == "sub":
            a, b, c = inst[1], inst[2], inst[3]

            memory[b] = get(b) - get(a)

            if memory[b] <= 0:
                pc = labels[c]
            else:
                pc += 1

        elif op == "print":
            print(memory.get(inst[1], 0))
            pc += 1

        elif op == "halt":
            break

        else:
            print("unknown instruction:", op)
            break


code = [
    "data n 3",
    "data one 1",
    "data zero 0",

    "loop:",
    "sub one n end",
    "print n",
    "sub zero zero loop",

    "end:",
    "halt"
]

parse(code)
run()

