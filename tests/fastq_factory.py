#!/usr/bin/env python3
import argparse
import random
import uuid

UPPER = ['A', 'T', 'C', 'G', 'N']
LOWER = ['a', 't', 'c', 'g', 'n']
QUALS = [chr(i + 33) for i in range(41)]


def fastq_factory(out_path, n, short, long, upper=False, lower=False):
    alphabet = UPPER + LOWER
    if upper:
        alphabet = UPPER
    if lower:
        alphabet = LOWER
    with open(out_path, 'w') as f:
        for i in range(n):
            length = random.randint(short, long)
            f.write(f'@{str(i).zfill(len(str(n-1)))} {uuid.uuid4()}\n')
            f.write(''.join(random.choices(alphabet, k=length)) + '\n')
            f.write('+\n')
            f.write(''.join(random.choices(QUALS, k=length)) + '\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', type=str, required=True)
    parser.add_argument('-n', type=int, required=True)
    parser.add_argument('-r', type=str, required=True)
    flag_group = parser.add_mutually_exclusive_group()
    flag_group.add_argument('--upper', action='store_true')
    flag_group.add_argument('--lower', action='store_true')
    args = parser.parse_args()

    if ':' in args.r:
        split = args.r.split(':')
        short = int(split[0])
        long = int(split[1])
    else:
        short = long = int(args.r)
    fastq_factory(
        args.o, args.n, short, long, upper=args.upper, lower=args.lower
    )
