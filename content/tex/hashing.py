import sys
import hashlib
import subprocess

lines = subprocess.run("cpp -dD -P -fpreprocessed %s 2> /dev/null" % sys.argv[1], shell=True, capture_output=True).stdout.replace(b" ", b"").split(b"\n")
hashes = [hashlib.md5(b"\n".join(lines[:i+1])).hexdigest()[:2] for i in range(len(lines))]

# pre = 0
# hashes = []
# for line in lines:
#     pre = (pre*42 + int.from_bytes(hashlib.md5(line.encode()).digest()[-1:], byteorder="little")) % 256
#     hashes.append("{:02x}".format(pre))

# ! Das den richtigen Zeilen im Document zuzuordnen ist quasi unmöglich, du solltest hier keine Zeit mehr reinstecken

if __name__ == "__main__": # optional
    print("\n".join(h + " " + l.decode() for (h, l) in zip(hashes, lines)))
