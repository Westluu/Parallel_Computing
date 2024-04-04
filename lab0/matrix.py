import random

n = 1000 #1000

matrix1s = "{"
matrix1 = []
matrix2s = "{"
matrix2 = []
matrix3s = "{"
matrix3 = []

for _ in range(n):
    row = []
    for _ in range(n):
        row.append(random.random())
    matrix1s += "{" + str(row)[1:-1] + "}, "
    matrix1.append(row)
matrix1s = matrix1s[:-2] + "}"

for _ in range(n):
    row = []
    for _ in range(n):
        row.append(random.random())
    matrix2s += "{" + str(row)[1:-1] + "}, "
    matrix2.append(row)
matrix2s = matrix2s[:-2] + "}"

for i in range(n):
    row = []
    for j in range(n):
        row.append(matrix1[i][j] + matrix2[i][j])
    matrix3s += "{" + str(row)[1:-1] + "}, "
matrix3s = matrix3s[:-2] + "}"


print("First:")
print(matrix1s)
print("Second:")
print(matrix2s)
print("Sum:")
print(matrix3s)