import random
import string

def generate_string():
    letters = string.ascii_uppercase
    digits = string.digits
    parts = [''.join(random.choices(letters, k=4)) for _ in range(3)]
    parts += [''.join(random.choices(digits, k=4)) for _ in range(5)]
    print('-'.join(parts))
    return '-'.join(parts)

with open('information.txt', 'w') as file:
    for _ in range(4):
        file.write(generate_string() + '\n')