import hashlib
import os
import random
import subprocess
import uuid
from unittest import TestCase

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


def md5(path):
    checksum = hashlib.md5()
    with open(path, 'rb') as f:
        while True:
            data = f.read(8192)
            if not data:
                break
            checksum.update(data)
    return checksum.digest()


def lfq_encode(
    exe_path, fastq_path, lfq_path, header_path=None, qual_path=None
):
    command = [exe_path, 'encode', fastq_path]
    command += ['-o', lfq_path]
    if header_path:
        command += ['-h', header_path]
    if qual_path:
        command += ['-q', qual_path]
    return subprocess.run(command, check=True)


def lfq_decode(
    exe_path, lfq_path, fastq_path, header_path=None, qual_path=None
):
    command = [exe_path, 'decode', lfq_path]
    command += ['-o', fastq_path]
    if header_path:
        command += ['-h', header_path]
    if qual_path:
        command += ['-q', qual_path]
    return subprocess.run(command, check=True)


class TestMixin(TestCase):

    @classmethod
    def setUpClass(cls):
        cls.base_dir = os.path.dirname(os.path.abspath(__file__))
        cls.fixtures_dir = os.path.join(cls.base_dir, 'fixtures')
        cls.build_dir = os.path.join(os.path.dirname(cls.base_dir), 'build')
        cls.exe_path = os.path.join(cls.build_dir, 'src', 'lfq')
