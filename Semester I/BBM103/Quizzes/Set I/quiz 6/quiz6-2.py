# Hacettepe University
# Kayla Akyüz
# 21726914

n = int(input())
a = {p: [" "] * p for p in range(2 * n - 1, -1, -1)}
print(a)
b = {p: ["*"] * p for p in range(1, 2 * n + 1, 2)}
print(b)
for p in range(1, n + 1):
    print("".join(a[n - p]), "".join(b[2 * p - 1]), "".join(a[p]), end="\n")
for p in range(n - 1, 0, -1):
    print("".join(a[n - p]), "".join(b[2 * p - 1]), "".join(a[p]), end="\n")
