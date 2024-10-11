N = int(input("Введіть кількість рядків: "))

lines = []

for _ in range(N):
    line = input("Введіть рядок з парною кількістю символів: ")
    if len(line) % 2 == 0:
        mid = len(line) // 2
        line = line[:mid-1] + line[mid-1:mid+1].upper() + line[mid+1:]
        lines.append(line)
    else:
        print("Рядок повинен мати парну довжину!")

for modified_line in lines:
    print(modified_line)
