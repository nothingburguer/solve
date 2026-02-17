def calc(a, b, op):
    if op == "+":
        return a + b
    elif op == "-":
        return a - b
    elif op == "*":
        return a * b
    elif op == "/":
        if b == 0:
            return "ERROR: Dividing zero"
        return a / b
    else:
        return "ERROR: Invalid operator"


print("solve calc")
x = 10
y = 5

print("10 + 5 =", calc(x, y, "+"))
print("10 - 5 =", calc(x, y, "-"))
print("10 * 5 =", calc(x, y, "*"))
print("10 / 5 =", calc(x, y, "/"))

