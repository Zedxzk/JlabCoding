a = {}
b = {}
d = {}
c = [a, b]
a[1] = 8
b[1] = 8
print(a)
print(c)
d.update(a)
d.update(b)
print(d)