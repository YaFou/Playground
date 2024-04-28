import sys

r = []
def dec2bin(b, i):
    if i == 0:
        return
    if b & 1 == 1:
        r.append(1)
    else:
        r.append(0)
    dec2bin(b >> 1, i - 1)

def bin2dec(s):
    x = 0
    for i in range(0, len(s)):
        x = x << 1
        if s[i] == "1":
            x = x + 1
    return x

f = open(sys.argv[1], "rb")
data = f.read()
g = open("log3", "wb")
for c in data:
    # g.write(bytearray([c]))
    dec2bin(c, 8)
# r.reverse()
chunks = []
width = 9
i = 0
while i < len(r) - width:
    if i == 255 * 9:
        width = 10
    chunk = []
    for j in range(i, i + width):
        chunk.append(r[j])
    chunk.reverse()
    chunks.append(chunk)
    i = i + width
for i in range(0, len(chunks)):
    s = ""
    chunk = chunks[i]
    for j in range(0, len(chunk)):
        if chunk[j] == 1:
            s += "1"
        else:
            s += "0"
        print(chunk[j], end="")
    print(" =", bin2dec(s))