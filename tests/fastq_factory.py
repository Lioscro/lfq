#!/usr/bin/env python3
import argparse

from mixins import fastq_factory

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
