import random

n = 2  # You can change this to any size you want, for example, 1000 for a 1000x1000 matrix.

matrix1 = []
matrix2 = []
result_matrix = []

# Generating the first matrix with random values
for _ in range(n):
    row = [random.random() for _ in range(n)]
    matrix1.append(row)

# Generating the second matrix with random values
for _ in range(n):
    row = [random.random() for _ in range(n)]
    matrix2.append(row)

# Performing matrix multiplication
for i in range(n):
    result_row = []
    for j in range(n):
        product_sum = sum(matrix1[i][k] * matrix2[k][j] for k in range(n))
        result_row.append(product_sum)
    result_matrix.append(result_row)

# Formatting the matrices as strings for printing
def matrix_to_string(matrix):
    return "{" + ", ".join("{" + ", ".join(f"{val:.4f}" for val in row) + "}" for row in matrix) + "}"

print("First Matrix:")
print(matrix_to_string(matrix1))
print("Second Matrix:")
print(matrix_to_string(matrix2))
print("Result of Multiplication:")
print(matrix_to_string(result_matrix))
