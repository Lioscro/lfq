#!/usr/bin/env python3
import argparse
import os
import shutil
import tempfile

from utils import lfq_decode, lfq_encode, md5


def test_encode_decode(exe_path, fastq_path):
    temp_dir = tempfile.mkdtemp()
    lfq_path = os.path.join(temp_dir, 'output.lfq')
    header_path = os.path.join(temp_dir, 'output.lfqh')
    qual_path = os.path.join(temp_dir, 'output.lfqq')
    decode_path = os.path.join(temp_dir, 'decoded.fastq')

    try:
        lfq_encode(exe_path, fastq_path, lfq_path, header_path, qual_path)
        lfq_decode(exe_path, lfq_path, decode_path, header_path, qual_path)

        assert md5(fastq_path) == md5(decode_path)
    finally:
        shutil.rmtree(temp_dir, ignore_errors=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('exe', type=str)
    parser.add_argument('fastq', type=str)
    args = parser.parse_args()
    test_encode_decode(args.exe, args.fastq)
